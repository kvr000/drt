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

#include <dr/Const.hxx>

#include <dr/sql/SqlExcept.hxx>

DR_SQL_NS_BEGIN

DR_THROW_DEF(DR_SQL_NS_STR, SqlExcept, Except);
DR_THROW_IMPL_SIMPLE(SqlExcept);


DR_EXPORT_MET SqlExcept::SqlExcept(int error_, const String &error_desc_):
	error(error_),
	error_desc(error_desc_)
{
}

DR_EXPORT_MET SqlExcept::~SqlExcept()
{
}

DR_EXPORT_MET int SqlExcept::getSqlError() const
{
	return error;
}

DR_EXPORT_MET String SqlExcept::getSqlDescription() const
{
	return error_desc;
}

DR_EXPORT_MET String SqlExcept::stringify() const
{
	String desc("sql exception (");
	desc.appendNumber(error).append("): ");
	desc.append(error_desc);
	return desc;
}


DR_SQL_NS_END
