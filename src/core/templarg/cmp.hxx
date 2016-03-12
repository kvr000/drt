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

#ifndef drcmp_h__
# define drcmp_h__

#include <dr/def_core.hxx>
#include <dr/Assert.hxx>

DR_NS_BEGIN


template <typename T>
DR_MINLINE int cmp(const T &t1, const T &t2)
{
	return (t1 == t2)?0:(t1 < t2)?-1:1;
}

template <typename T>
class Compar
{
public:
	static DR_MINLINE bool		teq(const T &t1, const T &t2)		{ return t1 == t2; }
	static DR_MINLINE int		tcmp(const T &t1, const T &t2)		{ return cmp(t1, t2); }
	static DR_MINLINE void		txchg(T *t1, T *t2)			{ T t(*t1); *t1 = *t2; *t2 = t; }
};

template <typename T>
class ComparEq
{
public:
	static DR_MINLINE bool		teq(const T &t1, const T &t2)		{ return t1 == t2; }
	static DR_MINLINE int		tcmp(const T &t1, const T &t2)		{ DR_AssertInvalid(); return 0; }
	static DR_MINLINE void		txchg(T *t1, T *t2)			{ T t(*t1); *t1 = *t2; *t2 = t; }
};

template <typename T>
class ComparInv
{
public:
	static DR_MINLINE bool		teq(const T &t1, const T &t2)		{ DR_AssertInvalid(); return true; }
	static DR_MINLINE int		tcmp(const T &t1, const T &t2)		{ DR_AssertInvalid(); return 0; }
	static DR_MINLINE void		txchg(T *t1, T *t2)			{ T t(*t1); *t1 = *t2; *t2 = t; }
};

template <typename T> class Ref;

template <typename T>
class RCompar
{
public:
	static DR_MINLINE bool		teq(const Ref<T> &t1, const Ref<T> &t2)	{ return t1->eq(t2); }
	static DR_MINLINE int		tcmp(const Ref<T> &t1, const Ref<T> &t2) { return t1->cmp(t2); }
	static DR_MINLINE void		txchg(Ref<T> *t1, Ref<T> *t2)		{ t1->xchg(t2); }
	static DR_MINLINE long		khash(Ref<T> *t1)			{ return (*t1)->hash(); }
};


DR_NS_END

#endif
