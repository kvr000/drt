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

#ifndef dr__sql__mysql5__SqlStatement_mysql5_direct__hxx__
# define dr__sql__mysql5__SqlStatement_mysql5_direct__hxx__

#include <dr/Array.hxx>

#include <dr/sql/mysql5/def.hxx>

#include <mysql/mysql.h>

#include <dr/sql/StatementDummy.hxx>

DR_SQL_MYSQL5_NS_BEGIN

DR_NS_USE
DR_SQL_NS_USE

class Connection_mysql5;


class Statement_mysql5_direct: public StatementDummy
{
	DR_OBJECT_DECL_SIMPLE(Statement_mysql5_direct, StatementDummy);

public:
	/* constructor */		Statement_mysql5_direct(Connection_mysql5 *conn, const String &stmt_str);
	virtual				~Statement_mysql5_direct();

public:
	virtual void			executeUpdate();

protected:
	Ref<Connection_mysql5>	conn;
	String				stmt_str;
};


DR_SQL_MYSQL5_NS_END

#endif
