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

#ifndef dr__Ref__hxx__
# define dr__Ref__hxx__

#include <dr/types.hxx>
#include <dr/Atomic.hxx>

DR_NS_BEGIN


template <typename Type>
void DR_REF_XCHG(Type **dst, Type *src)
{
	Type *tmp = *dst;
	*dst = src;
	if (tmp)
		tmp->unref();
}


/**
 * Reference holder
 *
 * Automatically unreferences holded object on destruction
 */
template <typename Type>
class Ref: public Base
{
protected:
	Type *				obj;

public:
	/**
	 * constructs Ref setting the held object to NULL
	 */
	DR_MINLINE			Ref():
		obj(NULL)
	{
	}

	/**
	 * constructs Ref setting the held object, not referencing it
	 *
	 * @param s
	 * 	held object, the reference is lost
	 */
	DR_MINLINE			Ref(Type *s, const Noref &):
		obj(s)
	{
	}

	/**
	 * constructs Ref setting the held object, referencing it
	 *
	 * @param s
	 * 	held object
	 */
	DR_MINLINE			Ref(Type *s, const Doref &):
		obj(s)
	{
		if (s)
			s->ref();
	}

	/**
	 * constructs Ref setting the held object, referencing it depending on
	 * parameter
	 *
	 * @param s
	 * 	held object
	 * @param doref
	 * 	indicates if object should be referenced first
	 */
	DR_MINLINE			Ref(Type *s, bool doref):
		obj(s)
	{
		if (s && doref)
			s->ref();
	}

	/**
	 * constructs Ref copying the held object, referencing it
	 *
	 * @param s
	 * 	old reference
	 */
	DR_MINLINE			Ref(const Ref &s):
		obj(s.getDoref())
	{
	}

	/**
	 * constructs Ref copying the held object, referencing it
	 *
	 * @param s
	 * 	old reference
	 */
	template <typename TS>
	DR_MINLINE			Ref(const Ref<TS> &s):
		obj(s.getDoref())
	{
	}

	/**
	 * destroys the reference, unreferencing the object
	 */
	DR_MINLINE			~Ref()
	{
		if (obj)
			obj->unref();
	}

	/**
	 * assigns from existing reference, referencing the object
	 */
	template <typename TS>
	DR_RINLINE Ref &		operator=(const Ref<TS> &s)
	{
		setNoref(s.getDoref());
		return *this;
	}

	DR_RINLINE Ref &		operator=(const Ref &s)
	{
		setNoref(s.getDoref());
		return *this;
	}

	/**
	 * indicates if object is set (i.e. is not NULL)
	 *
	 * @return true
	 * 	if the object is set
	 * @return false
	 * 	if the object isn't set
	 */
	DR_RINLINE			operator bool() const
	{
		return obj != NULL;
	}

	/**
	 * returns the held object
	 */
	template <typename TS>
	DR_RINLINE			operator TS *() const
	{
		return obj;
	}

	/**
	 * direct access to held object
	 */
	DR_RINLINE Type *		operator->() const
	{
		return obj;
	}

	/**
	 * direct access to held object
	 */
	DR_RINLINE Type *		operator*() const
	{
		return obj;
	}

	/**
	 * check if object is null
	 */
	DR_RINLINE bool			isNull()
	{
		return obj == NULL;
	}

	/**
	 * reset and unref object
	 */
	DR_RINLINE void			setNull()
	{
		if (Type *old = obj) {
			obj = NULL;
			old->unref();
		}
	}

	/**
	 * assigns the object, not referencing it
	 *
	 * @param s
	 * 	the assigned object
	 */
	DR_MINLINE Ref &		setNoref(Type *s)
	{
		Type *old = obj;
		obj = s;
		if (old)
			old->unref();
		return *this;
	}

	/**
	 * assigns the object, referencing it
	 *
	 * @param s
	 * 	the assigned object
	 */
	DR_MINLINE Ref &		setDoref(Type *s)
	{
		if (s)
			s->ref();
		Type *old = obj;
		obj = s;
		if (old)
			old->unref();
		return *this;
	}

	/**
	 * updates the object (sets only if it's not set yet), in case the held
	 * object is already set, the passed one is unreferenced, the update is
	 * thread safe
	 *
	 * @param s
	 * 	the assigned object
	 */
	DR_MINLINE void			updateUnref_ts(Type *s)
	{
		if (obj || !Atomic::cmpxchg((void **)(void *)&obj, NULL, s))
			s->unref();
	}

	/**
	 * updates the object (sets only if it's not set yet), referencing it,
	 * the update is thread safe
	 *
	 * @param s
	 * 	the assigned object
	 */
	DR_MINLINE void			updateRef_ts(Type *s)
	{
		if (!obj && s) {
			s->ref();
			updateUnref_ts(s);
		}
	}

	/**
	 * gets the object, referencing it
	 */
	DR_MINLINE Type *		getDoref() const
	{
		Type *o = obj;
		if (o)
			o->ref();
		return o;
	}

	/**
	 * gets the object, not referencing it
	 */
	DR_MINLINE Type *		getNoref() const
	{
		return obj;
	}

	/**
	 * gets the object, resetting it
	 */
	DR_MINLINE Type *		getAndNull()
	{
		Type *o = obj;
		obj = NULL;
		return o;
	}

	/**
	 * gets pointer to the object memory
	 */
	DR_MINLINE Type **		mem()
	{
		return &obj;
	}

	DR_MINLINE Type *		xchg(Ref *chg)
	{
		Type *o = obj;
		obj = chg->obj;
		chg->obj = o;
	}

private:
	template <typename TS>
	Ref &				operator=(TS *s);
};

/**
 * Reference holder with implicit auto-reference
 *
 * This holder extends basic holder by auto-reference for constructor
 */
template <typename Type>
class IRef: public Ref<Type>
{
	typedef Ref<Type>		Base;

public:
	DR_MINLINE			IRef():
		Base()
	{
	}

	template <typename TS>
	DR_MINLINE			IRef(TS *s):
		Base(s, true)
	{
	}

	DR_MINLINE			IRef(Type *s, const Noref &t):
		Base(s, t)
	{
	}

	DR_MINLINE			IRef(Type *s, const Doref &t):
		Base(s, t)
	{
	}

	DR_MINLINE			IRef(const IRef &s):
		Base(s)
	{
	}

	DR_MINLINE			IRef(const Base &s):
		Base(s)
	{
	}

	template <typename TS>
	DR_MINLINE			IRef(const Ref<TS> &s):
		Base(s)
	{
	}

private:
	template <typename T>
	IRef &				operator=(T *s);
};

/**
 * Reference holder with explicit auto-reference
 *
 * This holder extends basic holder by noauto-reference for constructor
 */
template <typename Type>
class ERef: public Ref<Type>
{
	typedef Ref<Type>		Base;

public:
	DR_MINLINE			ERef():
		Base()
	{
	}

	template <typename TS>
	DR_MINLINE			ERef(TS *s): Base(s, false)
	{
	}

	DR_MINLINE			ERef(Type *s, const Noref &t):
		Base(s, t)
	{
	}

	DR_MINLINE			ERef(Type *s, const Doref &t):
		Base(s, t)
	{
	}

	DR_MINLINE			ERef(const ERef &s):
		Base(s)
	{
	}

	DR_MINLINE			ERef(const Base &s):
		Base(s)
	{
	}

	template <typename TS>
	DR_MINLINE			ERef(const Ref<TS> &s):
		Base(s)
	{
	}

private:
	template <typename T>
	ERef &				operator=(T *s);
};

/**
 * Reference holder with explicit auto-reference
 *
 * This holder extends basic holder by noauto-reference for constructor
 */
template <typename Type>
class NRef: public Ref<Type>
{
	typedef Ref<Type>		Base;

public:
	template <typename TS>
	DR_MINLINE			NRef(TS *s): Base(s, false)
	{
	}

	DR_MINLINE			~NRef()
	{
		this->obj = NULL;
	}

private:
	template <typename T>
	NRef &				operator=(T *s);
	template <typename T>
	NRef &				operator=(const Ref<T> &s);
};


#if 0
template <typename Type>
class MRef: public Ref<Type>
{
	typedef Ref<Type>		Base;

public:
	DR_MINLINE			MRef():
		Base()
	{
	}

	template <typename TS>
	DR_MINLINE			MRef(TS *s): Base(s, false)
	{
	}

	DR_MINLINE			MRef(Type *s, const Noref &t):
		Base(s, t)
	{
	}

	DR_MINLINE			MRef(Type *s, const Doref &t):
		Base(s, t)
	{
	}

	DR_MINLINE			MRef(const MRef &s):
		Base(s)
	{
	}

	DR_MINLINE			MRef(const Base &s):
		Base(s)
	{
	}

	template <typename TS>
	DR_MINLINE			MRef(const Ref<TS> &s):
		Base(s)
	{
	}
};
#endif

/**
 * Temporary reference
 *
 * create an auto destroy reference for the passed object
 */
template <typename Type>
ERef<Type> tref(Type *o)
{
	return o;
}

/**
 * Temporary reference
 *
 * create an auto destroy reference for the passed object
 */
template <typename Type>
ERef<Type> tref(const Ref<Type> &o)
{
	return o.getDoref();
}

/**
 * Implicit reference
 *
 * create an auto destroy reference for the passed object, referencing it first
 *
 * @note iref makes the object to be out of control so it is safe to use only in
 * case when there is no chance of exception until the object is hold again
 */
template <typename Type>
Type *iref(Type *o)
{
	o->ref();
	return o;
}

/**
 * Implicit reference
 *
 * create an auto destroy reference for the passed object, referencing it first
 */
template <typename Type>
Type *iref(const Ref<Type> &o)
{
	return o.getDoref();
}

/**
 * Move reference
 *
 * create an auto destroy reference for the passed object reference, the
 * original reference is reset
 */
template <typename Type>
ERef<Type> mref(Type **o)
{
	Type *no = *o;
	*o = NULL;
	return no;
}

/**
 * Move reference
 *
 * create an auto destroy reference for the passed object reference, the
 * original reference is reset
 */
template <typename Type>
ERef<Type> mref(const Ref<Type> *o)
{
	return o->getAndNull();
}

/**
 * No reference
 *
 * create a reference holder for the passed object without referencing it
 */
template <typename Type>
NRef<Type> nref(Type *o)
{
	return o;
}


DR_NS_END

#endif
