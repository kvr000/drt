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


#ifndef DR_NOARCH

DR_RINLINE bool Atomic::cmpxchg(void *volatile *val, void *cmpval, void *newval)
{
	bool e;
	__asm__ __volatile__(
			"lock; cmpxchgq %3, %1\n\t"
			"sete %b0\n\t"
			: "=a"(e), "+m"(*val)
			: "0"(cmpval), "r"(newval)
			);
	return e;
}

DR_RINLINE void *Atomic::xchg(void *volatile *val, void *newval)
{
	__asm__ __volatile__(
			"lock; xchgq %0, %1\n\t"
			: "=r"(newval), "+m"(*val)
			: "0"(newval)
			);
	return newval;
}

DR_RINLINE bool Atomic::incr(volatile SintPtr *val)
{
	bool nz;
	__asm__ __volatile__(
			"lock; incq %1\n\t"
			"setnz %0\n\t"
			: "=g"(nz), "+m"(*val)
			);
	return nz;
}

DR_RINLINE bool Atomic::decr(volatile SintPtr *val)
{
	bool ns;
	__asm__ __volatile__(
			"lock; decq %1\n\t"
			"setns %0\n\t"
			: "=g"(ns), "+m"(*val)
			);
	return ns;
}

DR_RINLINE void Atomic::inc(volatile SintPtr *val)
{
	__asm__ __volatile__(
			"lock; incq %0"
			: "+m"(*val)
			);
}

DR_RINLINE void Atomic::dec(volatile SintPtr *val)
{
	__asm__ __volatile__(
			"lock; decq %0"
			: "+m"(*val)
			);
}

DR_RINLINE bool Atomic::incdecr(volatile SintPtr *valInc, volatile SintPtr *valDec)
{
	bool ns;
	__asm__ __volatile__(
			"lock; incq %0\n\t"
			: "+m"(*valInc)
			);
	__asm__ __volatile__(
			"lock; decq %1\n\t"
			"setns %0\n\t"
			: "=g"(ns), "+m"(*valDec)
			);
	return ns;
}

#endif


DR_NS_END
