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
#include <dr/Exception.hxx>

#include <mysql/mysql.h>

#include <dr/sql/SqlException.hxx>
#include <dr/sql/SqlUniqueConstraintException.hxx>

#include <dr/sql/mysql5/Statement_mysql5.hxx>
#include <dr/sql/mysql5/Statement_mysql5_direct.hxx>
#include <dr/sql/mysql5/ResultSet_mysql5.hxx>

#include <dr/sql/mysql5/Connection_mysql5.hxx>

DR_SQL_MYSQL5_NS_BEGIN

DR_OBJECT_DEF(DR_SQL_MYSQL5_NS_STR, Connection_mysql5, Connection);
DR_OBJECT_IMPL_SIMPLE(Connection_mysql5);

Connection_mysql5::Connection_mysql5(MYSQL *handle_):
	mysql_handle(handle_),
	auto_reconnect(false),
	use_locks(false),
	busy_rs(NULL)
{
}

Connection_mysql5::~Connection_mysql5()
{
	mysql_close(mysql_handle);
}

bool Connection_mysql5::ping()
{
	int err = mysql_ping(mysql_handle);
	if (err != 0) {
		throwSqlExcept(mysql_sqlstate(mysql_handle), mysql_error(mysql_handle));
	}
	return true;
}

void Connection_mysql5::reconnect()
{
}

void Connection_mysql5::commit()
{
}

void Connection_mysql5::rollback()
{
}

Statement *Connection_mysql5::createStatement(const String &sql)
{
	saveBusy();
	if (MYSQL_STMT *stmt = mysql_stmt_init(mysql_handle)) {
		DR_TRY {
			return new Statement_mysql5(this, stmt, sql);
		}
		DR_CATCHANY {
			mysql_stmt_close(stmt);
			DR_RETHROWANY;
		}
		DR_ENDTRY
	}
	return NULL;
}

Statement *Connection_mysql5::prepareStatement(const String &sql)
{
	ERef<Statement> s(createStatement(sql));
	s->prepare();
	return s.getAndNull();
}

bool Connection_mysql5::prepareLockStatements(Statement **lock_mem, Statement **unlock_mem, int lock_type0, const String *table0, ...)
{
	bool exists = false;
	va_list vl;
	va_start(vl, table0);
	String lock_str("LOCK TABLES ");
	String unlock_str("UNLOCK TABLES");
	if (use_locks || (lock_type0&4)) {
		lock_str.append(*table0);
		lock_str.append(lock_type0 == 1 ? " READ" : " WRITE");
		exists = true;
	}
	for (;;) {
		lock_type0 = va_arg(vl, int);
		if (lock_type0 <= 0)
			break;
		table0 = va_arg(vl, String *);
		if (use_locks || (lock_type0&4)) {
			if (exists) {
				lock_str.append(", ");
			}
			else {
				exists = true;
			}
			lock_str.append(*table0);
			lock_str.append(lock_type0 == 1 ? " READ" : " WRITE");
		}
	}
	if (exists) {
		lock_str.append(";");
		unlock_str.append(";");
		saveBusy();
		DR_REF_XCHG(lock_mem, (Statement *)new Statement_mysql5_direct(this, lock_str));
		DR_REF_XCHG(unlock_mem, (Statement *)new Statement_mysql5_direct(this, unlock_str));
	}
	return exists;
}

bool Connection_mysql5::prepareLockStatements(Statement **lock_mem, Statement **unlock_mem, int *types, const String *tables, size_t count)
{
	bool exists = false;
	String lock_str("LOCK TABLES ");
	String unlock_str("UNLOCK TABLES");
	for (size_t i = 0; i < count; i++) {
		if (use_locks || (types[i]&4)) {
			if (exists) {
				lock_str.append(", ");
			}
			else {
				exists = true;
			}
			lock_str.append(tables[i]);
			lock_str.append(types[i] == 1 ? " READ" : " WRITE");
		}
	}
	if (exists) {
		lock_str.append(";");
		unlock_str.append(";");
		saveBusy();
		DR_REF_XCHG(lock_mem, (Statement *)new Statement_mysql5_direct(this, lock_str));
		DR_REF_XCHG(unlock_mem, (Statement *)new Statement_mysql5_direct(this, unlock_str));
	}
	return exists;
}

void Connection_mysql5::processAllBusy()
{
	if (busy_rs) {
		busy_rs->store();
		busy_rs = NULL;
	}
}

int Connection_mysql5::parseSqlCode(const char *code_str)
{
	int code = 0;
	for (; *code_str; code_str++) {
		if (!isdigit(*code_str)) {
			code = -1;
			break;
		}
		code = code*10+code_str[0]-'0';
	}
	return code;
}

void Connection_mysql5::throwSqlExcept(const char *code_str, const char *error_desc)
{
	int code = parseSqlCode(code_str);
	SqlException *ex;
	switch (code) {
	case 23000:
		ex = new SqlUniqueConstraintException(code, error_desc);
		break;

	default:
		ex = new SqlException(code, error_desc);
		break;
	}
	xthrowmove(ex);
}


DR_SQL_MYSQL5_NS_END
