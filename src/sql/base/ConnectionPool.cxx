/**
 * drt library
 *
 * drt multiplatform development toolkit
 *
 * ----------------------------------------------------------------------------------
 *
 * Copyright (C) 2004-2008 Zbyněk Vyškovský
 *
 * ----------------------------------------------------------------------------------
 *
 * LICENSE:
 *
 * This file is part of drt
 *
 * drt is free software; you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option)
 * any later version.
 *
 * drt is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with drt; if not, write to the Free Software Foundation, Inc., 51
 * Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * @copyright	2004-2008 Zbyněk Vyškovský
 * @link	mailto:kvr@matfyz.cz
 * @link	http://kvr.matfyz.cz/drt/
 * @license	http://www.gnu.org/licenses/lgpl.txt GNU Lesser General Public License v3
 **/

/*drt
 * include:	dr/List.hxx
 * include:	dr/MutexCond.hxx
 * include:	dr/Time.hxx
 *
 * include:	dr/sql/ConnectionHold.hxx
 * include:	dr/sql/dev/SqlManager.hxx
 *
 * ns:		dr::sql
 */

#include <dr/x_kw.hxx>
#include <dr/Exception.hxx>

#include <dr/sql/ConnectionPool.hxx>
#include "_gen/ConnectionPool-all.hxx"

DR_SQL_NS_BEGIN


/*drt
 * class:	ConnectionPool::ConnectionHoldPool
 * ancestor:	ConnectionHold
 *
 * at:	ConnectionPool *		owner;
 * at:	Time::SysTime			created;
 *
 * friend:	class				ConnectionPool;
 */

DR_MET(public)
ConnectionPool::ConnectionHoldPool::ConnectionHoldPool(ConnectionPool *owner_, Connection *conn):
	Super(conn),
	owner(owner_),
	created(Time::getTime())
{
	// we are already under mutex protection
	owner->num_connections++;
}

DR_MET(protected virtual)
ConnectionPool::ConnectionHoldPool::~ConnectionHoldPool()
{
	owner->destroyingConnection(this);
}


/*drt
 * class:	ConnectionPool
 * ancestor:	dr::Object
 *
 * at:	String				connect_str;
 * at:	THash<String, String>		connect_pars;
 * at:	SysTime				max_oldness;
 * at:	int				max_connections;
 * at:	int				num_connections;
 * at:	Ref<MutexCond>			list_mutex;
 * at:	Ref<SqlManager>			manager;
 * at:	RList<ConnectionHoldPool>	connection_list;
 */

DR_MET(public)
ConnectionPool::ConnectionPool(const String &connect_str_, int init_conns):
	connect_str(connect_str_),
	max_oldness(0),
	max_connections(1023),
	num_connections(0),
	list_mutex(MutexCond::create(), false)
{
	if (init_conns == 0)
		init_conns = 1;
	while (init_conns--) {
		ERef<ConnectionHoldPool> conn = new ConnectionHoldPool(this, tref(Connection::openConnection(connect_str, &connect_pars, manager.mem())));
		releaseConnection(conn);
	}
	if (String *v = connect_pars.accValue("max"))
		max_connections = atoi(v->utf8());
}

DR_MET(protected virtual)
ConnectionPool::~ConnectionPool()
{
	connection_list.clean();
}

DR_MET(public virtual)
void ConnectionPool::setMaxOldness(SysTime max_oldness_)
{
	max_oldness = max_oldness_;
}

DR_MET(public virtual)
void ConnectionPool::setMaxConnections(int max_connections_)
{
	max_connections = max_connections_;
}

DR_MET(public virtual)
int ConnectionPool::getMaxConnections()
{
	return max_connections;
}

DR_MET(public virtual)
ConnectionHold *ConnectionPool::getConnection()
{
	MutexCondLocker list_mutex_locker(list_mutex);
	ConnectionHold *conn;
	for (;;) {
		if (RList<ConnectionHoldPool>::Node *n = connection_list.iterFirst()) {
			if (max_oldness > 0 && n->v->created+max_oldness < Time::getTime()) {
				connection_list.remove(n);
			}
			else {
				conn = n->v.getAndNull();
				connection_list.remove(n);
				break;
			}
		}
		else if (num_connections >= max_connections) {
			list_mutex->wait();
		}
		else {
			conn = new ConnectionHoldPool(this, tref(Connection::openConnection(connect_str, &connect_pars, manager.mem())));
			break;
		}
	}
	return conn;
}

DR_MET(public virtual)
ConnectionHold *ConnectionPool::getConnectionPing()
{
	for (;;) {
		ConnectionHold *conn = getConnection();
		xtry {
			conn->ping();
			return conn;
		}
		xcatchany {
			xsafe(conn->unref());
		}
		xend;
	}
}

DR_MET(public virtual)
void ConnectionPool::releaseConnection(ConnectionHold *connection)
{
	MutexCondLocker list_mutex_locker(list_mutex);
	connection_list.insert(nref((ConnectionHoldPool *)connection->getIfaceUnref(ConnectionHoldPool::comp_name)));
	list_mutex->signal();
}

DR_MET(protected virtual)
void ConnectionPool::destroyingConnection(ConnectionHoldPool *connection)
{
	MutexCondLocker list_mutex_locker(list_mutex);

	num_connections--;
	list_mutex->signal();
}


DR_SQL_NS_END
