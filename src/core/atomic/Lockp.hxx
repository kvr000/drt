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

#ifndef dr__Lockp__hxx__
# define dr__Lockp__hxx__

#include <dr/def_core.hxx>
#include <dr/Assert.hxx>
#include <dr/Atomic.hxx>

DR_NS_BEGIN


/**
 * Pointer locks support
 *
 * Operations for locking the pointers. This functions can be used for very
 * short locks of pointers.
 */
class DR_PUB Lockp
{
public:
	static void *			lock(void *volatile *ptr);
	static void			unlock(void *volatile *ptr, void *newval);
	static void *			xchg(void *volatile *ptr, void *newval);
	DR_RINLINE static void *	read(void *volatile *ptr);
	DR_RINLINE static bool		locked(void *volatile *ptr);

	DR_RINLINE static void *	i_lock(void *volatile *ptr);
	DR_RINLINE static void		i_unlock(void *volatile *ptr, void *newval);
	DR_RINLINE static void *	i_xchg(void *volatile *ptr, void *newval);

public:
	/* pointer conditional locks - the lock is performed only if locking was set */
	DR_RINLINE static void *	clock(void *volatile *ptr, bool *locked);
	DR_RINLINE static void		cunlock(void *volatile *ptr, void *newval, bool locked);
	DR_RINLINE static void *	cxchg(void *volatile *ptr, void *newval);
	DR_RINLINE static void *	cread(void *volatile *ptr);
	template <typename T>
	DR_RINLINE static T		cmem(void *volatile *ptr, SintPtr t_offs);
	DR_RINLINE static bool		clocked(void *volatile *ptr);
	DR_RINLINE static bool		clocking(void *volatile *ptr);
	DR_RINLINE static void		cinit(void *volatile *ptr, void *val, bool locking);
	static void			creset(void *volatile *ptr, bool locking);

	DR_RINLINE static bool		cvlocking(void *val);
	DR_RINLINE static bool		cvlocked(void *val);

	DR_RINLINE static void *	i_clock(void *volatile *ptr, bool *locked);
	DR_RINLINE static void		i_cunlock(void *volatile *ptr, void *newval, bool locked);
	DR_RINLINE static void *	i_cxchg(void *volatile *ptr, void *newval);
	DR_RINLINE static void *	i_cref(void *volatile *ptr, SintPtr ref_offs);
	DR_RINLINE static void *	i_crefc(void *volatile *ptr, bool *refed, SintPtr ref_offs);
	template <typename T>
	DR_RINLINE static T		i_cmem(void *volatile *ptr, SintPtr t_offs);

public:
	static void			s_init();

private:
	/* std locks support functions */
	static void			p_wake(void *volatile *ptr);

	/* condition locks support functions */
	static void *			p_clock(void *volatile *ptr, bool *locked);
	static void *			p_cxchg(void *volatile *ptr, void *newval);
	static void *			p_cref(void *volatile *ptr, SintPtr ref_offs);
	static void *			p_crefc(void *volatile *ptr, bool *refed, SintPtr ref_offs);
	template <size_t size>
	static void			p_cmem(void *volatile *ptr, SintPtr t_offs, void *dst);
};

DR_RINLINE void *Lockp::read(void *volatile *ptr)
{
	return (void *)((SintPtr)*ptr&-4);
}

DR_RINLINE bool Lockp::locked(void *volatile *ptr)
{
	return ((SintPtr)*ptr&1) != 0;
}

DR_RINLINE void *Lockp::i_lock(void *volatile *ptr)
{
	register SintPtr val = (SintPtr)*ptr;
	if ((val&1) != 0 || !Atomic::cmpxchg(ptr, (void *)val, (void *)(val|3)))
		val = (SintPtr)lock(ptr);
	return (void *)val;
}

DR_RINLINE void Lockp::i_unlock(void *volatile *ptr, void *newval)
{
	newval = Atomic::xchg(ptr, newval);
	if (((SintPtr)newval&2) == 0)
		p_wake(ptr);
}

DR_RINLINE void *Lockp::i_xchg(void *volatile *ptr, void *newval)
{
	register void *old = *ptr;
	if (((SintPtr)old&1) != 0 || !Atomic::cmpxchg(ptr, old, newval))
		old = xchg(ptr, newval);
	return old;
}

DR_RINLINE void *Lockp::clock(void *volatile *ptr, bool *locked)
{
	register SintPtr ret = (SintPtr)*ptr;
	if ((ret&3) == 0)
		*locked = false;
	else
		ret = (SintPtr)p_clock(ptr, locked);
	return (void *)ret;
}

DR_RINLINE void Lockp::cunlock(void *volatile *ptr, void *newval, bool locked)
{
	if (!locked)
		*ptr = newval;
	else
		unlock(ptr, (char *)newval+2);
}

DR_RINLINE void *Lockp::cxchg(void *volatile *ptr, void *newval)
{
	register void *old = *ptr;
	if (((SintPtr)old&3) == 0)
		*ptr = newval;
	else
		old = p_cxchg(ptr, newval);
	return old;
}

DR_RINLINE void *Lockp::cread(void *volatile *ptr)
{
	return (void *)((SintPtr)*ptr&-4);
}

DR_RINLINE bool Lockp::clocked(void *volatile *ptr)
{
	return ((SintPtr)*ptr&1) != 0;
}

DR_RINLINE bool Lockp::clocking(void *volatile *ptr)
{
	return ((SintPtr)*ptr&3) != 0;
}

DR_RINLINE void Lockp::cinit(void *volatile *ptr, void *val, bool locking)
{
	*ptr = (char *)val+locking*2;
}

DR_RINLINE bool Lockp::cvlocking(void *val)
{
	return ((SintPtr)val&3) != 0;
}

DR_RINLINE bool Lockp::cvlocked(void *val)
{
	return ((SintPtr)val&1) != 0;
}

DR_RINLINE void *Lockp::i_clock(void *volatile *ptr, bool *locked)
{
	register SintPtr ret = (SintPtr)*ptr;
	if ((ret&3) == 0)
		*locked = false;
	else if ((ret |= 3), Atomic::cmpxchg(ptr, (char *)ret-1, (void *)ret))
		*locked = true, ret -= 3;
	else
		ret = (SintPtr)p_clock(ptr, locked);
	return (void *)ret;
}

DR_RINLINE void Lockp::i_cunlock(void *volatile *ptr, void *newval, bool locked)
{
	if (!locked)
		*ptr = newval;
	else
		i_unlock(ptr, (char *)newval+2);
}

DR_RINLINE void *Lockp::i_cxchg(void *volatile *ptr, void *newval)
{
	register SintPtr old = (SintPtr)*ptr;
	if ((old&3) == 0)
		*ptr = newval;
	else if (((old |= 3) -= 1), Atomic::cmpxchg(ptr, (char *)old, (char *)newval+2))
		old -= 2;
	else
		old = (SintPtr)p_cxchg(ptr, newval);
	return (void *)old;
}

DR_RINLINE void *Lockp::i_cref(void *volatile *ptr, SintPtr ref_offs)
{
	register SintPtr ret = (SintPtr)*ptr;
	if ((ret&3) == 0)
		Atomic::inc((Refcnt *)(ret+ref_offs));
	else if (((ret |= 3)), Atomic::cmpxchg(ptr, (char *)ret-1, (char *)ret)) {
		Atomic::inc((Refcnt *)(--ret-2+ref_offs));
		i_unlock(ptr, (void *)ret);
		ret -= 2;
	}
	else
		ret = (SintPtr)p_cref(ptr, ref_offs);
	return (void *)ret;
}

DR_RINLINE void *Lockp::i_crefc(void *volatile *ptr, bool *refed, SintPtr ref_offs)
{
	register SintPtr ret = (SintPtr)*ptr;
	if ((ret&3) == 0)
		*refed = false;
	else if (((ret |= 3)), Atomic::cmpxchg(ptr, (char *)ret-1, (char *)ret)) {
		*refed = true;
		Atomic::inc((Refcnt *)(--ret-2+ref_offs));
		i_unlock(ptr, (void *)ret);
		ret -= 2;
	}
	else
		ret = (SintPtr)p_crefc(ptr, refed, ref_offs);
	return (void *)ret;
}

template <typename T>
DR_RINLINE T Lockp::cmem(void *volatile *ptr, SintPtr t_offs)
{
	T out;
	register SintPtr addr = (SintPtr)*ptr;
	if ((addr&3) == 0)
		out = *(T *)(addr+t_offs);
	else
		p_cmem<sizeof(T)>(ptr, t_offs, &out);
	return out;
}

template <typename T>
DR_RINLINE T Lockp::i_cmem(void *volatile *ptr, SintPtr t_offs)
{
	T out;
	register SintPtr addr = (SintPtr)*ptr;
	if ((addr&3) == 0)
		out = *(T *)(addr+t_offs);
	else if (((addr |= 3)), Atomic::cmpxchg(ptr, (char *)addr-1, (char *)addr)) {
		out = *(T *)(--addr-2+t_offs);
		i_unlock(ptr, (void *)addr);
	}
	else
		p_cmem<sizeof(T)>(ptr, t_offs, &out);
	return out;
}


DR_NS_END

#endif
