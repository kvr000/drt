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

#include <string.h>

#include <dr/Assert.hxx>
#include <dr/Atomic.hxx>
#include <dr/Mutex.hxx>
#include <dr/MutexCond.hxx>

#include <dr/Lockp.hxx>

DR_NS_BEGIN


#define LOW_COND_MAX 255

static MutexCond *low_conds[LOW_COND_MAX+1];

static inline MutexCond *accCond(void *volatile *ptr)
{
	MutexCond *cond;
	UintPtr hash = (UintPtr)ptr;
#if DR_SIZE_PTR > 4
	hash += hash>>32;
#endif
	hash += hash>>16;
#if LOW_COND_MAX < 256
	hash += hash>>8;
#endif
	hash %= LOW_COND_MAX+1;

	if (low_conds[hash])
		return low_conds[hash];
	cond = MutexCond::create();
	if (!Atomic::cmpxchg((void **)(void *)&low_conds[hash], NULL, cond))
		cond->unref();
	return low_conds[hash];
}

void Lockp::s_init()
{
	for (size_t hash = 0; hash <= LOW_COND_MAX; hash++) {
		if (low_conds[hash] != NULL)
			continue;
		MutexCond *cond = MutexCond::create();
		if (!Atomic::cmpxchg((void **)(void *)&low_conds[hash], NULL, cond))
			cond->unref();
	}
}

/* pointer has three states:
 * 0: unlocked
 * 1: locked, another thread waiting
 * 2: unlocked, using conditional lock
 * 3: locked
 */

DR_EXPORT_MTS void Lockp::p_wake(void *volatile *ptr)
{
	MutexCond *cond = accCond(ptr);
	cond->lock();
	cond->signal();
	cond->unlock();
}

DR_EXPORT_MTS void *Lockp::lock(void *volatile *ptr)
{
	MutexCond *cond = NULL;
	SintPtr val;

	if (1) {
		// use aggressive approach on initialization and always suppose
		// unlocked on the first try
		val = (SintPtr)*ptr&~(SintPtr)1;
	}
	else {
retry:
		val = (SintPtr)*ptr;
	}

	if ((val&1) == 0) {
		if (!Atomic::cmpxchg(ptr, (void *)val, (void *)(val&~3|(cond?1:3))))
			goto retry;
	}
	else {
		if ((val&2) != 0) {
			if (!Atomic::cmpxchg(ptr, (void *)val, (char *)val-2))
				goto retry;
			val -= 2;
		}
		if (!cond)
			(cond = accCond(ptr))->lock();
		if (*ptr == (void *)val)
			cond->wait();
		goto retry;
	}
	if (cond)
		cond->unlock();
	return (void *)val;
}

DR_EXPORT_MTS void Lockp::unlock(void *volatile *ptr, void *newval)
{
	newval = Atomic::xchg(ptr, newval);

	DR_Assert(((SintPtr)newval&1) != 0);

	if (((SintPtr)newval&2) == 0)
		p_wake(ptr);
}

DR_EXPORT_MTS void *Lockp::xchg(void *volatile *ptr, void *newval)
{
	void *old = *ptr;

	if (((SintPtr)old&1) != 0 || !Atomic::cmpxchg(ptr, old, newval)) {
		old = i_lock(ptr);
		i_unlock(ptr, newval);
	}
	return old;
}

DR_EXPORT_MTS void Lockp::creset(void *volatile *ptr, bool locking)
{
	SintPtr val = (SintPtr)*ptr;

	if ((val&1) != 0 || !Atomic::cmpxchg(ptr, (void *)val, (char *)(val&-4)+2*locking)) {
		val = (SintPtr)lock(ptr);
		unlock(ptr, (char *)(val&-4)+2*locking);
	}
}

DR_EXPORT_MTS void *Lockp::p_clock(void *volatile *ptr, bool *locked)
{
	SintPtr ret = (SintPtr)*ptr;

	if ((ret&3) == 0) {
		*locked = false;
	}
	else if ((ret |= 3), Atomic::cmpxchg(ptr, (char *)ret-1, (char *)ret)) {
		*locked = true;
		ret -= 3;
	}
	else {
		ret = (SintPtr)i_lock(ptr);
		if ((*locked = ((ret&2) != 0)))
			ret -= 2;
	}
	return (void *)ret;
}

DR_EXPORT_MTS void *Lockp::p_cxchg(void *volatile *ptr, void *newval)
{
	SintPtr old = (SintPtr)*ptr;

	if ((old&3) == 0)
		*ptr = newval;
	else if (((old |= 3) -= 1), Atomic::cmpxchg(ptr, (char *)old, (char *)newval+2))
		old -= 2;
	else {
		old = (SintPtr)lock(ptr);
		if ((old&2) != 0) {
			old -= 2;
			newval = (char *)newval+2;
		}
		i_unlock(ptr, newval);
	}
	return (void *)old;
}

DR_EXPORT_MTS void *Lockp::p_cref(void *volatile *ptr, SintPtr ref_offs)
{
	SintPtr ret = (SintPtr)*ptr;

	if ((ret&3) == 0) {
		Atomic::inc((Refcnt *)(ret+ref_offs));
	}
	else if ((ret |= 3), Atomic::cmpxchg(ptr, (char *)ret-1, (char *)ret)) {
		Atomic::inc((Refcnt *)(--ret+ref_offs));
		i_unlock(ptr, (void *)ret);
		ret -= 2;
	}
	else {
		SintPtr real = (SintPtr)i_lock(ptr);
		ret = real&-4;
		Atomic::inc((Refcnt *)(ret+ref_offs));
		i_unlock(ptr, (void *)real);
	}
	return (void *)ret;
}

DR_EXPORT_MTS void *Lockp::p_crefc(void *volatile *ptr, bool *refed, SintPtr ref_offs)
{
	SintPtr ret = (SintPtr)*ptr;

	if (((SintPtr)ret&3) == 0) {
		*refed = false;
	}
	else if ((ret |= 3), Atomic::cmpxchg(ptr, (char *)ret-1, (char *)ret)) {
		*refed = true;
		Atomic::inc((Refcnt *)(--ret-2+ref_offs));
		i_unlock(ptr, (void *)ret);
		ret -= 2;
	}
	else {
		SintPtr real = (SintPtr)lock(ptr);
		ret = real;
		if ((*refed = (real&2) != 0)) {
			ret -= 2;
			Atomic::inc((Refcnt *)ret);
		}
		i_unlock(ptr, (void *)real);
	}
	return (void *)ret;
}

template <>
DR_EXPORT_MTS void Lockp::p_cmem<4>(void *volatile *ptr, SintPtr t_offs, void *dst)
{
	register SintPtr addr = (SintPtr)*ptr;
	if ((addr&3) == 0)
		*(Uint32 *)dst = *(Uint32 *)(addr+t_offs);
	else if (((addr |= 3)), Atomic::cmpxchg(ptr, (char *)addr-1, (char *)addr)) {
		*(Uint32 *)dst = *(Uint32 *)(--addr-2+t_offs);
		i_unlock(ptr, (void *)addr);
	}
	else {
		addr = (SintPtr)i_lock(ptr);
		*(Uint32 *)dst = *(Uint32 *)((addr&-4)+t_offs);
		i_unlock(ptr, (void *)addr);
	}
}

template <>
DR_EXPORT_MTS void Lockp::p_cmem<8>(void *volatile *ptr, SintPtr t_offs, void *dst)
{
	register SintPtr addr = (SintPtr)*ptr;
	if ((addr&3) == 0)
		*(Uint64 *)dst = *(Uint64 *)(addr+t_offs);
	else if (((addr |= 3)), Atomic::cmpxchg(ptr, (char *)addr-1, (char *)addr)) {
		*(Uint64 *)dst = *(Uint64 *)(--addr-2+t_offs);
		i_unlock(ptr, (void *)addr);
	}
	else {
		addr = (SintPtr)i_lock(ptr);
		*(Uint64 *)dst = *(Uint64 *)((addr&-4)+t_offs);
		i_unlock(ptr, (void *)addr);
	}
}

template <>
DR_EXPORT_MTS void Lockp::p_cmem<1>(void *volatile *ptr, SintPtr t_offs, void *dst)
{
	register SintPtr addr = (SintPtr)*ptr;
	if ((addr&3) == 0)
		*(Uint8 *)dst = *(Uint8 *)(addr+t_offs);
	else if (((addr |= 3)), Atomic::cmpxchg(ptr, (char *)addr-1, (char *)addr)) {
		*(Uint8 *)dst = *(Uint8 *)(--addr-2+t_offs);
		i_unlock(ptr, (void *)addr);
	}
	else {
		addr = (SintPtr)i_lock(ptr);
		*(Uint8 *)dst = *(Uint8 *)((addr&-4)+t_offs);
		i_unlock(ptr, (void *)addr);
	}
}


DR_NS_END
