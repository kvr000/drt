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

#ifndef dr__sql__SqlConnectionPool_hxx__
# define dr__sql__SqlConnectionPool_hxx__

#include <dr/List.hxx>
#include <dr/MutexCond.hxx>
#include <dr/Time.hxx>

#include <dr/sql/SqlConnectionHold.hxx>
#include <dr/sql/dev/SqlManager.hxx>

DR_SQL_NS_BEGIN

DR_NS_USE


class SqlConnectionPool: public Object
{
	DR_OBJECT_DECL_SIMPLE(SqlConnection, Object);

public:
	DR_CONSTRUCT			SqlConnectionPool(const String &conn_str, int init_conns = 0);

protected:
	virtual				~SqlConnectionPool();

public:
	virtual SqlConnectionHold *	getConnection();
	virtual SqlConnectionHold *	getConnectionPing();
	virtual void			releaseConnection(SqlConnectionHold *connection);

public:
	virtual void			setMaxOldness(SysTime oldness);
	virtual void			setMaxConnections(int num_connections);
	virtual int			getMaxConnections();

protected:
	virtual void			destroyingConnection(SqlConnectionHold *connection);

protected:
	String				connect_str;
	THash<String, String>		connect_pars;
	SysTime				max_oldness;
	int				max_connections;
	int				num_connections;
	Ref<MutexCond>			list_mutex;
	Ref<SqlManager>			manager;
	RList<SqlConnectionHold>	connection_list;

public:
	friend class SqlConnectionHold;
};


DR_SQL_NS_END

#endif
