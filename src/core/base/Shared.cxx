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

#ifndef dr__drShared__cxx__
# define dr__drShared__cxx__

#include <dr/Shared.hxx>

DR_NS_BEGIN


bool Shared::unref()
{
	DR_LOG2(("%s: unreferencing %p, new refs %d+1\n", DR_FUNCTION, this, refcnt-1));
	if (!Atomic::decr(&refcnt)) {
		unrefed();
		return false;
	}
	return true;
}

#if 0
class SharedPtr_g;
class SharedLockp_g;


class SharedPtr_g: public Base
{
protected:
	Shared *			sh;

public:
	DR_MINLINE			SharedPtr_g()				: sh(NULL) {}
	DR_MINLINE			SharedPtr_g(Shared *s): sh(s) {}
	DR_MINLINE			SharedPtr_g(const SharedPtr_g &s): sh(s.sh?s.sh->ref():NULL) {}
	DR_COREMET			SharedPtr_g(const SharedLockp_g &s);
	DR_COREMET			~SharedPtr_g();

	DR_COREMET SharedPtr_g &	operator=(const SharedPtr_g &s);
	DR_COREMET SharedPtr_g &	operator=(const SharedLockp_g &s);

	DR_RINLINE Shared *		getInc() const { return sh->ref(); }
	DR_RINLINE Shared *		getIncNull() const { return sh?sh->ref():NULL; }
	DR_RINLINE Shared *		i_getInc() const { return sh->ref(); }
	DR_RINLINE Shared *		i_getIncNull() const { return sh?sh->ref():NULL; }
};


class SharedLockp_g: public Base
{
protected:
	mutable Shared	*sh;

public:
	DR_MINLINE			SharedLockp_g()			: sh(NULL) {}
	DR_MINLINE			SharedLockp_g(Shared *s)		: sh(s) {}
	DR_MINLINE			SharedLockp_g(const SharedLockp_g &s): sh(s.getIncNull()) {}
	DR_MINLINE			SharedLockp_g(const SharedPtr_g &s)	: sh(s.getIncNull()) {}
	DR_COREMET			~SharedLockp_g();

	DR_COREMET SharedLockp_g &	operator=(const SharedLockp_g &s);
	DR_COREMET SharedLockp_g &	operator=(const SharedPtr_g &s);

	DR_COREMET Shared *		getInc() const;
	DR_COREMET Shared *		getIncNull() const;
	DR_RINLINE Shared *		i_getInc() const			{ Shared *o = (Shared *)Lockp::i_lock((void **)&sh); o->i_ref(); Lockp::i_unlock((void **)&sh, o); return o; }
	DR_RINLINE Shared *		i_getIncNull() const			{ Shared *o = (Shared *)Lockp::i_lock((void **)&sh); if (o) o->i_ref(); Lockp::i_unlock((void **)&sh, o); return o; }
};


template <typename TS>
class SharedRP_g: public Base
{
protected:
	TS *				sh;

public:
	DR_MINLINE			SharedRP_g()				: sh(NULL) {}
	DR_MINLINE			SharedRP_g(TS *s, const Noref &)	: sh(s) {}
	DR_MINLINE			SharedRP_g(TS *s, const Doref &)	: sh(s) { if (s) s->ref(); }
	DR_MINLINE			~SharedRP_g()				{ if (sh) sh->unref(); }

	DR_RINLINE 			operator TS *()				{ return sh; }

	DR_MINLINE SharedRP_g &	setnoref(TS *s)				{ if (sh) sh->unref(); sh = s; }
	DR_MINLINE SharedRP_g &	setref(TS *s)				{ if (s) s->ref(); if (sh) sh->unref(); sh = s; }
	DR_MINLINE TS *			ref()					{ if (sh) sh->ref(); return sh; }
	DR_MINLINE TS *			noref()					{ return sh; }

protected:
	DR_MINLINE			SharedRP_g(const SharedRP_g &s)	: sh(s.sh) { if (sh) sh->ref(); }
	DR_RINLINE SharedRP_g &	operator=(const SharedRP_g &s)	{ if (s.sh) s.sh->ref(); if (sh) sh->unref(); sh = s.sh; }
};

/**
 * shared pointer
 * explicit reference count for most operations
 */
template <typename TS>
class SharedPtr: public SharedRP_g<TS>
{
	typedef SharedRP_g<TS>	Base;

protected:
	TS *				sh;

public:
	DR_MINLINE			SharedPtr()				: Base() {}
	DR_MINLINE			SharedPtr(TS *s)			: Base(s, Noref()) {}
	DR_MINLINE			SharedPtr(const SharedPtr &s)	: Base(s) {}

	DR_RINLINE SharedPtr &	operator=(const SharedPtr &s)		{ return Base::operator=(s); }

	DR_MINLINE SharedPtr &	set(TS *s)				{ return this->setnoref(s); }
	DR_MINLINE TS *			get()					{ return this->noref(); }
};


/**
 * shared reference
 * implicit reference count for all operations
 */
template <typename TS>
class SharedRef: public SharedRP_g<TS>
{
	typedef SharedRP_g<TS>	Base;

protected:
	TS *				sh;

public:
	DR_MINLINE			SharedRef()				: Base() {}
	DR_MINLINE			SharedRef(TS *s)			: Base(s, Doref()) {}
	DR_MINLINE			SharedRef(const SharedRef &s)	: Base(s) {}

	DR_RINLINE SharedRef &	operator=(const SharedRef &s)		{ return Base::operator=(s); }

	DR_MINLINE SharedRef &	set(TS *s)				{ return this->setref(s); }
	DR_MINLINE TS *			get()					{ return this->ref(); }
};


template <typename TS>
class SharedLockp: public Base
{
};

DR_EXPORT_MET SharedPtr_g::SharedPtr_g(const SharedLockp_g &s)
{
	sh = s.getIncNull();
}

DR_EXPORT_MET SharedPtr_g::~SharedPtr_g()
{
	if (sh)
		sh->i_unref();
}

DR_EXPORT_MET SharedPtr_g &SharedPtr_g::operator=(const SharedPtr_g &s)
{
	Shared *o = sh;
	sh = s.i_getIncNull();
	if (o)
		o->i_unref();
	return *this;
}

DR_EXPORT_MET SharedPtr_g &SharedPtr_g::operator=(const SharedLockp_g &s)
{
	Shared *o = sh;
	sh = s.i_getIncNull();
	if (o)
		o->i_unref();
	return *this;
}

DR_EXPORT_MET SharedLockp_g::~SharedLockp_g()
{
	if (sh)
		sh->i_unref();
}

DR_EXPORT_MET SharedLockp_g &SharedLockp_g::operator=(const SharedLockp_g &s)
{
	Shared *n = s.i_getIncNull();
	Shared *o = (Shared *)Lockp::i_lock((void **)&sh);
	if (o)
		o->i_unref();
	Lockp::i_unlock((void **)&sh, n);
	return *this;
}

DR_EXPORT_MET SharedLockp_g &SharedLockp_g::operator=(const SharedPtr_g &s)
{
	Shared *n = s.i_getIncNull();
	Shared *o = (Shared *)Lockp::i_lock((void **)&sh);
	if (o)
		o->i_unref();
	Lockp::i_unlock((void **)&sh, n);
	return *this;
}

DR_EXPORT_MET Shared *SharedLockp_g::getInc() const
{
	Shared *o = (Shared *)Lockp::i_lock((void **)&sh);
	o->i_ref();
	Lockp::i_unlock((void **)&sh, o);
	return o;
}

DR_EXPORT_MET Shared *SharedLockp_g::getIncNull() const
{
	Shared *o = (Shared *)Lockp::i_lock((void **)&sh);
	if (o)
		o->i_ref();
	Lockp::i_unlock((void **)&sh, o);
	return o;
}

#endif

DR_NS_END

#endif
