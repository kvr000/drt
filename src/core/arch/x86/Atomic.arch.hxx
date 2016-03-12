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

#ifndef dr__Atomic__hxx__
# error dont include arch files directly
#endif

DR_NS_BEGIN


/**
 * @see Atomic_generic
 */
class DR_PUB Atomic: public Atomic_generic
{
#ifndef DR_NOARCH
public:
	DR_RINLINE static bool		cmpxchg(void *volatile *val, void *cmpval, void *newval);
	DR_RINLINE static void *	xchg(void *volatile *val, void *newval);
	DR_RINLINE static bool		incr(volatile SintPtr *val);
	DR_RINLINE static bool		decr(volatile SintPtr *val);
	DR_RINLINE static void		inc(volatile SintPtr *val);
	DR_RINLINE static void		dec(volatile SintPtr *val);
	DR_RINLINE static bool		incdecr(volatile SintPtr *valInc, volatile SintPtr *valDec);
#endif
};


#ifdef DR_NOARCH

#elif (defined DR_CXX_GCC)

DR_RINLINE bool Atomic::cmpxchg(void *volatile *val, void *cmpval, void *newval)
{
	char e;
	__asm__ __volatile__(
			"lock; cmpxchgl %3, %1\n\t"
			"sete %b0\n\t"
			: "=a"(e), "+m"(*val)
			: "0"(cmpval), "r"(newval)
			);
	return e;
}

DR_RINLINE void *Atomic::xchg(void *volatile *val, void *newval)
{
	__asm__ __volatile__(
			"lock; xchgl %0, %1\n\t"
			: "=r"(newval), "+m"(*val)
			: "0"(newval)
			);
	return newval;
}

DR_RINLINE bool Atomic::incr(volatile SintPtr *val)
{
	char nz;
	__asm__ __volatile__(
			"lock; incl %1\n\t"
			"setnz %b0\n\t"
			: "=q"(nz), "+m"(*val)
			);
	return nz;
}

DR_RINLINE bool Atomic::decr(volatile SintPtr *val)
{
	char ns;
	__asm__ __volatile__(
			"lock; decl %1\n\t"
			"setns %b0\n\t"
			: "=q"(ns), "+m"(*val)
			);
	return ns;
}

DR_RINLINE void Atomic::inc(volatile SintPtr *val)
{
	__asm__ __volatile__(
			"lock; incl %0"
			: "+m"(*val)
			);
}

DR_RINLINE void Atomic::dec(volatile SintPtr *val)
{
	__asm__ __volatile__(
			"lock; decl %0"
			: "+m"(*val)
			);
}

DR_RINLINE bool Atomic::incdecr(volatile SintPtr *valInc, volatile SintPtr *valDec)
{
	char ns;
	__asm__ __volatile__(
			"lock; incl %0\n\t"
			: "+m"(*valInc)
			);
	__asm__ __volatile__(
			"lock; decl %1\n\t"
			"setns %b0\n\t"
			: "=q"(ns), "+m"(*valDec)
			);
	return ns;
}

#elif (defined DR_CXX_VC)

DR_RINLINE bool Atomic::cmpxchg(void *volatile *val, void *cmpval, void *newval)
{
	char e;
	__asm {
		mov	eax, cmpval;
		mov	edx, newval;
		mov	ecx, val;
		lock cmpxchg	[ecx], edx;
		sete	cl;
		mov	e, cl;
	};
	return e;
}

DR_RINLINE void *Atomic::xchg(void *volatile *val, void *newval)
{
	__asm {
		mov	ecx, val;
		mov	edx, newval;
		lock xchg	[ecx], edx;
		mov	newval, edx;
	};
	return newval;
}

DR_RINLINE bool Atomic::incr(volatile SintPtr *val)
{
	char nz;
	__asm {
		mov	ecx, val;
		lock inc	dword ptr [ecx];
		setnz	cl;
		mov	nz, cl;
	};
	return nz;
}

DR_RINLINE bool Atomic::decr(volatile SintPtr *val)
{
	char ns;
	__asm {
		mov	ecx, val;
		lock dec	dword ptr [ecx];
		setns	cl;
		mov	ns, cl;
	};
	return ns;
}

DR_RINLINE void Atomic::inc(volatile SintPtr *val)
{
	__asm {
		mov	ecx, val;
		lock inc	dword ptr [ecx];
	};
}

DR_RINLINE void Atomic::dec(volatile SintPtr *val)
{
	__asm {
		mov	ecx, val;
		lock dec	dword ptr [ecx];
	};
}

DR_RINLINE bool Atomic::incdecr(volatile SintPtr *valInc, volatile SintPtr *valDec)
{
	char ns;
	__asm {
		mov	ecx, valInc;
		lock inc	dword ptr [ecx];
	};
	__asm {
		mov	ecx, valDec;
		lock dec	dword ptr [ecx];
		setns	cl;
		mov	ns, cl;
	};
	return ns;
}

#else

// ignore, use default implementation

#endif


DR_NS_END
