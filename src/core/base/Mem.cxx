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

#include <limits.h>
#include <string.h>

#include <dr/Assert.hxx>
#include <dr/sex.hxx>
#include <dr/Mem.hxx>

DR_NS_BEGIN


DR_EXPORT_FUN void Mem::xchg_zero(void *, void *, size_t len)
{
	DR_Assert(len == 0);
}

DR_EXPORT_FUN void Mem::xchg_bchar(void *d1, void *d2, size_t len)
{
	do {
		char x = *(char *)d1;
		*(char *)d1 = *(char *)d2;
		*(char *)d2 = x;
		d1 = (char *)d1+1; d2 = (char *)d2+1;
	} while (--len != 0);
}

DR_EXPORT_FUN void Mem::xchg_int(void *d1, void *d2, size_t len)
{
	DR_Assert(len == sizeof(int));
	int x = *(int *)d1;
	*(int *)d1 = *(int *)d2;
	*(int *)d2 = x;
}

DR_EXPORT_FUN void Mem::xchg_long(void *d1, void *d2, size_t len)
{
	DR_Assert(len == sizeof(long));
	long x = *(long *)d1;
	*(long *)d1 = *(long *)d2;
	*(long *)d2 = x;
}

DR_EXPORT_FUN void Mem::xchg_bint(void *d1, void *d2, size_t len)
{
	do {
		int x = *(int *)d1;
		*(int *)d1 = *(int *)d2;
		*(int *)d2 = x;
		d1 = (int *)d1+1; d2 = (int *)d2+1;
	} while ((len -= sizeof(int)) != 0);
}

DR_EXPORT_FUN void Mem::xchg_blong(void *d1, void *d2, size_t len)
{
	do {
		long x = *(long *)d1;
		*(long *)d1 = *(long *)d2;
		*(long *)d2 = x;
		d1 = (long *)d1+1;; d2 = (long *)d2+1;
	} while ((len -= sizeof(long)) != 0);
}

DR_EXPORT_FUN void (*Mem::getXchgFunc(size_t tsize))(void *, void *, size_t)
{
	switch (tsize) {
	case 0:
		return &xchg_zero;
	case 1:
	case 3:
	case 5:
	case 7:
		return &xchg_bchar;
#if ULONG_MAX != UINT_MAX
	case sizeof(int):
		return &xchg_int;
#endif
	case sizeof(long):
		return xchg_long;
	case 2*sizeof(long):
		return xchg_blong;
	default:
		switch (tsize%sizeof(long)) {
		case 0:
			return xchg_blong;
		case sizeof(int):
			return xchg_bint;
		default:
			return xchg_bchar;
		}
	}
}

DR_EXPORT_FUN void Mem::xchg(void *d1, void *d2, size_t len)
{
	switch (len) {
	case 0:
		return;
	case 1:
	case 3:
	case 5:
	case 7:
		xchg_bchar(d1, d2, len);
		break;
#if ULONG_MAX != UINT_MAX
	case sizeof(int):
		xchg_int(d1, d2, len);
		break;
#endif
	case sizeof(long):
		xchg_long(d1, d2, len);
		break;
	case 2*sizeof(long):
		xchg_blong(d1, d2, len);
		break;
	default:
		switch (len%sizeof(long)) {
		case 0:
			xchg_blong(d1, d2, len);
			break;
#if ULONG_MAX != UINT_MAX
		case sizeof(int):
			xchg_bint(d1, d2, len);
			break;
#endif
		default:
			xchg_bchar(d1, d2, len);
			break;
		}
	}
}

DR_EXPORT_FUN void Mem::copyLe(void *dest, const void *src, size_t len)
{
#if DR_SEX == DR_SEX_LE
	memcpy(dest, src, len);
#else
	for (; len-- > 0; dest = (char *)dest+1) {
		*(char *)dest = ((const char *)src)[len];
	}
#endif
}

DR_EXPORT_FUN void Mem::copyBe(void *dest, const void *src, size_t len)
{
#if DR_SEX == DR_SEX_BE
	memcpy(dest, src, len);
#else
	for (; len-- > 0; dest = (char *)dest+1) {
		*(char *)dest = ((const char *)src)[len];
	}
#endif
}

DR_EXPORT_FUN void Mem::copyPartialLe(void *dest, size_t dst_size, const void *src, size_t src_size)
{
#if DR_SEX == DR_SEX_LE
	memcpy(dest, src, src_size);
#else
	for (dest = (char *)dest+dst_size-src_size; src_size-- > 0; dest = (char *)dest-1) {
		*(char *)dest = ((const char *)src)[len];
	}
#endif
}

DR_EXPORT_FUN void Mem::copyPartialBe(void *dest, size_t dst_size, const void *src, size_t src_size)
{
#if DR_SEX == DR_SEX_BE
	memcpy(dest+dst_size-src_size, src, src_size);
#else
	for (; src_size-- > 0; dest = (char *)dest-1) {
		*(char *)dest = ((const char *)src)[src_size];
	}
#endif
}


DR_NS_END
