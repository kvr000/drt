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

#ifndef dr__sql__mysql5__SqlStatement_mysql5__hxx__
# define dr__sql__mysql5__SqlStatement_mysql5__hxx__

#include <dr/Array.hxx>

#include <dr/sql/mysql5/def.hxx>

#include <mysql/mysql.h>

#include <dr/sql/SqlStatement.hxx>

DRSQL_MYSQL5_NS_BEGIN

DR_NS_USE
DR_SQL_NS_USE

class SqlConnection_mysql5;


class SqlStatement_mysql5: public SqlStatement
{
	DR_OBJECT_DECL_SIMPLE(SqlStatement_mysql5, SqlStatement);

public:
	/* constructor */		SqlStatement_mysql5(SqlConnection_mysql5 *conn, MYSQL_STMT *stmt, String stmt_str);
	virtual				~SqlStatement_mysql5();

public:
	virtual void			setLimit(long limit);
	virtual void			setOffset(long offset);
	virtual void			setOffsetLimit(long offset, long limit);

public:
	virtual void			prepare();

public:
	virtual void			bindParamNull(unsigned param_id);
	virtual void			bindParam(unsigned param_id, Sint8 value);
	virtual void			bindParam(unsigned param_id, Sint16 value);
	virtual void			bindParam(unsigned param_id, Sint32 value);
	virtual void			bindParam(unsigned param_id, Sint64 value);
	virtual void			bindParam(unsigned param_id, double value);
	virtual void			bindParam(unsigned param_id, const String &value);
	virtual void			bindParam(unsigned param_id, const Blob &value);
	virtual void			bindParam(unsigned param_id, const Date &value);
	virtual void			bindParam(unsigned param_id, Variant *value);
	virtual void			bindParams(Variant **values, size_t values_count);

public:
	virtual ResultSet *		executeQuery();
	virtual void			executeUpdate();

public:
	virtual Uint64			getAffectedRows();
	virtual Uint64			getInsertId();

protected:
	void				recreateStmt();
	MYSQL_BIND *			allocParBinding(unsigned idx);

protected:
	Ref<SqlConnection_mysql5>	conn;
	String				stmt_str;
	long				limit;
	long				offset;
	MYSQL_STMT *			stmt;
	SArray<MYSQL_BIND, ComparInv<MYSQL_BIND> > par_bindings;

	bool				has_limit:1;
	bool				has_offset:1;

	friend class ResultSet_mysql5;
};


DRSQL_MYSQL5_NS_END

#endif
