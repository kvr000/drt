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

#include <stdarg.h>
#include <e32base.h>

#include <dr/types.hxx>

extern "C" {

ssize_t snprintf(char *buf, size_t length, const char *format, ...)
{
	va_list vl;
	TPtr8 des((TUint8 *)buf, length);
	va_start(vl, format);
	des.FormatList(TPtrC8((const TUint8 *)format), vl);
	va_end(vl);
	return des.Length();
}

ssize_t vsnprintf(char *buf, size_t length, const char *format, va_list vl)
{
	TPtr8 des((TUint8 *)buf, length);
	des.FormatList(TPtrC8((const TUint8 *)format), vl);
	return des.Length();
}

};
