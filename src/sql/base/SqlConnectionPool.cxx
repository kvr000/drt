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
	list_mutex(Mutex::create(), false)
{
	if (init_conns == 0)
		init_conns = 1;
	while (init_conns--)
		releaseConnection(tref(new SqlConnectionHold(tref(SqlConnection::openConnection(connect_str, manager.mem())))));
}

SqlConnectionPool::~SqlConnectionPool()
{
}

void SqlConnectionPool::setMaxOldness(long max_oldness_)
{
	max_oldness = max_oldness_;
}

SqlConnectionHold *SqlConnectionPool::getConnection()
{
	MutexLocker list_mutex_locker(list_mutex);
	SqlConnectionHold *conn;
	for (;;) {
		if (RList<SqlConnectionHold>::Node *n = connection_list.getFirst()) {
			if (max_oldness > 0 && n->v->created+max_oldness < Time::getTime()) {
				connection_list.remove(n);
			}
			else {
				conn = n->v.getAndNull();
				connection_list.remove(n);
				break;
			}
		}
		else {
			conn = new SqlConnectionHold(tref(SqlConnection::openConnection(connect_str, manager.mem())));
			break;
		}
	}
	return conn;
}

SqlConnectionHold *SqlConnectionPool::getConnectionPing()
{
	MutexLocker list_mutex_locker(list_mutex);
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
	MutexLocker list_mutex_locker(list_mutex);
	connection_list.append(IRef<SqlConnectionHold>(connection));
}


DR_SQL_NS_END
