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

#ifndef dr__io__NonblockExcept__hxx__
# define dr__io__NonblockExcept__hxx__

#include <dr/io/IoExcept.hxx>

DR_IO_NS_BEGIN


/**
 * Nonblock exception
 *
 * Caused by temporarily not available space for input/output
 */
class DR_IO_PUB NonblockExcept: public IoExcept
{
	DR_THROW_DECL_SIMPLE(NonblockExcept, IoExcept);

public:
	DR_CONSTRUCT			NonblockExcept(Handle *handle, const String &operation, int posix_error, int minor_error);
	DR_CONSTRUCT			NonblockExcept(Throwable *previous_, Handle *handle, const String &operation, int posix_error, int minor_error);
};


DR_IO_NS_END

#endif
