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

#include <dr/x_kw.hxx>
#include <dr/Const.hxx>
#include <dr/Except.hxx>

#include <dr/Array.hxx>

#include <dr/sql/SqlConnectionPool.hxx>

DR_SQL_NS_BEGIN

DR_OBJECT_DEF(DR_SQL_NS_STR, SqlConnectionPool, SqlConnection);
DR_OBJECT_IMPL_SIMPLE(SqlConnectionPool);


SqlConnectionPool::SqlConnectionPool(const String &connect_str_, int init_conns):
	connect_str(connect_str_),
	max_oldness(0),
	max_connections(1023),
	num_connections(0),
	list_mutex(MutexCond::create(), false)
{
	if (init_conns == 0)
		init_conns = 1;
	while (init_conns--) {
		ERef<SqlConnectionHold> conn = new SqlConnectionHold(tref(SqlConnection::openConnection(connect_str, &connect_pars, manager.mem())));
		num_connections++;
		releaseConnection(conn);
	}
	if (String *v = connect_pars.accValue("max"))
		max_connections = atoi(v->utf8());
}

SqlConnectionPool::~SqlConnectionPool()
{
	connection_list.clean();
}

void SqlConnectionPool::setMaxOldness(SysTime max_oldness_)
{
	max_oldness = max_oldness_;
}

void SqlConnectionPool::setMaxConnections(int max_connections_)
{
	max_connections = max_connections_;
}

SqlConnectionHold *SqlConnectionPool::getConnection()
{
	MutexCondLocker list_mutex_locker(list_mutex);
	SqlConnectionHold *conn;
	for (;;) {
		if (RList<SqlConnectionHold>::Node *n = connection_list.iterFirst()) {
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
			conn = new SqlConnectionHold(this, tref(SqlConnection::openConnection(connect_str, &connect_pars, manager.mem())));
			num_connections++;
			break;
		}
	}
	return conn;
}

SqlConnectionHold *SqlConnectionPool::getConnectionPing()
{
	for (;;) {
		SqlConnectionHold *conn = getConnection();
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

void SqlConnectionPool::releaseConnection(SqlConnectionHold *connection)
{
	MutexCondLocker list_mutex_locker(list_mutex);
	connection_list.append(IRef<SqlConnectionHold>(connection));
	list_mutex->signal();
}

void SqlConnectionPool::destroyingConnection(SqlConnectionHold *connection)
{
	MutexCondLocker list_mutex_locker(list_mutex);

	num_connections--;
	list_mutex->signal();
}


DR_SQL_NS_END
