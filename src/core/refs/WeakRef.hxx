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

#ifndef dr__WeakRef__hxx__
# define dr__WeakRef__hxx__

#include <dr/Alloc.hxx>
#include <dr/Atomic.hxx>
#include <dr/Assert.hxx>

DR_NS_BEGIN


class DR_PUB WeakRef_g
{
public:
	DR_RINLINE			WeakRef_g(void *ptr);

	DR_RINLINE WeakRef_g *		ref();
	DR_RINLINE void *		unref();

public:
	DR_RINLINE static void *	operator new(size_t size);
	DR_RINLINE static void		operator delete(void *m);

protected:
	Refcnt				refcnt;
	void *				ptr;
};

/**
 * Weak Reference
 *
 * Holds weak reference to an object, e.g. the object is not referenced by this
 * holder and when the object disappears, this holder forgets the object.
 */
template <typename T>
class WeakRef: public WeakRef_g
{
public:
	DR_RINLINE			WeakRef(T *ptr);
	DR_RINLINE			~WeakRef();

	DR_RINLINE WeakRef *		ref();
	DR_RINLINE T *			unref();
	DR_RINLINE T *			get();
	DR_RINLINE void			reset();
};

DR_RINLINE void *WeakRef_g::operator new(size_t size)
{
	return Alloc::allocS(size);
}

DR_RINLINE void WeakRef_g::operator delete(void *m)
{
	return Alloc::freeS(m, sizeof(WeakRef_g));
}

DR_RINLINE WeakRef_g::WeakRef_g(void *ptr_):
	refcnt(0),
	ptr(ptr_)
{
}

DR_RINLINE WeakRef_g *WeakRef_g::ref()
{
	Atomic::inc(&refcnt);
	return this;
}

DR_RINLINE void *WeakRef_g::unref()
{
	void *ret = ptr;
	if (!Atomic::decr(&refcnt))
		delete this;
	return ret;
}


template <typename T>
DR_RINLINE WeakRef<T>::WeakRef(T *ptr_):
	WeakRef_g(ptr_)
{
	DR_LOG3(("%s: creating %p\n", DR_FUNCTION, this));
}

template <typename T>
DR_RINLINE WeakRef<T>::~WeakRef()
{
	DR_LOG3(("%s: destroying %p\n", DR_FUNCTION, this));
}

template <typename T>
DR_RINLINE WeakRef<T> *WeakRef<T>::ref()
{
	DR_LOG3(("%s: referencing %p (%ld)\n", DR_FUNCTION, this, (long)refcnt+1));
	WeakRef_g::ref();
	return this;
}

template <typename T>
DR_RINLINE T *WeakRef<T>::unref()
{
	DR_LOG3(("%s: unreferencing %p (%ld)\n", DR_FUNCTION, this, (long)refcnt-1));
	return (T *)WeakRef_g::unref();
}

template <typename T>
DR_RINLINE T *WeakRef<T>::get()
{
	DR_LOG3(("%s: getting %p (%ld)\n", DR_FUNCTION, this, (long)refcnt));
	return (T *)ptr;
}

template <typename T>
DR_RINLINE void WeakRef<T>::reset()
{
	DR_LOG3(("%s: reseting %p (%ld)\n", DR_FUNCTION, this, (long)refcnt));
	ptr = NULL;
}


DR_NS_END

#endif
