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

#include <mysql/mysql.h>

DRSQL_MYSQL5_NS_BEGIN

DR_OBJECT_DEF(DRSQL_MYSQL5_NS_STR, ResultSet_mysql5, ResultSet);
DR_OBJECT_IMPL_SIMPLE(ResultSet_mysql5);

ResultSet_mysql5::ResultSet_mysql5(SqlStatement_mysql5 *statement_):
	statement(statement_, true),
	stmt(statement_->stmt),
	col_names((size_t)0, (const String *)NULL),
	res_conversions(NULL),
	res_binding_done(false),
	stored(false)
{
}

ResultSet_mysql5::~ResultSet_mysql5()
{
	mysql_stmt_free_result(stmt);
	mysql_stmt_reset(stmt);
	statement->is_busy = false;

	bindResultReset();
}

void ResultSet_mysql5::initColNames()
{
	if (stmt->field_count == 0)
		return;
	col_names.reinitArraySizedConst(stmt->field_count, &stmt->fields[0].name, sizeof(MYSQL_FIELD));
}

MYSQL_BIND *ResultSet_mysql5::allocResBinding(unsigned idx)
{
	unsigned orig_size;
	if (idx >= (orig_size = res_bindings.count())) {
		res_bindings.resize(idx+1);
		memset(&res_bindings[orig_size], 0, (idx+1-orig_size)*sizeof(MYSQL_BIND));
		while (orig_size <= idx) {
			res_bindings[orig_size].buffer_type = MYSQL_TYPE_NULL;
			orig_size++;
		}
	}
	res_binding_done = false;
	return &res_bindings[idx];
}

ResultSet_mysql5::conversion *ResultSet_mysql5::addResConversion(void (*conv_func)(ResultSet_mysql5 *this_, conversion *conv), int column, size_t bufsize, void *user_var)
{
	conversion *conv = (conversion *)Alloc::alloc(sizeof(conversion)+bufsize);

	conv->convertor = conv_func;
	conv->user_var = user_var;
	conv->column = column;
	conv->next = res_conversions;

	res_conversions = conv;

	return conv;
}

void ResultSet_mysql5::resConv_String(ResultSet_mysql5 *this_, conversion *conv)
{
	*(String *)conv->user_var = this_->getString(conv->column);
}

void ResultSet_mysql5::resConv_Blob(ResultSet_mysql5 *this_, conversion *conv)
{
	*(Blob *)conv->user_var = this_->getBlob(conv->column);
}

void ResultSet_mysql5::resConv_Variant(ResultSet_mysql5 *this_, conversion *conv)
{
	DR_REF_XCHG((Variant **)conv->user_var, this_->getVariant(conv->column));
}

void ResultSet_mysql5::resConv_nullHandler(ResultSet_mysql5 *this_, conversion *conv)
{
	if (*this_->res_bindings[conv->column].is_null) {
		((void (*)(void *))conv->user_var)(this_->res_bindings[conv->column].buffer);
	}
}

void ResultSet_mysql5::bindResultReset()
{
	res_binding_done = false;
	for (size_t i = res_bindings.count(); i-- > 0; ) {
		res_bindings[i].buffer_type = MYSQL_TYPE_NULL;
		res_bindings[i].is_null = NULL;
	}
	for (conversion *i = res_conversions; i; ) {
		conversion *n = i->next;
		Alloc::free(i);
		i = n;
	}
}

void ResultSet_mysql5::bindResult(unsigned column, Sint8 *value)
{
	MYSQL_BIND *rb = allocResBinding(column);
	rb->buffer_type = MYSQL_TYPE_TINY;
	rb->buffer = value;
}

void ResultSet_mysql5::bindResult(const String &column, Sint8 *value)
{
	return bindResult(getColumnIdDirect(column), value);
}

void ResultSet_mysql5::bindResult(unsigned column, Sint16 *value)
{
	MYSQL_BIND *rb = allocResBinding(column);
	rb->buffer_type = MYSQL_TYPE_SHORT;
	rb->buffer = value;
}

void ResultSet_mysql5::bindResult(const String &column, Sint16 *value)
{
	return bindResult(getColumnIdDirect(column), value);
}

void ResultSet_mysql5::bindResult(unsigned column, Sint32 *value)
{
	MYSQL_BIND *rb = allocResBinding(column);
	rb->buffer_type = MYSQL_TYPE_LONG;
	rb->buffer = value;
}

void ResultSet_mysql5::bindResult(const String &column, Sint32 *value)
{
	return bindResult(getColumnIdDirect(column), value);
}

void ResultSet_mysql5::bindResult(unsigned column, Sint64 *value)
{
	MYSQL_BIND *rb = allocResBinding(column);
	rb->buffer_type = MYSQL_TYPE_LONGLONG;
	rb->buffer = value;
}

void ResultSet_mysql5::bindResult(const String &column, Sint64 *value)
{
	return bindResult(getColumnIdDirect(column), value);
}

void ResultSet_mysql5::bindResult(unsigned column, double *value)
{
	MYSQL_BIND *rb = allocResBinding(column);
	rb->buffer_type = MYSQL_TYPE_DOUBLE;
	rb->buffer = value;
}

void ResultSet_mysql5::bindResult(const String &column, double *value)
{
	return bindResult(getColumnIdDirect(column), value);
}

void ResultSet_mysql5::bindResult(unsigned column, String *value)
{
	MYSQL_BIND *rb = allocResBinding(column);
	rb->buffer_type = MYSQL_TYPE_NULL;
	addResConversion(&resConv_String, column, 0, value);
}

void ResultSet_mysql5::bindResult(const String &column, String *value)
{
	return bindResult(getColumnIdDirect(column), value);
}

void ResultSet_mysql5::bindResult(unsigned column, Blob *value)
{
	MYSQL_BIND *rb = allocResBinding(column);
	rb->buffer_type = MYSQL_TYPE_NULL;
	addResConversion(&resConv_Blob, column, 0, value);
}

void ResultSet_mysql5::bindResult(const String &column, Blob *value)
{
	return bindResult(getColumnIdDirect(column), value);
}

void ResultSet_mysql5::bindResult(unsigned column, Date *value)
{
	MYSQL_BIND *rb = allocResBinding(column);
	rb->buffer_type = MYSQL_TYPE_LONGLONG;
	rb->buffer = value;
}

void ResultSet_mysql5::bindResult(const String &column, Date *value)
{
	return bindResult(getColumnIdDirect(column), value);
}

void ResultSet_mysql5::bindResult(unsigned column, Variant **value)
{
	MYSQL_BIND *rb = allocResBinding(column);
	rb->buffer_type = MYSQL_TYPE_NULL;
	addResConversion(&resConv_Variant, column, 0, value);
}

void ResultSet_mysql5::bindResult(const String &column, Variant **value)
{
	return bindResult(getColumnIdDirect(column), value);
}

void ResultSet_mysql5::bindNullHandler(unsigned column, void (*handler)(void *value))
{
	if (res_bindings[column].buffer == NULL) {
		xthrownew(SqlExcept(-1, "null handler allowed only on basic scalar types"));
	}
	addResConversion(&resConv_nullHandler, column, 0, (void *)handler);
}

void ResultSet_mysql5::bindNullHandler(const String &column, void (*handler)(void *value))
{
	return bindNullHandler(getColumnIdDirect(column), handler);
}

bool ResultSet_mysql5::fetchRow()
{
	if (!res_binding_done) {
		allocResBinding(mysql_stmt_field_count(stmt)-1);
		for (size_t i = 0; i < res_bindings.count(); i++) {
			if (res_bindings[i].is_null == NULL)
				res_bindings[i].is_null = &res_bindings[i].is_null_value;
		}
		if (mysql_stmt_bind_result(stmt, &res_bindings[0]) != 0) {
			SqlConnection_mysql5::throwSqlExcept(mysql_stmt_sqlstate(stmt), mysql_stmt_error(stmt));
			return false;
		}
		res_binding_done = true;
	}
	switch (mysql_stmt_fetch(stmt)) {
	case 0:
#ifdef MYSQL_DATA_TRUNCATED
	case MYSQL_DATA_TRUNCATED:
#endif
		for (size_t i = res_bindings.count(); i-- > 0; ) {
			if (*res_bindings[i].is_null) {
				switch (res_bindings[i].buffer_type) {
				case MYSQL_TYPE_TINY:
					*(Sint8 *)res_bindings[i].buffer = 0;
					break;
				case MYSQL_TYPE_SHORT:
					*(Sint16 *)res_bindings[i].buffer = 0;
					break;
				case MYSQL_TYPE_LONG:
					*(Sint32 *)res_bindings[i].buffer = 0;
					break;
				case MYSQL_TYPE_LONGLONG:
					*(Sint64 *)res_bindings[i].buffer = 0;
					break;
				case MYSQL_TYPE_DOUBLE:
					*(double *)res_bindings[i].buffer = 0;
					break;
				default:
					break;
				}
			}
		}
		for (conversion *c = res_conversions; c; c = c->next) {
			c->convertor(this, c);
		}
		return true;

	case MYSQL_NO_DATA:
		return false;

	default:
		SqlConnection_mysql5::throwSqlExcept(mysql_stmt_sqlstate(stmt), mysql_stmt_error(stmt));
		return false;
	}
}

void ResultSet_mysql5::store()
{
	if (stored)
		return;
	stored = true;
	if (mysql_stmt_store_result(stmt) != 0) {
		SqlConnection_mysql5::throwSqlExcept(mysql_stmt_sqlstate(stmt), mysql_stmt_error(stmt));
	}
}

bool ResultSet_mysql5::isNull(unsigned column)
{
	return *res_bindings[column].is_null;
}

bool ResultSet_mysql5::isNull(const String &column)
{
	return isNull(getColumnIdDirect(column));
}

Sint64 ResultSet_mysql5::getInt(unsigned column)
{
	int err;
	my_bool null;
	Sint64 value = 0;
	MYSQL_BIND b;
	memset(&b, 0, sizeof(b));
	b.buffer_type = MYSQL_TYPE_LONGLONG;
	b.buffer = &value;
	b.is_null = &null;
	switch ((err = mysql_stmt_fetch_column(stmt, &b, column, 0))) {
	case 0:
		return value;
	default:
		SqlConnection_mysql5::throwSqlExcept(mysql_stmt_sqlstate(stmt), mysql_stmt_error(stmt));
		return 0;
	}
}

Sint64 ResultSet_mysql5::getInt(const String &column)
{
	return getInt(getColumnIdDirect(column));
}

double ResultSet_mysql5::getDouble(unsigned column)
{
	int err;
	my_bool null;
	double value = 0;
	MYSQL_BIND b;
	memset(&b, 0, sizeof(b));
	b.buffer_type = MYSQL_TYPE_DOUBLE;
	b.buffer = &value;
	b.is_null = &null;
	switch ((err = mysql_stmt_fetch_column(stmt, &b, column, 0))) {
	case 0:
		return value;
	default:
		SqlConnection_mysql5::throwSqlExcept(mysql_stmt_sqlstate(stmt), mysql_stmt_error(stmt));
		return 0;
	}
}

double ResultSet_mysql5::getDouble(const String &column)
{
	return getDouble(getColumnIdDirect(column));
}

Blob ResultSet_mysql5::getBlob(unsigned column)
{
	if (*res_bindings[column].is_null)
		return Null();
	unsigned long length;
	Blob value;
	MYSQL_BIND b;
	memset(&b, 0, sizeof(b));
	b.buffer_type = MYSQL_TYPE_STRING;
	b.buffer_length = 32;
	b.length = &length;
retry:
	b.buffer = value.lock(b.buffer_length);
	int ret;
	switch ((ret = mysql_stmt_fetch_column(stmt, &b, column, 0))) {
	case 0:
#if (!defined MYSQL_DATA_TRUNCATED) || 1
		if (b.buffer_length < length) {
			b.buffer_length = length;
			goto retry;
		}
#endif
		break;

#ifdef MYSQL_DATA_TRUNCATED
	case MYSQL_DATA_TRUNCATED:
		b.buffer_length = length;
		goto retry;
#endif

	default:
		value.unlock(length);
		SqlConnection_mysql5::throwSqlExcept(mysql_stmt_sqlstate(stmt), mysql_stmt_error(stmt));
	}
	value.unlock(length);
	return value;
}

Blob ResultSet_mysql5::getBlob(const String &column)
{
	return getBlob(getColumnIdDirect(column));
}

String ResultSet_mysql5::getString(unsigned column)
{
	if (*res_bindings[column].is_null)
		return Null();
	unsigned long length;
	String value;
	MYSQL_BIND b;
	memset(&b, 0, sizeof(b));
	b.buffer_type = MYSQL_TYPE_BLOB;
	b.buffer_length = 32;
	b.length = &length;
retry:
	b.buffer = value.lockAscii(b.buffer_length);
	int ret;
	switch (ret = mysql_stmt_fetch_column(stmt, &b, column, 0)) {
	case 0:
#if (!defined MYSQL_DATA_TRUNCATED) || 1
		if (b.buffer_length < length) {
			b.buffer_length = length;
			goto retry;
		}
#endif
		break;

#ifdef MYSQL_DATA_TRUNCATED
	case MYSQL_DATA_TRUNCATED:
		b.buffer_length = length;
		goto retry;
#endif

	default:
		value.unlock(length);
		SqlConnection_mysql5::throwSqlExcept(mysql_stmt_sqlstate(stmt), mysql_stmt_error(stmt));
	}
	value.unlock(length);
	return value;
}

String ResultSet_mysql5::getString(const String &column)
{
	return getString(getColumnIdDirect(column));
}

Date ResultSet_mysql5::getDate(unsigned column)
{
	unsigned long length;
	Date native_value;
	MYSQL_TIME value;
	MYSQL_BIND b;
	memset(&b, 0, sizeof(b));
	b.buffer_type = MYSQL_TYPE_DATE;
	b.buffer_length = sizeof(value);
	b.length = &length;
	switch (mysql_stmt_fetch_column(stmt, &b, column, 0)) {
	case 0:
		/* TODO: convert to date */
		xthrownew(UnsupportedExcept(this, comp_name, "Date", ""));
		break;

	default:
		SqlConnection_mysql5::throwSqlExcept(mysql_stmt_sqlstate(stmt), mysql_stmt_error(stmt));
	}
	return native_value;
}

Date ResultSet_mysql5::getDate(const String &column)
{
	return getDate(getColumnIdDirect(column));
}

Variant *ResultSet_mysql5::getVariant(unsigned column)
{
	if (*res_bindings[column].is_null) {
		return new Variant(Null());
	}
	switch (stmt->fields[column].type) {
	case MYSQL_TYPE_NULL:
		return new Variant(Null());

	case MYSQL_TYPE_BIT:
		return new Variant((bool)getInt(column));

	case MYSQL_TYPE_TINY:
	case MYSQL_TYPE_SHORT:
	case MYSQL_TYPE_INT24:
	case MYSQL_TYPE_LONG:
	case MYSQL_TYPE_LONGLONG:
		return new Variant(getInt(column));

	case MYSQL_TYPE_DECIMAL:
	case MYSQL_TYPE_NEWDECIMAL:
	case MYSQL_TYPE_FLOAT:
	case MYSQL_TYPE_DOUBLE:
		return new Variant(getDouble(column));

	case MYSQL_TYPE_VARCHAR:
	case MYSQL_TYPE_VAR_STRING:
	case MYSQL_TYPE_STRING:
	case MYSQL_TYPE_ENUM:
	case MYSQL_TYPE_SET:
		return new Variant(getString(column));

	case MYSQL_TYPE_TINY_BLOB:
	case MYSQL_TYPE_MEDIUM_BLOB:
	case MYSQL_TYPE_LONG_BLOB:
	case MYSQL_TYPE_BLOB:
		return new Variant(getBlob(column));

	case MYSQL_TYPE_TIMESTAMP:
	case MYSQL_TYPE_DATE:
	case MYSQL_TYPE_TIME:
	case MYSQL_TYPE_DATETIME:
	case MYSQL_TYPE_YEAR:
	case MYSQL_TYPE_NEWDATE:
	case MYSQL_TYPE_GEOMETRY:
	default:
		return new Variant(getString(column));
	}
}

Variant *ResultSet_mysql5::getVariant(const String &column)
{
	return getVariant(getColumnIdDirect(column));
}


DRSQL_MYSQL5_NS_END
