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

#include <dr/sql/SqlParseExcept.hxx>

DR_SQL_NS_BEGIN

DR_THROW_DEF(DR_SQL_NS_STR, SqlParseExcept, SqlExcept);
DR_THROW_IMPL_SIMPLE(SqlParseExcept);


DR_EXPORT_MET SqlParseExcept::SqlParseExcept(const String &command_, int error_, const String &error_desc_):
	SqlExcept(error_, error_desc_),
	command(command_)
{
}

DR_EXPORT_MET SqlParseExcept::~SqlParseExcept()
{
}

DR_EXPORT_MET String SqlParseExcept::stringify() const
{
	String desc("sql parse exception (");
	desc.appendNumber(error).append("): ");
	desc.append(command);
	desc.append(" :").append(error_desc);
	return desc;
}

DR_EXPORT_MET String SqlParseExcept::getCommand() const
{
	return command;
}


DR_SQL_NS_END
