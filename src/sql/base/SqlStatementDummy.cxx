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
#include <dr/Except.hxx>
#include <dr/UnsupportedExcept.hxx>
#include <dr/sql/SqlExcept.hxx>

#include <dr/sql/SqlStatementDummy.hxx>

#include <mysql/mysql.h>

DR_SQL_NS_BEGIN

DR_OBJECT_DEF(DR_SQL_NS_STR, SqlStatementDummy, SqlStatement);
DR_OBJECT_IMPL_SIMPLE(SqlStatementDummy);


SqlStatementDummy::~SqlStatementDummy()
{
}

void SqlStatementDummy::setLimit(long limit_)
{
}

void SqlStatementDummy::setOffset(long offset_)
{
}

void SqlStatementDummy::setOffsetLimit(long offset_, long limit_)
{
}

void SqlStatementDummy::prepare()
{
}

void SqlStatementDummy::bindParamNull(unsigned column)
{
}

void SqlStatementDummy::bindParam(unsigned column, Sint8 value)
{
}

void SqlStatementDummy::bindParam(unsigned column, Sint16 value)
{
}

void SqlStatementDummy::bindParam(unsigned column, Sint32 value)
{
}

void SqlStatementDummy::bindParam(unsigned column, Sint64 value)
{
}

void SqlStatementDummy::bindParam(unsigned column, const String &value)
{
}

void SqlStatementDummy::bindParam(unsigned column, const Date &value)
{
}

void SqlStatementDummy::bindParam(unsigned column, const Blob &value)
{
}

void SqlStatementDummy::executeUpdate()
{
}

ResultSet *SqlStatementDummy::executeQuery()
{
	xthrownew(UnsupportedExcept(this, "sql", "executeQuery", ""));
	return NULL;
}

Uint64 SqlStatementDummy::getAffectedRows()
{
	return 0;
}

Uint64 SqlStatementDummy::getInsertId()
{
	xthrownew(UnsupportedExcept(this, "sql", "getInsertId", ""));
	return 0;
}


DR_SQL_NS_END
