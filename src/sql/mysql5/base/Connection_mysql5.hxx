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

#ifndef dr__sql__mysql5__SqlConnection_mysql5__hxx__
# define dr__sql__mysql5__SqlConnection_mysql5__hxx__

#include <mysql/mysql.h>

#include <dr/sql/mysql5/def.hxx>

#include <dr/sql/Connection.hxx>

DR_SQL_MYSQL5_NS_BEGIN

DR_NS_USE
DR_SQL_NS_USE


class Connection_mysql5: public Connection
{
	DR_OBJECT_DECL_SIMPLE(Connection_mysql5, Connection);

public:
	/* constructor */		Connection_mysql5(MYSQL *handle);

protected:
	virtual				~Connection_mysql5();

public:
	virtual bool			ping();
	virtual void			reconnect();
	
public:
	virtual void			commit();
	virtual void			rollback();

public:
	virtual Statement *		createStatement(const String &sql);
	virtual Statement *		prepareStatement(const String &sql);
	virtual bool			prepareLockStatements(Statement **lock_mem, Statement **unlock_mem, int lock_type0, const String *table0, ...);
	virtual bool			prepareLockStatements(Statement **lock_mem, Statement **unlock_mem, int *lock_type, const String *lock_tables, size_t count);

public: // within mysql5 implementation
	static void			throwSqlExcept(const char *code, const char *error_desc);
	static int			parseSqlCode(const char *code);

public:
	MYSQL *				mysql_handle;
	bool				auto_reconnect;
	bool				use_locks;

public:
	friend class SqlManager_mysql5;
};


Connection *openConnection(const char *host, const char *user, const char *pass);


DR_SQL_MYSQL5_NS_END

#endif
