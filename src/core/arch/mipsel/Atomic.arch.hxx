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
#endif
};


#ifdef DR_NOARCH

#elif (defined DR_CXX_GCC)

DR_RINLINE bool Atomic::cmpxchg(void *volatile *val, void *cmpval, void *newval)
{
	void *loaded;
	__asm__ __volatile__(
			"	.set	push		\n"
			"	.set	noat		\n"
			"	.set	mips3		\n"
			"1:				\n"
			"	ll	%0, %2		\n"
			"	bne	%0, %z3, 2f	\n"
			"	.set	mips0		\n"
			"	move	$1, %z4		\n"
			"	.set	mips3		\n"
			"	sc	$1, %1		\n"
			"	beqzl	$1, 1b		\n"
			"2:				\n"
			"	.set	pop		\n"
			: "=&r"(loaded), "=R"(*val)
			: "R"(*val), "Jr"(cmpval), "Jr"(newval)
			: "memory"
			);
	return loaded == cmpval;
}

#endif


DR_NS_END
