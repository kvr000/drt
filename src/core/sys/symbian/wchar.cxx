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

#include <wchar.h>

void wmemcpy(wchar_t *dst, const wchar_t *src, size_t length)
{
	while (length--)
		*dst++ = *src++;
}

void wmemmove(wchar_t *dst, const wchar_t *src, size_t length)
{
	if (dst >= src) {
		while (length--)
			*dst++ = *src++;
	}
	else if (dst < src) {
		dst += length;
		src += length;
		while (length--)
			*--dst = *--src;
	}
}

int wmemcmp(const wchar_t *sl, const wchar_t *sr, size_t length)
{
	while (length--) {
		if (*sl == *sr)
			continue;
		return *sl < *sr ? -1 : 1;
	}
	return 0;
}

void wmemset(wchar_t *dst, wchar_t c, size_t length)
{
	while (length--)
		*dst++ = c;
}
