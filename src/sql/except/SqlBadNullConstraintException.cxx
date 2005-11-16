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

/*drt
 * include:	dr/sql/SqlConstraintException.hxx
 *
 * ns:		dr::sql
 */

#include <dr/sql/SqlBadNullConstraintException.hxx>
#include "_gen/SqlBadNullConstraintException-all.hxx"

DR_SQL_NS_BEGIN


/*drt
 * class:	SqlBadNullConstraintException
 * type:	exception
 * ancestor:	SqlConstraintException
 */

DR_MET(public)
SqlBadNullConstraintException::SqlBadNullConstraintException(int error_, const String &error_desc_):
	SqlConstraintException(error_, error_desc_)
{
}

DR_MET(protected virtual)
SqlBadNullConstraintException::~SqlBadNullConstraintException()
{
}

DR_MET(public virtual)
String SqlBadNullConstraintException::stringify() const
{
	String desc("sql bad null constraint exception (");
	desc.appendNumber(error).append("): ");
	desc.append(error_desc);
	return desc;
}


DR_SQL_NS_END
