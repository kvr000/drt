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

#ifndef dr__sql__SqlExcept__hxx__
# define dr__sql__SqlExcept__hxx__

#include <dr/sql/def.hxx>

#include <dr/Except.hxx>

DR_SQL_NS_BEGIN

DR_NS_USE


class SqlExcept: public Except
{
	DR_THROW_DECL_SIMPLE(SqlExcept, Except);
public:
	DR_CONSTRUCT			SqlExcept(int error, const String &error_desc);

protected:
	virtual				~SqlExcept();

public:
	int				getSqlError() const;
	String				getSqlDescription() const;

	String				stringify() const;

protected:
	int				error;
	String				error_desc;
};


DR_SQL_NS_END

#endif
