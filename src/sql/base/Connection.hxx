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

#ifndef dr__sql__SqlConnection_hxx__
# define dr__sql__SqlConnection_hxx__

#include <dr/sql/def.hxx>

#include <dr/Hash.hxx>

DR_SQL_NS_BEGIN

DR_NS_USE


class Statement;
class SqlManager;


class Connection: public Object
{
	DR_OBJECT_DECL_SIMPLE(Connection, Object);

public:
	virtual bool			ping() = 0;
	virtual void			reconnect() = 0;

public:
	virtual void			commit() = 0;
	virtual void			rollback() = 0;

public:
	virtual Statement *		createStatement(const String &sql) = 0;
	virtual Statement *		prepareStatement(const String &sql) = 0;
	virtual bool			prepareLockStatements(Statement **lock_mem, Statement **unlock_mem, int lock_type0, const String *table0, ...) = 0;
	virtual bool			prepareLockStatements(Statement **lock_mem, Statement **unlock_mem, int *lock_type, const String *lock_tables, size_t count) = 0;

public:
	static Connection *		openConnection(const String &conn_str);
	static Connection *		openConnection(const String &conn_str, THash<String, String> *pars, SqlManager **manager);
};


DR_SQL_NS_END

#endif
