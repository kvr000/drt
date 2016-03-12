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

#ifndef dr__types__hxx__
# define dr__types__hxx__

#include <stddef.h>

#include <new>

#include <dr/def_core.hxx>
#include <dr/arch/types.arch.hxx>

#if defined DR_OS_WNT
typedef DR_INTPTR			ssize_t;
#elif defined DR_OS_SYMBIAN
typedef DR_INTPTR			ssize_t;
# include <sys/types.h>
#else
#include <sys/types.h>
#endif

DR_NS_BEGIN

#define DR_CAST(iface, obj) ((iface *)((obj)->getIface(obj::comp_name)))
#define DR_CASTUR(iface, obj) ((iface *)((obj)->getIfaceUnref(obj::comp_name)))


/* fixed sized ints */
typedef signed DR_INT8			Sint8;
typedef unsigned DR_INT8		Uint8;

typedef signed DR_INT16			Sint16;
typedef unsigned DR_INT16		Uint16;

typedef signed DR_INT32			Sint32;
typedef unsigned DR_INT32		Uint32;

typedef signed DR_INT64			Sint64;
typedef unsigned DR_INT64		Uint64;

typedef signed DR_INTPTR		SintPtr;
typedef unsigned DR_INTPTR		UintPtr;


/* reference counting */
typedef volatile SintPtr		Refcnt;


/* alignment */
typedef DR_ALIGN_TYPE			Aligned;

#define DR_ALIGN_BITS			(DR_ALIGN*8)


template <typename T>
class typeref
{
public:
        typedef T			orig;					/* original type */
        typedef T			typ;					/* typ itself (ie without &) */
	typedef T			nvtyp;					/* non-void type */
	typedef T			nvret;					/* non-void ret-type */
	typedef T			nvsimp;					/* non-void simple (ie int for void) */
        typedef T &			ref;					/* reference */
	typedef const T &		cref;					/* constant reference */
	typedef const T &		aref;					/* argument best-way (reference or type) */
        typedef T *			ptr;					/* pointer */

	static const bool isref		= false;				/* type is reference-type */
	static const bool isvoid	= false;				/* type is void */
};

template <typename T>
class typeref<T &>
{
public:
        typedef T &			orig;
        typedef T 			typ;
	typedef T 			nvtyp;
	typedef T 			nvret;
	typedef T 			nvsimp;
        typedef T &			ref;
	typedef T &			aref;
	typedef const T &		cref;
        typedef T *			ptr;

	static const bool isref		= true;
	static const bool isvoid	= false;
};

template <>
class typeref<void>
{
	class vref {};
public:
	typedef void			orig;
	typedef void			typ;
	typedef vref 			nvtyp;
	typedef vref &			nvret;
	typedef int 			nvsimp;
	typedef vref &			ref;
	typedef const vref &		cref;
	typedef void *			ptr;

	static const bool isref		= false;
	static const bool isvoid	= true;
};


template <typename T, size_t s = sizeof(T)>
struct typesize_h
{
	struct p0 { char c; T t; };
	static const size_t		size = sizeof(T);
#ifdef DR_BAD_OFFSETOF_ALIGN
	static const size_t		align = (DR_BAD_OFFSETOF_ALIGN >= 8 && size >= 8) ? 8 : ((DR_BAD_OFFSETOF_ALIGN >= 4 && size >= 4) ? 4 : ((DR_BAD_OFFSETOF_ALIGN >= 2 && size >= 2) ? 2 : 1));
#endif
};

template <typename T>
struct typesize_h<T, 1>
{
	struct p0: T { Aligned a; };
	static const size_t		size = (sizeof(p0) == sizeof(Aligned)) ? 0 : 1;
	static const size_t		align = 1;
};

template <> /* specialization for char, hopefully the only existing simple one-byte type */
struct typesize_h<char, 1>
{
	static const size_t		size = 1;
	static const size_t		align = 1;
};

template <typename T>
struct typesize
{
	static const size_t		size = typesize_h<T, sizeof(T)>::size;
	static const size_t		align = typesize_h<T, sizeof(T)>::align;
};


template <typename T>
struct typetrait: public typeref<T>, typesize_h<T>
{
};


template <typename T, int id, size_t size = typesize<T>::size>
struct typestore
{
	T v;
	DR_MINLINE T &			d()					{ return v; }
	DR_MINLINE const T &		d() const				{ return v; }
	DR_MINLINE			typestore()				{ }
	DR_MINLINE			typestore(const T &s)			{ new(&d()) T(s); }
};

template <typename T, int id>
struct typestore<T, id, 0>
{
	T &				d()					{ return *reinterpret_cast<T *>(this); }
	const T &			d() const				{ return *reinterpret_cast<const T *>(this); }
	DR_MINLINE			typestore()				{ }
	DR_MINLINE			typestore(const T &s)			{ new(&d()) T(s); }
};

template <typename T>
struct typefixed
{
};

template <>
struct typefixed<signed char>
{
	typedef signed DR_BASE_CHAR	ctype;
	typedef signed DR_BASE_CHAR	stype;
	typedef unsigned DR_BASE_CHAR	utype;
};

template <>
struct typefixed<unsigned char>
{
	typedef unsigned DR_BASE_CHAR	ctype;
	typedef signed DR_BASE_CHAR	stype;
	typedef unsigned DR_BASE_CHAR	utype;
};

template <>
struct typefixed<short>
{
	typedef DR_BASE_SHORT		ctype;
	typedef signed DR_BASE_SHORT	stype;
	typedef unsigned DR_BASE_SHORT	utype;
};

template <>
struct typefixed<unsigned short>
{
	typedef unsigned DR_BASE_SHORT	ctype;
	typedef signed DR_BASE_SHORT	stype;
	typedef unsigned DR_BASE_SHORT	utype;
};

template <>
struct typefixed<int>
{
	typedef DR_BASE_INT		ctype;
	typedef signed DR_BASE_INT	stype;
	typedef unsigned DR_BASE_INT	utype;
};

template <>
struct typefixed<unsigned int>
{
	typedef unsigned DR_BASE_INT	ctype;
	typedef signed DR_BASE_INT	stype;
	typedef unsigned DR_BASE_INT	utype;
};

template <>
struct typefixed<long>
{
	typedef DR_BASE_LONG		ctype;
	typedef signed DR_BASE_LONG	stype;
	typedef unsigned DR_BASE_LONG	utype;
};

template <>
struct typefixed<unsigned long>
{
	typedef unsigned DR_BASE_LONG	ctype;
	typedef signed DR_BASE_LONG	stype;
	typedef unsigned DR_BASE_LONG	utype;
};

#if DR_SIZE_BIGINT != DR_SIZE_LONG
template <>
struct typefixed<DR_TYPE_BIGINT>
{
	typedef DR_BASE_BIGINT		ctype;
	typedef signed DR_BASE_BIGINT	stype;
	typedef unsigned DR_BASE_BIGINT	utype;
};

template <>
struct typefixed<unsigned DR_TYPE_BIGINT>
{
	typedef unsigned DR_BASE_BIGINT	ctype;
	typedef signed DR_BASE_BIGINT	stype;
	typedef unsigned DR_BASE_BIGINT	utype;
};
#endif


/**
 * init object to NULL
 */
class DR_PUB Null
{
public:
	DR_MINLINE			Null()					{}
};


/**
 * don't init at all
 */
class DR_PUB None
{
public:
	DR_MINLINE			None()					{}
};


/**
 * auto reference
 */
class DR_PUB Doref
{
public:
	DR_MINLINE			Doref()					{}
};


/**
 * don't reference
 */
class DR_PUB Noref
{
public:
	DR_MINLINE			Noref()					{}
};


/**
 * init with thread safe
 */
class DR_PUB Tsafe
{
public:
	DR_MINLINE			Tsafe()					{}
};


/**
 * invalid, undefined class
 */
class Invalid;


DR_NS_END

#include <dr/Alloc.hxx>

DR_NS_BEGIN


/**
 * Base object
 *
 * Overrides the basic functionality such as new and delete operators
 */
class DR_PUB Base
{
public:
	DR_MINLINE static void *	operator new(size_t s)			{ return Alloc::alloc(s); }
	DR_MINLINE static void		operator delete(void *m)		{ Alloc::free(m); }
	DR_MINLINE static void *	operator new(size_t s, void *p)		{ return p; }
	DR_MINLINE static void		operator delete(void *m, void *p)	{}
};


DR_NS_END

#endif
