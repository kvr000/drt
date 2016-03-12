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

#ifndef dr__char_traits_h__
# define dr__char_traits_h__

#include <ctype.h>
#ifndef DR_OS_SYMBIAN
#include <wctype.h>
#endif
#include <wchar.h>
#include <string.h>

DR_NS_BEGIN


template <typename TC>
struct CharTraits
{
	typedef TC			type;

	static const size_t		size = sizeof(type);

	static const type		terminator = 0;

	static bool			eq(const type &c1, const type &c2);
	static int			bcmp(const type &c1, const type &c2);

	static void			pcopy(type *d, const type *s, size_t l);
	static void			pmove(type *d, const type *s, size_t l);
	static bool			peq(const type *c1, const type *c2, size_t l);
	static int			pbcmp(const type *c1, const type *c2, size_t l);
	static long			phash(const type *s, size_t l);
	static size_t			zlen(const type *s);
	static bool			ctspace(type c);
};

template <>
struct CharTraits<char>
{
	typedef char			type;

	static const size_t		size = sizeof(type);

	static const type		terminator = '\0';

	DR_MINLINE static bool		eq(const type &c1, const type &c2) { return c1 == c2; }
	DR_MINLINE static int		bcmp(const type &c1, const type &c2) { return (c1 == c2)?0:(c1 < c2)?-1:1; }

	DR_MINLINE static void		pcopy(type *d, const type *s, size_t l) { memcpy(d, s, l); }
	DR_MINLINE static void		pmove(type *d, const type *s, size_t l) { memmove(d, s, l); }
	DR_MINLINE static bool		peq(const type *c1, const type *c2, size_t l) { return memcmp(c1, c2, l) == 0; }
	DR_MINLINE static int		pbcmp(const type *c1, const type *c2, size_t l) { return memcmp(c1, c2, l); }

	DR_MINLINE static size_t 	zlen(const type *s) { return strlen(s); }
	DR_MINLINE static bool		ctspace(type c)				{ return isspace(c) != 0; }
};

template <>
struct CharTraits<wchar_t>
{
	typedef wchar_t			type;

	static const size_t		size = sizeof(type);

	static const type		terminator = '\0';

	DR_MINLINE static bool		eq(const type &c1, const type &c2)	{ return c1 == c2; }
	DR_MINLINE static int		bcmp(const type &c1, const type &c2)	{ return (c1 == c2)?0:(c1 < c2)?-1:1; }

	DR_MINLINE static void		pcopy(type *d, const type *s, size_t l)	{ wmemcpy(d, s, l); }
	DR_MINLINE static void		pmove(type *d, const type *s, size_t l)	{ wmemmove(d, s, l); }
	DR_MINLINE static bool		peq(const type *c1, const type *c2, size_t l) { return wmemcmp(c1, c2, l) == 0; }
	DR_MINLINE static int		pbcmp(const type *c1, const type *c2, size_t l) { return wmemcmp(c1, c2, l); }

	DR_MINLINE static size_t	zlen(const type *s)			{ return wcslen(s); }
	DR_MINLINE static bool		ctspace(type c)				{ return iswspace(c) != 0; }
};

template <>
struct CharTraits<void>
{
	typedef char			type;

	static const size_t		size = sizeof(type);

	static const type		terminator = '\0';

	DR_MINLINE static bool		eq(const type &c1, const type &c2)	{ return c1 == c2; }
	DR_MINLINE static int		bcmp(const type &c1, const type &c2)	{ return (c1 == c2)?0:(c1 < c2)?-1:1; }

	DR_MINLINE static void		pcopy(type *d, const type *s, size_t l)	{ memcpy(d, s, l); }
	DR_MINLINE static void		pmove(type *d, const type *s, size_t l)	{ memmove(d, s, l); }
	DR_MINLINE static bool		peq(const type *c1, const type *c2, size_t l) { return memcmp(c1, c2, l) == 0; }
	DR_MINLINE static int		pbcmp(const type *c1, const type *c2, size_t l) { return memcmp(c1, c2, l); }

	DR_MINLINE static size_t	zlen(const type *s)			{ return strlen(s); }
	DR_MINLINE static bool		ctspace(type c)				{ return isspace(c) != 0; }
};


DR_NS_END

#endif
