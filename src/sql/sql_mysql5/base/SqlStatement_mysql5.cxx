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

#include <stdio.h>

#include <dr/x_kw.hxx>
#include <dr/Const.hxx>
#include <dr/Except.hxx>
#include <dr/UnsupportedExcept.hxx>

#include <dr/sql/SqlExcept.hxx>
#include <dr/sql/SqlParseExcept.hxx>

#include <dr/sql/Date.hxx>
#include <dr/sql/mysql5/SqlConnection_mysql5.hxx>
#include <dr/sql/mysql5/ResultSet_mysql5.hxx>

#include <dr/sql/mysql5/SqlStatement_mysql5.hxx>

#include <mysql/mysql.h>
#include <mysql/errmsg.h>

DRSQL_MYSQL5_NS_BEGIN

DR_OBJECT_DEF(DRSQL_MYSQL5_NS_STR, SqlStatement_mysql5, SqlStatement);
DR_OBJECT_IMPL_SIMPLE(SqlStatement_mysql5);

SqlStatement_mysql5::SqlStatement_mysql5(SqlConnection_mysql5 *conn_, MYSQL_STMT *stmt_, String stmt_str_):
	conn(conn_, true),
	stmt_str(stmt_str_),
	limit(-1),
	offset(-1),
	stmt(stmt_)
{
}

SqlStatement_mysql5::~SqlStatement_mysql5()
{
	for (size_t i = 0; i < par_bindings.count(); i++) {
		if (void *p = par_bindings[i].buffer)
			Alloc::free(p);
	}
	if (stmt)
		mysql_stmt_close(stmt);
}

void SqlStatement_mysql5::recreateStmt()
{
	mysql_stmt_close(stmt);
	if ((stmt = mysql_stmt_init(conn->mysql_handle)) == NULL)
		SqlConnection_mysql5::throwSqlExcept(mysql_sqlstate(conn->mysql_handle), mysql_error(conn->mysql_handle));
	prepare();
}

MYSQL_BIND *SqlStatement_mysql5::allocParBinding(unsigned idx)
{
	unsigned orig_size;
	if (idx >= (orig_size = par_bindings.count())) {
		par_bindings.resize(idx+1);
		memset(&par_bindings[orig_size], 0, (idx+1-orig_size)*sizeof(MYSQL_BIND));
	}
	else {
		if (par_bindings[idx].buffer != NULL)
			Alloc::free(par_bindings[idx].buffer);
		memset(&par_bindings[idx], 0, sizeof(MYSQL_BIND));
	}
	return &par_bindings[idx];
}

void SqlStatement_mysql5::setLimit(long limit_)
{
	limit = limit_;
}

void SqlStatement_mysql5::setOffset(long offset_)
{
	offset = offset_;
}

void SqlStatement_mysql5::setOffsetLimit(long offset_, long limit_)
{
	offset = offset_;
	limit = limit_;
}

void SqlStatement_mysql5::prepare()
{
	String s(stmt_str);
	if (offset >= 0 || limit >= 0) {
		if (offset < 0) {
			s.append(" LIMIT ").appendNumber((Uint64)limit);
		}
		else if (limit < 0) {
			s.append(" LIMIT ").appendNumber((Uint64)offset).append(" , 2147483647");
		}
		else {
			s.append(" LIMIT ").appendNumber((Uint64)offset).append(", ").appendNumber((Uint64)limit);
		}
	}
	if (mysql_stmt_prepare(stmt, s.utf8().toStr(), s.utf8().getSize()) != 0) {
		//printf("err=%s\n", mysql_stmt_error(stmt));
		xthrownew(SqlParseExcept(s, SqlConnection_mysql5::parseSqlCode(mysql_stmt_sqlstate(stmt)), mysql_stmt_error(stmt)));
	}
}

void SqlStatement_mysql5::bindParamNull(unsigned column)
{
	MYSQL_BIND *b = allocParBinding(column);
	b->buffer_type = MYSQL_TYPE_NULL;
}

void SqlStatement_mysql5::bindParam(unsigned column, Sint8 value)
{
	MYSQL_BIND *b = allocParBinding(column);
	b->buffer_type = MYSQL_TYPE_TINY;
	b->buffer = Alloc::alloc(sizeof(Sint8));
	*(Sint8 *)b->buffer = value;
}

void SqlStatement_mysql5::bindParam(unsigned column, Sint16 value)
{
	MYSQL_BIND *b = allocParBinding(column);
	b->buffer_type = MYSQL_TYPE_SHORT;
	b->buffer = Alloc::alloc(sizeof(Sint16));
	*(Sint16 *)b->buffer = value;
}

void SqlStatement_mysql5::bindParam(unsigned column, Sint32 value)
{
	MYSQL_BIND *b = allocParBinding(column);
	b->buffer_type = MYSQL_TYPE_LONG;
	b->buffer = Alloc::alloc(sizeof(Sint32));
	*(Sint32 *)b->buffer = value;
}

void SqlStatement_mysql5::bindParam(unsigned column, Sint64 value)
{
	MYSQL_BIND *b = allocParBinding(column);
	b->buffer_type = MYSQL_TYPE_LONGLONG;
	b->buffer = Alloc::alloc(sizeof(Sint64));
	*(Sint64 *)b->buffer = value;
}

void SqlStatement_mysql5::bindParam(unsigned column, const String &value)
{
	MYSQL_BIND *b = allocParBinding(column);
	b->buffer_type = MYSQL_TYPE_VAR_STRING;
	b->buffer = Alloc::alloc(value.utf8().getSize());
	memcpy(b->buffer, value.utf8().toStr(), value.utf8().getSize());
	b->buffer_length = value.utf8().getSize();
}

void SqlStatement_mysql5::bindParam(unsigned column, const Date &value)
{
	/* TODO */
	MYSQL_BIND *b = allocParBinding(column);
	b->buffer_type = MYSQL_TYPE_DATE;
	//MYSQL_TIME *mt = (MYSQL_TIME *)b->buffer = Alloc::alloc(sizeof(MYSQL_TIME));
	//memcpy(b->buffer, value.utf8().toStr(), value.utf8().getSize());
}

void SqlStatement_mysql5::bindParam(unsigned column, const Blob &value)
{
	MYSQL_BIND *b = allocParBinding(column);
	b->buffer_type = MYSQL_TYPE_VAR_STRING;
	b->buffer = Alloc::alloc(value.getSize());
	memcpy(b->buffer, value.toStr(), value.getSize());
	b->buffer_length = value.getSize();
}

void SqlStatement_mysql5::executeUpdate()
{
	if (!stmt)
		recreateStmt();
retry:
	if (par_bindings.count()) {
		if (mysql_stmt_param_count(stmt) != par_bindings.count())
			xthrownew(SqlExcept(-1, "missing binding"));
		if (mysql_stmt_bind_param(stmt, &par_bindings[0]) != 0) {
			SqlConnection_mysql5::throwSqlExcept(mysql_stmt_sqlstate(stmt), mysql_stmt_error(stmt));
		}
	}
	if (mysql_stmt_execute(stmt) != 0) {
		if (conn->auto_reconnect && (stmt->last_errno == CR_SERVER_LOST || stmt->last_errno == 0) && conn->mysql_handle->net.last_errno == 0) {
			recreateStmt();
			goto retry;
		}
		SqlConnection_mysql5::throwSqlExcept(mysql_stmt_sqlstate(stmt), mysql_stmt_error(stmt));
	}
	mysql_stmt_free_result(stmt);
}

ResultSet *SqlStatement_mysql5::executeQuery()
{
	if (!stmt)
		recreateStmt();
retry:
	if (par_bindings.count()) {
		if (mysql_stmt_param_count(stmt) != par_bindings.count())
			xthrownew(SqlExcept(-1, "missing binding"));
		if (mysql_stmt_bind_param(stmt, &par_bindings[0]) != 0) {
			SqlConnection_mysql5::throwSqlExcept(mysql_stmt_sqlstate(stmt), mysql_stmt_error(stmt));
		}
	}
	if (mysql_stmt_execute(stmt) != 0) {
		if (conn->auto_reconnect && (stmt->last_errno == CR_SERVER_LOST || stmt->last_errno == 0) && conn->mysql_handle->net.last_errno == 0) {
			recreateStmt();
			goto retry;
		}
		SqlConnection_mysql5::throwSqlExcept(mysql_stmt_sqlstate(stmt), mysql_stmt_error(stmt));
	}
	xtry {
		return new ResultSet_mysql5(this);
	}
	xcatchany {
		mysql_stmt_free_result(stmt);
		xrethrowany;
	}
	xend;
}

Uint64 SqlStatement_mysql5::getAffectedRows()
{
	my_ulonglong r;
	const char *info = mysql_info(stmt->mysql);
	if ((info = strchr(info, ':')))
		return strtoll(info+1, (char **)&info, 0);
	if ((r = mysql_stmt_num_rows(stmt)) == (my_ulonglong)-1) {
		SqlConnection_mysql5::throwSqlExcept(mysql_stmt_sqlstate(stmt), mysql_stmt_error(stmt));
	}
	return r;
}

Uint64 SqlStatement_mysql5::getInsertId()
{
	my_ulonglong r;
	if ((r = mysql_stmt_insert_id(stmt)) == (my_ulonglong)-1) {
		SqlConnection_mysql5::throwSqlExcept(mysql_stmt_sqlstate(stmt), mysql_stmt_error(stmt));
	}
	return r;
}


DRSQL_MYSQL5_NS_END
