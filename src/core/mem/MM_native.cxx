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

#include <stdlib.h>

#include <dr/MM.hxx>
#include <dr/MemException.hxx>
#include <dr/dev/MM_impl.hxx>
#include <dr/dev/MM_native.hxx>

DR_NS_BEGIN


void MM_native::setup(const char *opts, int set_level)
{
	MM_impl::setImpl(&impl_alloc, (void (*)(void *m, size_t size))&impl_free, &impl_alloc, &impl_free, &impl_realloc, &impl_realloc);

	Fatal::s_init();

	MM_impl::debug_options = set_level;
}

void *MM_native::impl_alloc(size_t size)
{
	if (void *mem = ::malloc(size))
		return mem;
	MemException::throwInstance(size);
	return NULL;
}

void MM_native::impl_free(void *m)
{
	::free(m);
}

void *MM_native::impl_realloc(void *m, size_t newsize)
{
	if (void *mem = ::realloc(m, newsize))
		return mem;
	MemException::throwInstance(newsize);
	return NULL;
}


DR_NS_END
