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

#include <dr/sql/SqlExcept.hxx>
#include <dr/sql/SqlParseExcept.hxx>

#include <dr/sql/mysql5/SqlConnection_mysql5.hxx>

#include <dr/sql/mysql5/SqlStatement_mysql5_direct.hxx>

#include <mysql/mysql.h>

DRSQL_MYSQL5_NS_BEGIN

DR_OBJECT_DEF(DRSQL_MYSQL5_NS_STR, SqlStatement_mysql5_direct, SqlStatementDummy);
DR_OBJECT_IMPL_SIMPLE(SqlStatement_mysql5_direct);

SqlStatement_mysql5_direct::SqlStatement_mysql5_direct(SqlConnection_mysql5 *conn_, const String &stmt_str_):
	conn(conn_, true),
	stmt_str(stmt_str_)
{
}

SqlStatement_mysql5_direct::~SqlStatement_mysql5_direct()
{
}

void SqlStatement_mysql5_direct::executeUpdate()
{
	BString stmt_utf8(stmt_str.utf8());
	if (mysql_real_query(conn->mysql_handle, stmt_utf8.toStr(), stmt_utf8.getSize()) != 0) {
		SqlConnection_mysql5::throwSqlExcept(mysql_sqlstate(conn->mysql_handle), mysql_error(conn->mysql_handle));
	}
	if (MYSQL_RES *res = mysql_store_result(conn->mysql_handle)) {
		mysql_free_result(res);
	}
	mysql_affected_rows(conn->mysql_handle);
}


DRSQL_MYSQL5_NS_END
