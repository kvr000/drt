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

#ifndef dr__Alloc__hxx__
# define dr__Alloc__hxx__

#include <dr/MM.hxx>

DR_NS_BEGIN


/**
 * Generic allocator
 */
class DR_PUB Alloc
{
public:
	/* sized alloc/free */
	DR_MINLINE static void *	allocS(size_t size)			{ return MM::allocS(size); }
	DR_MINLINE static void		freeS(void *m, size_t size)		{ MM::freeS(m, size); }

	/* constant size allocations */
	DR_MINLINE static void *	allocC(size_t size)			{ return MM::allocS(size); }
	DR_MINLINE static void		freeC(void *m, size_t size)		{ MM::freeS(m, size); }

	/* generic allocations */
	DR_MINLINE static void *	alloc(size_t size)			{ return MM::alloc(size); }
	DR_MINLINE static void *	realloc(void *m, size_t size)		{ return MM::realloc(m, size); }
	DR_MINLINE static void *	regrow(void *m, size_t size)		{ return MM::regrow(m, size); }
	DR_MINLINE static void		free(void *m)				{ MM::free(m); }

public:
	DR_MINLINE bool			operator==(const Alloc &s)		{ return true; }
	DR_MINLINE bool			operator!=(const Alloc &s)		{ return false; }
};

/**
 * Constant allocator (memory can't never be free)
 */
class DR_PUB AllocConst
{
public:
	/* allocations */
	DR_MINLINE static void *	alloc(size_t size)			{ return MM::nat_alloc(size); }

public:
	DR_MINLINE bool			operator==(const Alloc &s)		{ return true; }
	DR_MINLINE bool			operator!=(const Alloc &s)		{ return false; }
};

/**
 * Native allocator
 */
class DR_PUB AllocNat
{
public:
	/* sized alloc/free */
	DR_MINLINE static void *	allocS(size_t size)			{ return MM::nat_allocS(size); }
	DR_MINLINE static void		freeS(void *m, size_t size)		{ MM::nat_freeS(m, size); }

	/* constant size allocations */
	DR_MINLINE static void *	allocC(size_t size)			{ return MM::nat_allocS(size); }
	DR_MINLINE static void		freeC(void *m, size_t size)		{ MM::nat_freeS(m, size); }

	/* generic allocations */
	DR_MINLINE static void *	alloc(size_t size)			{ return MM::nat_alloc(size); }
	DR_MINLINE static void *	realloc(void *m, size_t size)		{ return MM::nat_realloc(m, size); }
	DR_MINLINE static void *	regrow(void *m, size_t size)		{ return MM::nat_regrow(m, size); }
	DR_MINLINE static void		free(void *m)				{ MM::nat_free(m); }

public:
	DR_MINLINE bool			operator==(const AllocNat &s)		{ return true; }
	DR_MINLINE bool			operator!=(const AllocNat &s)		{ return false; }
};

/**
 * Type allocator
 */
template <typename T>
class AllocT
{
public:
	DR_MINLINE static T *		allocT()				{ return new T; }
	DR_MINLINE static T *		allocT(const T &i)			{ return new T(i); }
	DR_MINLINE static void		freeT(T *p)				{ delete p; }

public:
	DR_MINLINE bool			operator==(const AllocT &s)		{ return true; }
	DR_MINLINE bool			operator!=(const AllocT &s)		{ return false; }
};

/**
 * Type and realloc allocator
 */
template <typename T>
class AllocTR: public AllocT<T>, public Alloc
{
public:
	DR_MINLINE bool			operator==(const AllocTR &s)		{ return true; }
	DR_MINLINE bool			operator!=(const AllocTR &s)		{ return false; }
};


DR_NS_END

#endif
