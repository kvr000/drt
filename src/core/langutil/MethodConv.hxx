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

#ifndef dr__MethodConv__hxx__
# define dr__MethodConv__hxx__

#include <dr/Assert.hxx>

DR_NS_BEGIN


/**
 * Supports conversion between function pointers and method pointers
 *
 * This class may be used only for pointers to simple methods, having the
 * implementation in basic object (which is common for drt library). Multiple
 * object inheritance (not including fully abstract interfaces) and virtual
 * inheritance are not supported.
 *
 * Note that the resulting function pointer still cannot be used for functions
 * calls, as, firstly, the pointer may contain just index to virtual method
 * table, and secondly, the compiler may use different ways to call method.
 */
class MethodConv
{
public:
	template <typename O, typename R>
	static R (*			methodToFptr(R (O::*m)()))(O*)
	{
		union {
			R (*vf)(O*);
			R (O::*vm)();
			struct { void *a; void *b; } dummy;
		} u;
		u.dummy.a = u.dummy.b = NULL;
		u.vm = m;
		DR_Assert(u.dummy.b == NULL);
		return u.vf;
	}

	template <typename O, typename R>
	static R (O::*			funcToMptr(R (*f)(O*)))()
	{
		union {
			R (*vf)(O*);
			R (O::*vm)();
			struct { void *a; void *b; } dummy;
		} u;
		u.dummy.a = u.dummy.b = NULL;
		u.vf = f;
		return u.vm;
	}

	template <typename O, typename R, typename A0>
	static R (*			methodToFptr(R (O::*m)(A0)))(O*, A0)
	{
		union {
			R (*vf)(O*, A0);
			R (O::*vm)(A0);
			struct { void *a; void *b; } dummy;
		} u;
		u.dummy.a = u.dummy.b = NULL;
		u.vm = m;
		DR_Assert(u.dummy.b == NULL);
		return u.vf;
	}

	template <typename O, typename R, typename A0>
	static R (O::*			funcToMptr(R (*f)(O*, A0)))(A0)
	{
		union {
			R (*vf)(O*, A0);
			R (O::*vm)(A0);
			struct { void *a; void *b; } dummy;
		} u;
		u.dummy.a = u.dummy.b = NULL;
		u.vf = f;
		return u.vm;
	}

	template <typename O, typename R, typename A0, typename A1>
	static R (*			methodToFptr(R (O::*m)(A0, A1)))(O*, A0, A1)
	{
		union {
			R (*vf)(O*, A0, A1);
			R (O::*vm)(A0, A1);
			struct { void *a; void *b; } dummy;
		} u;
		u.dummy.a = u.dummy.b = NULL;
		u.vm = m;
		DR_Assert(u.dummy.b == NULL);
		return u.vf;
	}

	template <typename O, typename R, typename A0, typename A1>
	static R (O::*			funcToMptr(R (*f)(O*, A0, A1)))(A0, A1)
	{
		union {
			R (*vf)(O*, A0, A1);
			R (O::*vm)(A0, A1);
			struct { void *a; void *b; } dummy;
		} u;
		u.dummy.a = u.dummy.b = NULL;
		u.vf = f;
		return u.vm;
	}

	template <typename O, typename R, typename A0, typename A1, typename A2>
	static R (*			methodToFptr(R (O::*m)(A0, A1, A2)))(O*, A0, A1, A2)
	{
		union {
			R (*vf)(O*, A0, A1, A2);
			R (O::*vm)(A0, A1, A2);
			struct { void *a; void *b; } dummy;
		} u;
		u.dummy.a = u.dummy.b = NULL;
		u.vm = m;
		DR_Assert(u.dummy.b == NULL);
		return u.vf;
	}

	template <typename O, typename R, typename A0, typename A1, typename A2>
	static R (O::*			funcToMptr(R (*f)(O*, A0, A1, A2)))(A0, A1, A2)
	{
		union {
			R (*vf)(O*, A0, A1, A2);
			R (O::*vm)(A0, A1, A2);
			struct { void *a; void *b; } dummy;
		} u;
		u.dummy.a = u.dummy.b = NULL;
		u.vf = f;
		return u.vm;
	}

	template <typename O, typename R, typename A0, typename A1, typename A2, typename A3>
	static R (*			methodToFptr(R (O::*m)(A0, A1, A2, A3)))(O*, A0, A1, A2, A3)
	{
		union {
			R (*vf)(O*, A0, A1, A2, A3);
			R (O::*vm)(A0, A1, A2, A3);
			struct { void *a; void *b; } dummy;
		} u;
		u.dummy.a = u.dummy.b = NULL;
		u.vm = m;
		DR_Assert(u.dummy.b == NULL);
		return u.vf;
	}

	template <typename O, typename R, typename A0, typename A1, typename A2, typename A3>
	static R (O::*			funcToMptr(R (*f)(O*, A0, A1, A2, A3)))(A0, A1, A2, A3)
	{
		union {
			R (*vf)(O*, A0, A1, A2, A3);
			R (O::*vm)(A0, A1, A2, A3);
			struct { void *a; void *b; } dummy;
		} u;
		u.dummy.a = u.dummy.b = NULL;
		u.vf = f;
		return u.vm;
	}

	template <typename O, typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
	static R (*			methodToFptr(R (O::*m)(A0, A1, A2, A3, A4)))(O*, A0, A1, A2, A3, A4)
	{
		union {
			R (*vf)(O*, A0, A1, A2, A3, A4);
			R (O::*vm)(A0, A1, A2, A3, A4);
			struct { void *a; void *b; } dummy;
		} u;
		u.dummy.a = u.dummy.b = NULL;
		u.vm = m;
		DR_Assert(u.dummy.b == NULL);
		return u.vf;
	}

	template <typename O, typename R, typename A0, typename A1, typename A2, typename A3, typename A4>
	static R (O::*			funcToMptr(R (*f)(O*, A0, A1, A2, A3, A4)))(A0, A1, A2, A3, A4)
	{
		union {
			R (*vf)(O*, A0, A1, A2, A3, A4);
			R (O::*vm)(A0, A1, A2, A3, A4);
			struct { void *a; void *b; } dummy;
		} u;
		u.dummy.a = u.dummy.b = NULL;
		u.vf = f;
		return u.vm;
	}

	template <typename O, typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5>
	static R (*			methodToFptr(R (O::*m)(A0, A1, A2, A3, A4, A5)))(O*, A0, A1, A2, A3, A4, A5)
	{
		union {
			R (*vf)(O*, A0, A1, A2, A3, A4, A5);
			R (O::*vm)(A0, A1, A2, A3, A4, A5);
			struct { void *a; void *b; } dummy;
		} u;
		u.dummy.a = u.dummy.b = NULL;
		u.vm = m;
		DR_Assert(u.dummy.b == NULL);
		return u.vf;
	}

	template <typename O, typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5>
	static R (O::*			funcToMptr(R (*f)(O*, A0, A1, A2, A3, A4, A5)))(A0, A1, A2, A3, A4, A5)
	{
		union {
			R (*vf)(O*, A0, A1, A2, A3, A4, A5);
			R (O::*vm)(A0, A1, A2, A3, A4, A5);
			struct { void *a; void *b; } dummy;
		} u;
		u.dummy.a = u.dummy.b = NULL;
		u.vf = f;
		return u.vm;
	}

	template <typename O, typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	static R (*			methodToFptr(R (O::*m)(A0, A1, A2, A3, A4, A5, A6)))(O*, A0, A1, A2, A3, A4, A5, A6)
	{
		union {
			R (*vf)(O*, A0, A1, A2, A3, A4, A5, A6);
			R (O::*vm)(A0, A1, A2, A3, A4, A5, A6);
			struct { void *a; void *b; } dummy;
		} u;
		u.dummy.a = u.dummy.b = NULL;
		u.vm = m;
		DR_Assert(u.dummy.b == NULL);
		return u.vf;
	}

	template <typename O, typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	static R (O::*			funcToMptr(R (*f)(O*, A0, A1, A2, A3, A4, A5, A6)))(A0, A1, A2, A3, A4, A5, A6)
	{
		union {
			R (*vf)(O*, A0, A1, A2, A3, A4, A5, A6);
			R (O::*vm)(A0, A1, A2, A3, A4, A5, A6);
			struct { void *a; void *b; } dummy;
		} u;
		u.dummy.a = u.dummy.b = NULL;
		u.vf = f;
		return u.vm;
	}

	template <typename O, typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
	static R (*			methodToFptr(R (O::*m)(A0, A1, A2, A3, A4, A5, A6, A7)))(O*, A0, A1, A2, A3, A4, A5, A6, A7)
	{
		union {
			R (*vf)(O*, A0, A1, A2, A3, A4, A5, A6, A7);
			R (O::*vm)(A0, A1, A2, A3, A4, A5, A6, A7);
			struct { void *a; void *b; } dummy;
		} u;
		u.dummy.a = u.dummy.b = NULL;
		u.vm = m;
		DR_Assert(u.dummy.b == NULL);
		return u.vf;
	}

	template <typename O, typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
	static R (O::*			funcToMptr(R (*f)(O*, A0, A1, A2, A3, A4, A5, A6, A7)))(A0, A1, A2, A3, A4, A5, A6, A7)
	{
		union {
			R (*vf)(O*, A0, A1, A2, A3, A4, A5, A6, A7);
			R (O::*vm)(A0, A1, A2, A3, A4, A5, A6, A7);
			struct { void *a; void *b; } dummy;
		} u;
		u.dummy.a = u.dummy.b = NULL;
		u.vf = f;
		return u.vm;
	}
};


DR_NS_END

#endif
