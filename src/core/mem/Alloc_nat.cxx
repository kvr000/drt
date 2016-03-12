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

#include <dr/types.hxx>

DR_NS_BEGIN


void *MM::nat_allocS(size_t size)
{
	return ::malloc(size);
}

void MM::nat_freeS(void *p, size_t size)
{
	::free(p);
}

void *MM::nat_alloc(size_t size)
{
	if (size == 0)
		size = 1;
	return ::malloc(size);
}

void *MM::nat_realloc(void *p, size_t size)
{
	if (size == 0)
		size = 1;
	return ::realloc(p, size);
}

void *MM::nat_regrow(void *p, size_t size)
{
	if (size == 0)
		size = 1;
	return ::realloc(p, size);
}

void MM::nat_free(void *p)
{
	::free(p);
}


DR_NS_END
