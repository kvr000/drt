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

#ifndef dr__sql__mysql5__SqlManager_mysql5__hxx__
# define dr__sql__mysql5__SqlManager_mysql5__hxx__

#include <dr/Subsystem.hxx>
#include <dr/Hash.hxx>

#include <dr/sql/mysql5/def.hxx>

#include <dr/sql/dev/SqlManager.hxx>

DRSQL_MYSQL5_NS_BEGIN

DR_NS_USE


class SqlManager_mysql5: public SqlManager
{
	DR_OBJECT_DECL_SIMPLE(SqlManager, Object);

public:
	virtual SqlConnection *		openConnection(THash<String, String> *conn_str);
};


class Sql_SS_mysql5: public Subsystem
{
public:
	/**/				Sql_SS_mysql5();
	virtual				~Sql_SS_mysql5();

public:
	Object *			create(const String &object);
	void *				create(const String &object, const String &iface);
	Object *			create(int object);
	void *				create(int object, int iface);
};


DRSQL_MYSQL5_NS_END

#endif
