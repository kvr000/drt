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

#ifndef dr__Object__hxx__
# define dr__Object__hxx__

#include <dr/def_core.hxx>
#include <dr/Atomic.hxx>
#include <dr/Iface.hxx>

DR_NS_BEGIN


class GroupRef;
class WeakRef_g;
template <typename T> class WeakRef;


#ifdef DR_IS_DOX

#define DR_METHOD_REDIR0(to, rettype, name)
#define DR_METHOD_REDIR1(to, rettype, name, A1)
#define DR_METHOD_REDIR2(to, rettype, name, A1, A2)
#define DR_METHOD_REDIR3(to, rettype, name, A1, A2, A3)
#define DR_METHOD_REDIR4(to, rettype, name, A1, A2, A3, A4)
#define DR_METHOD_REDIR5(to, rettype, name, A1, A2, A3, A4, A5)

#define DR_METHOD_REDIR0C(to, rettype, name)
#define DR_METHOD_REDIR1C(to, rettype, name, A1)
#define DR_METHOD_REDIR2C(to, rettype, name, A1, A2)
#define DR_METHOD_REDIR3C(to, rettype, name, A1, A2, A3)
#define DR_METHOD_REDIR4C(to, rettype, name, A1, A2, A3, A4)
#define DR_METHOD_REDIR5C(to, rettype, name, A1, A2, A3, A4, A5)

#define DR_NODOX(cont)

#else

#define DR_METHOD_REDIR0(to, rettype, name) \
	virtual rettype			name()					{ return to::name(); }
#define DR_METHOD_REDIR1(to, rettype, name, A1) \
	virtual rettype			name(A1 a1)				{ return to::name(a1); }
#define DR_METHOD_REDIR2(to, rettype, name, A1, A2) \
	virtual rettype			name(A1 a1, A2 a2)			{ return to::name(a1, a2); }
#define DR_METHOD_REDIR3(to, rettype, name, A1, A2, A3) \
	virtual rettype			name(A1 a1, A2 a2, A3 a3)		{ return to::name(a1, a2, a3); }
#define DR_METHOD_REDIR4(to, rettype, name, A1, A2, A3, A4) \
	virtual rettype			name(A1 a1, A2 a2, A3 a3, A4 a4)	{ return to::name(a1, a2, a3, a4); }
#define DR_METHOD_REDIR5(to, rettype, name, A1, A2, A3, A4, A5) \
	virtual rettype			name(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)	{ return to::name(a1, a2, a3, a4, a5); }

#define DR_METHOD_REDIR0C(to, rettype, name) \
	virtual rettype			name() const				{ return to::name(); }
#define DR_METHOD_REDIR1C(to, rettype, name, A1) \
	virtual rettype			name(A1 a1) const			{ return to::name(a1); }
#define DR_METHOD_REDIR2C(to, rettype, name, A1, A2) \
	virtual rettype			name(A1 a1, A2 a2) const		{ return to::name(a1, a2); }
#define DR_METHOD_REDIR3C(to, rettype, name, A1, A2, A3) \
	virtual rettype			name(A1 a1, A2 a2, A3 a3) const		{ return to::name(a1, a2, a3); }
#define DR_METHOD_REDIR4C(to, rettype, name, A1, A2, A3, A4) \
	virtual rettype			name(A1 a1, A2 a2, A3 a3, A4 a4) const	{ return to::name(a1, a2, a3, a4); }
#define DR_METHOD_REDIR5C(to, rettype, name, A1, A2, A3, A4, A5) \
	virtual rettype			name(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) const { return to::name(a1, a2, a3, a4, a5); }

#define DR_NODOX(cont)	cont

#endif

#define DR_SUPER_REDIR0(rettype, name) \
	DR_METHOD_REDIR0(Super, rettype, name)
#define DR_SUPER_REDIR1(rettype, name, A1) \
	DR_METHOD_REDIR1(Super, rettype, name, A1)
#define DR_SUPER_REDIR2(rettype, name, A1, A2) \
	DR_METHOD_REDIR2(Super, rettype, name, A1, A2)
#define DR_SUPER_REDIR3(rettype, name, A1, A2, A3) \
	DR_METHOD_REDIR3(Super, rettype, name, A1, A2, A3)
#define DR_SUPER_REDIR4(rettype, name, A1, A2, A3, A4) \
	DR_METHOD_REDIR4(Super, rettype, name, A1, A2, A3, A4)
#define DR_SUPER_REDIR5(rettype, name, A1, A2, A3, A4, A5) \
	DR_METHOD_REDIR5(Super, rettype, name, A1, A2, A3, A4, A5)

#define DR_SUPER_REDIR0C(rettype, name) \
	DR_METHOD_REDIR0C(Super, rettype, name)
#define DR_SUPER_REDIR1C(rettype, name, A1) \
	DR_METHOD_REDIR1C(Super, rettype, name, A1)
#define DR_SUPER_REDIR2C(rettype, name, A1, A2) \
	DR_METHOD_REDIR2C(Super, rettype, name, A1, A2)
#define DR_SUPER_REDIR3C(rettype, name, A1, A2, A3) \
	DR_METHOD_REDIR3C(Super, rettype, name, A1, A2, A3)
#define DR_SUPER_REDIR4C(rettype, name, A1, A2, A3, A4) \
	DR_METHOD_REDIR4C(Super, rettype, name, A1, A2, A3, A4)
#define DR_SUPER_REDIR5C(rettype, name, A1, A2, A3, A4, A5) \
	DR_METHOD_REDIR5C(Super, rettype, name, A1, A2, A3, A4, A5)


#define DR_OBJECT_DECL_SIMPLE(objname, ancestor) \
private: \
	typedef ancestor		Super; \
 \
public: \
	DR_SUPER_REDIR1C(void *, getIface, const DR_NSP(String) &); \
	DR_SUPER_REDIR1C(void *, getIfaceUnref, const DR_NSP(String) &); \
	DR_SUPER_REDIR1C(void *, getCheckIface, const DR_NSP(String) &); \
	DR_SUPER_REDIR1C(Object *, getCheckFinal, const DR_NSP(String) &); \
	DR_SUPER_REDIR1C(Object *, accCheckFinal, const DR_NSP(String) &); \
	virtual const DR_NSP(String) &	classname() const			{ return comp_name; } \
	DR_NODOX(virtual const DR_NSP(Static) *	getObjectStatic() const;) \
 \
public: \
	static const DR_NSP(String)	comp_name; \
private: \
	DR_NODOX(static const DR_NSP(Static) *	comp_static)


#define DR_OBJECT_DECL(objname, ancestor) \
	DR_OBJECT_DECL_SIMPLE(objname, ancestor); \
public: \
	DR_SUPER_REDIR0C(DR_NSP(Object) *, acc); \
	DR_SUPER_REDIR0C(DR_NSP(Object) *, ref); \
	DR_SUPER_REDIR0C(bool, unref); \
	DR_SUPER_REDIR0C(DR_NSP(Object) *, refLiving); \


#define DR_OBJECT_DECL_MODIF(objname, ancestor) \
	DR_OBJECT_DECL_SIMPLE(objname, ancestor); \


#define DR_REDIR_STRINGIFY() \
public: \
	DR_SUPER_REDIR0C(DR_NSP(String), stringify)

#define DR_REDIR_SERIALIZE() \
public: \
	DR_SUPER_REDIR1C(void, serializeTo, DR_NSP(SerializeEncoder) *);	\
	DR_SUPER_REDIR1(void, unserializeFrom, DR_NSP(SerializeDecoder) *)

#define DR_REDIR_CMP() \
public: \
	DR_SUPER_REDIR0C(long, hash); \
	DR_SUPER_REDIR1C(bool, eq, const DR_NSP(Iface) *); \
	DR_SUPER_REDIR1C(int, cmp, const DR_NSP(Iface) *); \
	DR_SUPER_REDIR1C(long, keyHash, int); \
	DR_SUPER_REDIR2C(bool, keyEq, const DR_NSP(Iface) *, int); \
	DR_SUPER_REDIR2C(int, keyCmp, const DR_NSP(Iface) *, int)

#define DR_REDIR_BEHAV()	\
	DR_REDIR_STRINGIFY();	\
	DR_REDIR_CMP();		\
	DR_REDIR_SERIALIZE()


#define DR_OBJECT_DEF(NS_STR, objname, ancestor) \
	DR_EXPORT_DTS const DR_NSP(String)	objname::comp_name(DR_NSP(Const)::string(NS_STR #objname)); \
	DR_NODOX(DR_EXPORT_DTS const DR_NSP(Static) *	objname::comp_static)


#define DR_OBJECT_IMPL_SIMPLE_getObjectStatic(objname) \
	DR_EXPORT_MET const DR_NSP(Static) *objname::getObjectStatic() const \
	{ \
		if (comp_static) \
			return comp_static; \
		return createObjectStatic(comp_name, &comp_static, Super::getObjectStatic(), 0, NULL); \
	}

#define DR_OBJECT_IMPL_IFACE1_getObjectStatic(objname, iface0) \
	DR_EXPORT_MET const DR_NSP(Static) *objname::getObjectStatic() const \
	{ \
		if (comp_static) \
			return comp_static; \
		return createObjectStatic(comp_name, &comp_static, Super::getObjectStatic(), (char *)(void *)(iface0 *)this-(char *)(void *)this, iface0::getIfaceStatic(), 0, NULL); \
	}

#define DR_OBJECT_IMPL_IFACE2_getObjectStatic(objname, iface0, iface1) \
	DR_EXPORT_MET const DR_NSP(Static) *objname::getObjectStatic() const \
	{ \
		if (comp_static) \
			return comp_static; \
		return createObjectStatic(comp_name, &comp_static, Super::getObjectStatic(), (char *)(void *)(iface0 *)this-(char *)(void *)this, iface0::getIfaceStatic(), (char *)(void *)(iface1 *)this-(char *)(void *)this, iface1::getIfaceStatic(), 0, NULL); \
	}

#define DR_OBJECT_IMPL_IFACE3_getObjectStatic(objname, iface0, iface1, iface2) const \
	DR_EXPORT_MET const DR_NSP(Static) *objname::getObjectStatic() \
	{ \
		if (comp_static) \
			return comp_static; \
		return createObjectStatic(comp_name, &comp_static, Super::getObjectStatic(), (char *)(void *)(iface0 *)this-(char *)(void *)this, iface0::getIfaceStatic(), (char *)(void *)(iface1 *)this-(char *)(void *)this, iface1::getIfaceStatic(), (char *)(void *)(iface2 *)this-(char *)(void *)this, iface1::getIfaceStatic(), 0, NULL); \
	}


#define DR_OBJECT_IMPL_SIMPLE(objname) \
	DR_OBJECT_IMPL_SIMPLE_getObjectStatic(objname)

#define DR_OBJECT_IMPL_IFACE0(objname) \
	DR_OBJECT_IMPL_SIMPLE_getObjectStatic(objname)

#define DR_OBJECT_IMPL_IFACE1(objname, iface0) \
	DR_OBJECT_IMPL_IFACE1_getObjectStatic(objname, iface0)

#define DR_OBJECT_IMPL_IFACE2(objname, iface0, iface1) \
	DR_OBJECT_IMPL_IFACE2_getObjectStatic(objname, iface0, iface1)

#define DR_OBJECT_IMPL_IFACE3(objname, iface0, iface1, iface2) \
	DR_OBJECT_IMPL_IFACE3_getObjectStatic(objname, iface0, iface1, iface2)


#define DR_OBJECT_INIT(objname, ancestor, args) \
	Super args


class Throwable;

/**
 * base for all objects
 */
class DR_PUB Object: public Iface
{
public:		/* basic */
	/**
	 * @see Iface::acc
	 */
	virtual Object *		acc() const				{ return const_cast<Object *>(this); }

	/**
	 * @see Iface::ref
	 */
	virtual Object *		ref() const				{ DR_LOG4(("%s: referencing %p, new refcnt %d+1\n", DR_FUNCTION, this, refcnt+1)); if (!Atomic::incr(&refcnt)) { const_cast<Object *>(this)->rerefed(); } return const_cast<Object *>(this); }

	/**
	 * @see Iface::unref
	 */
	virtual bool			unref() const				{ DR_LOG4(("%s: unreferencing %p, new refcnt %d+1\n", DR_FUNCTION, this, refcnt-1)); if (!Atomic::decr(&refcnt)) { return const_cast<Object *>(this)->unrefed(); } return true; }

	/**
	 * @see Iface::getIface
	 */
	virtual void *			getIface(const String &iface) const;

	/**
	 * @see Iface::getIfaceUnref
	 */
	virtual void *			getIfaceUnref(const String &iface) const;

	/**
	 * @see Iface::getCheckIface
	 */
	virtual void *			getCheckIface(const String &iface) const;

	/**
	 * @see Iface::accCheckFinal
	 */
	virtual Object *		accCheckFinal(const String &name) const;

	/**
	 * @see Iface::getCheckFinal
	 */
	virtual Object *		getCheckFinal(const String &name) const;

	/**
	 * @see Iface::refLiving()
	 */
	virtual Object *		refLiving() const			{ return refcnt < 0 ? NULL : ref(); }

	/**
	 * @see Iface::classname
	 */
	virtual const String &		classname() const			{ return comp_name; }

	/**
	 * @see Iface::stringify
	 */
	virtual String			stringify() const;

	/**
	 * @see Iface::hash
	 */
	virtual long			hash() const;

	/**
	 * @see Iface::eq
	 */
	virtual bool			eq(const Iface *obj) const;

	/**
	 * @see Iface::cmp
	 */
	virtual int			cmp(const Iface *obj) const;

	/**
	 * @see Iface::keyHash
	 */
	virtual long			keyHash(int key) const			{ return hash(); }

	/**
	 * @see Iface::keyEq
	 */
	virtual bool			keyEq(const Iface *obj, int key) const	{ return eq(obj); }

	/**
	 * @see Iface::keyCmp
	 */
	virtual int			keyCmp(const Iface *obj, int key) const	{ return cmp(obj); }

	/**
	 * @see Iface::serializeTo
	 */
	virtual void			serializeTo(SerializeEncoder *stream) const;

	/**
	 * @see Iface::unserializeFrom
	 */
	virtual void			unserializeFrom(SerializeDecoder *stream);

private: /* implementation static data */
	virtual void			object_reserved_static();
	virtual void			object_reserved_1();
	virtual void			object_reserved_2();
	enum {
		OVD_IFACE_DEF = 21,
		OVD_SECOND,
		OVD_THIRD,
	};

public:
	/**
	 * contains component name
	 */
	static const String		comp_name;

	/**
	 * contains static object data
	 */
	static const Static *		comp_static;

protected:
	/**
	 * object runtime information
	 */
	struct ObjectInfo
	{
		GroupRef *		group_ref;
		WeakRef_g **		weak_iface_refs;
	};
	
protected:
	/**
	 * reference count
	 * 0 means one reference, once coming negative, the object is destroyed
	 */
	mutable Refcnt			refcnt;

	/**
	 * runtime information
	 */
	mutable struct ObjectInfo *	core_data;

	/**
	 * constructor
	 */
	DR_CONSTRUCT			Object();

	/**
	 * no initialization constructor
	 */
	DR_CONSTRUCT			Object(const None &);

protected:
	/**
	 * returns object static information
	 *
	 * Must be overriden by inherited objects.
	 * It is done automatically by dr macros
	 *
	 * @return			object static information
	 */
	virtual const Static *		getObjectStatic() const;

	/**
	 * unrefed event
	 *
	 * Called when the object itself is no longer referenced.
	 * Note that the order of calling unrefed() and rerefed() is not
	 * guaranteed.
	 *
	 * @return false
	 * 	when object is destroyed
	 * @return true
	 * 	when object still exists
	 *
	 * @note
	 * 	it's recommended to not override this functionality
	 */
	virtual bool			unrefed();

	/**
	 * re-refed event
	 *
	 * Called when the object itself is referenced again.
	 * Note that the order of calling unrefed() and rerefed() is not
	 * guaranteed.
	 *
	 * @note
	 * 	it's recommended to not override this functionality
	 */
	virtual void			rerefed();

	/**
	 * destroy event
	 *
	 * Called when the object is going to be destroyed
	 *
	 * @note
	 * 	the default functionality calls destructor
	 */
	virtual void			destroy();

	/**
	 * destructor
	 */
	virtual				~Object();

protected:	/* generic support functions */
	static const Static *		createObjectStatic(const String &comp_name_, const Static **output, const Static *parent, ...);

protected:
	static ObjectInfo		default_oi;

protected:
	friend class Iface;
};


DR_NS_END

#endif
