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

#ifndef dr__sql__SqlStatementDummy__hxx__
# define dr__sql__SqlStatementDummy__hxx__

#include <dr/sql/def.hxx>

#include <dr/sql/Date.hxx>
#include <dr/sql/SqlStatement.hxx>

DR_SQL_NS_BEGIN

DR_NS_USE

class Date;
class ResultSet;


class SqlStatementDummy: public SqlStatement
{
	DR_OBJECT_DECL_SIMPLE(SqlStatementDummy, SqlStatement);

protected:
	virtual				~SqlStatementDummy();

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
	virtual Uint64			getAffectedRows();
	virtual Uint64			getInsertId();
};


DR_SQL_NS_END

#endif
