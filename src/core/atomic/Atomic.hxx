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
# define dr__Atomic__hxx__

#include <dr/types.hxx>

DR_NS_BEGIN


/**
 * Atomic class operations
 */
class DR_PUB Atomic_generic
{
public:
	/**
	 * atomic compare and exchange
	 *
	 * Compares *val to cmpval, and if equal set *val to newval
	 *
	 * @return true			if *val was equal to cmpval
	 * @return false		otherwise
	 */
	static bool			cmpxchg(void *volatile *val, void *cmpval, void *newval);

	/**
	 * atomic exchange
	 *
	 * Exchanges *val with newval
	 *
	 * @return			original *val
	 */
	DR_RINLINE static void *	xchg(void *volatile *val, void *newval);

	/**
	 * atomic increment with overflow detection
	 *
	 * Increments *val
	 *
	 * @return false		if the new value is 0
	 * @return true			if the new value is not zero
	 *
	 * @note			in case the old value was less than -1,
	 *				the result is undefined
	 */
	DR_RINLINE static bool		incr(volatile SintPtr *val);

	/**
	 * atomic decrement with overflow detection
	 *
	 * Decrements *val
	 *
	 * @return false		if the new value is < 0
	 * @return true			if the new value is >= 0
	 *
	 * @note			in case the old value was less than 0,
	 *				the result is undefined
	 */
	DR_RINLINE static bool		decr(volatile SintPtr *val);

	/**
	 * atomic increment
	 *
	 * Increments *val
	 */
	DR_RINLINE static void		inc(volatile SintPtr *val);

	/**
	 * atomic decrement
	 *
	 * Decrements *val
	 */
	DR_RINLINE static void		dec(volatile SintPtr *val);

	/**
	 * atomic increment and decrement
	 *
	 * Increments *valInc, decrements *valDec
	 *
	 * @return true			if the original *valDec wasn't 0
	 * @return false		if the original *valDec was 0
	 *
	 * @note			in case the original *valDec was less
	 *				than 0, the result is undefined
	 */
	DR_RINLINE static bool		incdecr(volatile SintPtr *valInc, volatile SintPtr *valDec);
};


DR_NS_END

#include <dr/arch/Atomic.arch.hxx>

DR_NS_BEGIN


DR_RINLINE void *Atomic_generic::xchg(void *volatile *val, void *newval)
{
	void *o;
	do {
		o = *val;
	} while (!Atomic::cmpxchg(val, o, newval));
	return o;
}

DR_RINLINE bool Atomic_generic::incr(volatile SintPtr *val)
{
	SintPtr o;
	do {
		o = *val;
	} while (!Atomic::cmpxchg((void **)val, (void *)o, (char *)o+1));
	return o != -1;
}

DR_RINLINE bool Atomic_generic::decr(volatile SintPtr *val)
{
	SintPtr o;
	do {
		o = *val;
	} while (!Atomic::cmpxchg((void **)val, (void *)o, (char *)o-1));
	return o > 0;
}

DR_RINLINE void Atomic_generic::inc(volatile SintPtr *val)
{
	Atomic::incr(val);
}

DR_RINLINE void Atomic_generic::dec(volatile SintPtr *val)
{
	Atomic::decr(val);
}

DR_RINLINE bool Atomic_generic::incdecr(volatile SintPtr *valInc, volatile SintPtr *valDec)
{
	Atomic::inc(valInc);
	return Atomic::decr(valDec);
}


DR_NS_END

#endif
