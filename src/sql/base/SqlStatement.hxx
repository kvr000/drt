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

#ifndef dr__sql__SqlStatement__hxx__
# define dr__sql__SqlStatement__hxx__

#include <dr/sql/def.hxx>

#include <dr/Object.hxx>
#include <dr/Blob.hxx>
#include <dr/Variant.hxx>

DR_SQL_NS_BEGIN

DR_NS_USE

class Date;
class ResultSet;


class SqlStatement: public Object
{
	DR_OBJECT_DECL_SIMPLE(SqlStatement, Object);

public:
	virtual void			setLimit(long limit) = 0;
	virtual void			setOffset(long offset) = 0;
	virtual void			setOffsetLimit(long offset, long limit) = 0;

public:
	virtual void			prepare() = 0;

public:
	virtual void			bindParamNull(unsigned param_id) = 0;
	virtual void			bindParam(unsigned param_id, Sint8 value) = 0;
	virtual void			bindParam(unsigned param_id, Sint16 value) = 0;
	virtual void			bindParam(unsigned param_id, Sint32 value) = 0;
	virtual void			bindParam(unsigned param_id, Sint64 value) = 0;
	virtual void			bindParam(unsigned param_id, double value) = 0;
	virtual void			bindParam(unsigned param_id, const String &value) = 0;
	virtual void			bindParam(unsigned param_id, const Blob &value) = 0;
	virtual void			bindParam(unsigned param_id, const Date &value) = 0;
	virtual void			bindParam(unsigned param_id, Variant *value) = 0;
	virtual void			bindParams(Variant **values, size_t values_count) = 0;

public:
	virtual bool			isBusy() = 0;

public:
	virtual ResultSet *		executeQuery() = 0;
	virtual void			executeUpdate() = 0;

public:
	virtual Uint64			getAffectedRows() = 0;
	virtual Uint64			getInsertId() = 0;
};


DR_SQL_NS_END

#endif
