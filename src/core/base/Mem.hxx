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

#ifndef dr__mem__hxx__
# define dr__mem__hxx__

#include <stdlib.h>

#include <dr/def_core.hxx>

DR_NS_BEGIN


/**
 * Memory support routines
 *
 * Library containing memory operations
 */
class DR_PUB Mem
{
public:
	static void			xchg_zero(void *d1, void *d2, size_t len);
	static void			xchg_bchar(void *d1, void *d2, size_t len);
	static void			xchg_int(void *d1, void *d2, size_t len);
	static void			xchg_long(void *d1, void *d2, size_t len);
	static void			xchg_bint(void *d1, void *d2, size_t len);
	static void			xchg_blong(void *d1, void *d2, size_t len);

	static void			xchg(void *d1, void *d2, size_t len);

	static void			(*getXchgFunc(size_t tsize))(void *, void *, size_t);

	static void			copyLe(void *dest, const void *source, size_t size);
	static void			copyBe(void *dest, const void *source, size_t size);
	static void			copyPartialLe(void *dest, size_t dst_size, const void *source, size_t src_size);
	static void			copyPartialBe(void *dest, size_t dst_size, const void *source, size_t src_size);
};


DR_NS_END

#endif
