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

#ifndef dr__sql__mysql5__ResultSet_mysql5__hxx__
# define dr__sql__mysql5__ResultSet_mysql5__hxx__

#include <dr/Array.hxx>

#include <dr/sql/mysql5/def.hxx>

#include <mysql/mysql.h>

#include <dr/sql/mysql5/SqlStatement_mysql5.hxx>
#include <dr/sql/ResultSet.hxx>

DRSQL_MYSQL5_NS_BEGIN

DR_NS_USE
DR_SQL_NS_USE


class ResultSet_mysql5: public ResultSet
{
	DR_OBJECT_DECL_SIMPLE(ResultSet_mysql5, ResultSet);

public:
	/* constructor */		ResultSet_mysql5(SqlStatement_mysql5 *conn);
	virtual				~ResultSet_mysql5();

public:
	virtual bool			isNull(unsigned column);
	virtual bool			isNull(const String &column);
	virtual void			bindResult(unsigned column, Sint8 *value);
	virtual void			bindResult(const String &column, Sint8 *value);
	virtual void			bindResult(unsigned column, Sint16 *value);
	virtual void			bindResult(const String &column, Sint16 *value);
	virtual void			bindResult(unsigned column, Sint32 *value);
	virtual void			bindResult(const String &column, Sint32 *value);
	virtual void			bindResult(unsigned column, Sint64 *value);
	virtual void			bindResult(const String &column, Sint64 *value);
	virtual void			bindResult(unsigned column, double *value);
	virtual void			bindResult(const String &column, double *value);
	virtual void			bindResult(unsigned column, String *value);
	virtual void			bindResult(const String &column, String *value);
	virtual void			bindResult(unsigned column, Blob *date);
	virtual void			bindResult(const String &column, Blob *date);
	virtual void			bindResult(unsigned column, Date *date);
	virtual void			bindResult(const String &column, Date *date);
	virtual void			bindResult(unsigned column, Variant **value);
	virtual void			bindResult(const String &column, Variant **value);
	virtual void			bindNullHandler(unsigned column, void (*handler)(void *value));
	virtual void			bindNullHandler(const String &column, void (*handler)(void *value));

public:
	virtual bool			fetchRow();
	virtual void			store();

public:
	virtual Sint64			getInt(unsigned column);
	virtual Sint64			getInt(const String &column);
	virtual double			getDouble(unsigned column);
	virtual double			getDouble(const String &column);
	virtual Blob			getBlob(unsigned column);
	virtual Blob			getBlob(const String &column);
	virtual String			getString(unsigned column);
	virtual String			getString(const String &column);
	virtual Date			getDate(unsigned column);
	virtual Date			getDate(const String &column);
	virtual Variant *		getVariant(unsigned column);
	virtual Variant *		getVariant(const String &column);

protected:
	typedef struct conversion
	{
		void 				(*convertor)(ResultSet_mysql5 *this_, conversion *conv);
		int				column;
		void *				user_var;
		conversion *			next;
		Aligned				buffer[0];
	} conversion;

protected:
	void				initColNames();
	inline int			getColumnIdDirect(const String &name)	{ int column_id; if (col_names.isEmpty()) initColNames(); if ((column_id = col_names.findXchg(name)) < 0) DR_THROWNEW(SqlExcept(42703, "invalid column")); return column_id; }
	MYSQL_BIND *			allocResBinding(unsigned idx);

protected:
	conversion *			addResConversion(void (*conv_func)(ResultSet_mysql5 *this_, conversion *conv), int column, size_t bufsize, void *user_var);

protected:
	Ref<SqlStatement_mysql5>	statement;
	MYSQL_STMT *			stmt;
	StringIndex			col_names;
	SArray<MYSQL_BIND, ComparInv<MYSQL_BIND> > res_bindings;

	conversion *			res_conversions;

	bool				res_binding_done:1;
	bool				stored:1;

protected:
	static void			resConv_String(ResultSet_mysql5 *this_, conversion *conv);
	static void			resConv_Blob(ResultSet_mysql5 *this_, conversion *conv);
	static void			resConv_Variant(ResultSet_mysql5 *this_, conversion *conv);
	static void			resConv_Date(ResultSet_mysql5 *this_, conversion *conv);
	static void			resConv_nullHandler(ResultSet_mysql5 *this_, conversion *conv);
};


DRSQL_MYSQL5_NS_END

#endif
