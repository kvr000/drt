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
#include <dr/MemExcept.hxx>

DR_NS_BEGIN


DR_THROW_DEF(DR_NS_STR, MemExcept, Except);

DR_THROW_IMPL_SIMPLE(MemExcept);

MemExcept *createMemExceptInstance();

static MemExcept *MemExcept_instance = createMemExceptInstance();

MemExcept *createMemExceptInstance()
{
	return new MemExcept(0);
}

DR_EXPORT_MET MemExcept::MemExcept(size_t size)
{
}

DR_EXPORT_MET String MemExcept::stringify() const
{
	return "memory allocation failed";
}

DR_EXPORT_MTS void MemExcept::throwInstance(size_t size)
{
	DR_THROWREF(MemExcept_instance);
}


DR_NS_END
