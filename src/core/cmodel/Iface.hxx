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

#ifndef dr__Iface__hxx__
# define dr__Iface__hxx__

#include <dr/String.hxx>

DR_NS_BEGIN


class Object;
struct Static;


#define DR_IFACE_DECL(name) \
public: \
	virtual DR_NSP(Object) *	ref() const = 0; \
	virtual bool			unref() const = 0; \
 \
	virtual void *			getIface(const DR_NSP(String) &iface) const = 0; \
	virtual void *			getIfaceUnref(const DR_NSP(String) &iface) const = 0; \
 \
	virtual DR_NSP(Object) *	refLiving() const = 0; \
 \
	virtual const DR_NSP(String) &	classname() const = 0; \
 \
	virtual DR_NSP(String)		stringify() const = 0; \
	virtual long			hash() const = 0; \
	virtual bool			eq(const DR_NSP(Iface) *obj2) const = 0; \
 \
public: \
	static const DR_NSP(String)	iface_name; \
protected: \
	static const DR_NSP(Static) *	iface_static; \
public: \
	static const DR_NSP(Static) *	getIfaceStatic()


#define DR_IFACE_DEF(NS_STR, name) \
	const DR_NSP(String)		name::iface_name(DR_NSP(Const)::string(NS_STR #name)); \
	const DR_NSP(Static) *		name::iface_static

#define DR_IFACE_OFFS(mainname, subname) ((char *)(void *)(subname *)(mainname *)1024-(char *)1024)

#define DR_IFACE_IMPL_SIMPLE_getIfaceStatic(ifacename) \
	DR_EXPORT_MET const DR_NSP(Static) *ifacename::getIfaceStatic() \
	{ \
		if (iface_static) \
			return iface_static; \
		return createIfaceStatic(iface_name, &iface_static, 0, NULL); \
	}

#define DR_IFACE_IMPL_IFACE1_getIfaceStatic(ifacename, iface0) \
	DR_EXPORT_MET const DR_NSP(Static) *ifacename::getIfaceStatic() \
	{ \
		if (iface_static) \
			return iface_static; \
		return createIfaceStatic(iface_name, &iface_static, DR_IFACE_OFFS(ifacename, iface0), iface0::getIfaceStatic(), 0, NULL); \
	}

#define DR_IFACE_IMPL_IFACE2_getIfaceStatic(ifacename, iface0, iface1) \
	DR_EXPORT_MET const DR_NSP(Static) *ifacename::getIfaceStatic() \
	{ \
		if (iface_static) \
			return iface_static; \
		return createIfaceStatic(iface_name, &iface_static, DR_IFACE_OFFS(ifacename, iface0), iface0::getIfaceStatic(), DR_IFACE_OFFS(ifacename, iface1), iface1::getIfaceStatic(), 0, NULL); \
	}

#define DR_IFACE_IMPL_IFACE3_getIfaceStatic(ifacename, iface0, iface1, iface2) \
	DR_EXPORT_MET const DR_NSP(Static) *ifacename::getIfaceStatic() \
	{ \
		if (iface_static) \
			return iface_static; \
		return createIfaceStatic(iface_name, &iface_static, DR_IFACE_OFFS(ifacename, iface0), iface0::getIfaceStatic(), DR_IFACE_OFFS(ifacename, iface1), iface1::getIfaceStatic(), DR_IFACE_OFFS(ifacename, iface2), iface2::getIfaceStatic(), 0, NULL); \
	}

#define DR_IFACE_IMPL_SIMPLE(ifacename) \
	DR_IFACE_IMPL_SIMPLE_getIfaceStatic(ifacename)

#define DR_IFACE_IMPL_IFACE1(ifacename, iface0) \
	DR_IFACE_IMPL_IFACE1_getIfaceStatic(ifacename, iface0)

#define DR_IFACE_IMPL_IFACE2(ifacename, iface0, iface1) \
	DR_IFACE_IMPL_IFACE2_getIfaceStatic(ifacename, iface0, iface1)

#define DR_IFACE_IMPL_IFACE3(ifacename, iface0, iface1, iface2) \
	DR_IFACE_IMPL_IFACE3_getIfaceStatic(ifacename, iface0, iface1, iface2)


/* this defines DRIF_PAD_VIRT, number of virtual methods that need to be padded
 * for binary compatibility */
#if (defined DR_CXX_GCC)
# if __GNUC__ >= 3 && __GNUC__ < 5
#  define DRIF_PAD_VIRT 1
# elif (defined __GCCE__)
#  define DRIF_PAD_VIRT 1
# else
#  error unknown GCC version __GCCE__
# endif
#elif (defined DR_CXX_VC)
# define DRIF_PAD_VIRT 1
#else
# error unknown compiler
#endif


class SerializeEncoder;
class SerializeDecoder;

/**
 * base for all interfaces
 */
class DR_PUB Iface: public Base
{
protected:
	DR_MINLINE virtual		~Iface()				{}

#if DRIF_PAD_VIRT >= 1
	DR_MINLINE virtual void		dummy_meth_if1()			{}
#endif
#if DRIF_PAD_VIRT >= 2
	DR_MINLINE virtual void		dummy_meth_if2()			{}
#endif

public:
	/**
	 * return the pointer to object, not referencing
	 *
	 * @return object(unref)
	 * 	the pointer to object
	 */
	virtual Object *		acc() const = 0;

	/**
	 * increases references to object
	 *
	 * @return
	 * 	reference to the object
	 */
	virtual Object *		ref() const = 0;

	/**
	 * decreases references to object
	 *
	 * may destroy the object
	 *
	 * @return
	 * 	false if the object was destroyed
	 */
	virtual bool			unref() const = 0;

	/**
	 * get the class name
	 *
	 * @return
	 * 	class name
	 */
	virtual const String &		classname() const = 0;

	/**
	 * gets an interface
	 *
	 * @param iface
	 * 	the requested interface
	 *
	 * @throws CastExcept
	 * 	if the interface cannot be found
	 */
	virtual void *			getIface(const String &iface) const = 0;

	/**
	 * gets an interface and unreferences the original object
	 *
	 * @param iface
	 * 	the requested interface
	 *
	 * @throws CastExcept
	 * 	if the interface cannot be found, the original object is
	 * 	unreferenced in this case
	 */
	virtual void *			getIfaceUnref(const String &iface) const = 0;

	/**
	 * accesses an interface
	 *
	 * @param iface
	 * 	the requested interface
	 *
	 * @return NULL
	 * 	if the interface cannot be found
	 * @return interface(unref)
	 * 	otherwise
	 */
	virtual void *			getCheckIface(const String &iface) const = 0;

	/**
	 * return address of the object if it's final instance of requested type
	 *
	 * @param name
	 * 	the requested object
	 *
	 * @return NULL
	 * 	if the object is not directly the name instance
	 * @return object(unref)
	 * 	otherwise
	 */
	virtual Object *		accCheckFinal(const String &name) const = 0;

	/**
	 * return address of the object if it's final instance of requested type
	 *
	 * @param name
	 * 	the requested object
	 *
	 * @return NULL
	 * 	if the object is not directly the name instance
	 * @return object
	 * 	otherwise
	 */
	virtual Object *		getCheckFinal(const String &name) const = 0;

	/**
	 * get the object hash
	 *
	 * @return
	 * 	the object hashed value
	 */
	virtual long			hash() const = 0;

	/**
	 * compare the object to another one for equality
	 *
	 * @return
	 * 	the equality of the objects
	 */
	virtual bool			eq(const Iface *obj2) const = 0;

	/**
	 * compare the object to another one for lower/greater
	 *
	 * @return -1
	 * 	if the current object is lower than argument
	 * @return 0
	 * 	if the current object is equal to argument
	 * @return 1
	 * 	if the current object is greater than argument
	 */
	virtual int			cmp(const Iface *obj2) const = 0;

	/**
	 * get the object key hash
	 *
	 * @return
	 * 	the object key hashed value
	 */
	virtual long			keyHash(int key) const = 0;

	/**
	 * compare the object key to another one for equality
	 *
	 * @return
	 * 	the equality of the objects
	 */
	virtual bool			keyEq(const Iface *obj2, int key) const = 0;

	/**
	 * compare the object key to another one for lower/greater
	 *
	 * @return -1
	 * 	if the current object is lower than argument
	 * @return 0
	 * 	if the current object is equal to argument
	 * @return 1
	 * 	if the current object is greater than argument
	 */
	virtual int			keyCmp(const Iface *obj2, int key) const = 0;

	/**
	 * returns state of the object
	 *
	 * @return true
	 * 	if the object is currently destroyed
	 * @return false
	 * 	if the object is not destroyed
	 */
	virtual Object *		refLiving() const = 0;

	/**
	 * get the string describing the object
	 *
	 * @return
	 * 	string describing the object
	 */
	virtual String			stringify() const = 0;

	/**
	 * stores object into stream
	 *
	 * @param stream
	 * 	stream to store the data to
	 */
	virtual void			serializeTo(SerializeEncoder *stream) const = 0;

	/**
	 * loads object from serialized stream
	 * 
	 * @param stream
	 * 	stream to load the data from
	 */
	virtual void			unserializeFrom(SerializeDecoder *stream) = 0;

	/**
	 * reserved function
	 */
	virtual void			iface_reserved_func();

protected:
	/**
	 * internal function which constructs static interface information
	 *
	 * @param output
	 * 	where the information should be stored
	 * @param iface_name
	 * 	this interface name
	 * @param ...
	 * 	other supported interfaces, may consist of:
	 * 		(non-negative ptrdiff_t, Static pointer):
	 * 			includes passed interface on an offset
	 * 		(negative ptrdiff_t, iface_name, function):
	 *			includes dynamic iface_name, calling function
	 *
	 * @return
	 * 	static object information
	 */
	static const Static *		createIfaceStatic(const String &iface_name, const Static **output, ...);

private:
	friend class Object;
	friend struct Static;
};


DR_NS_END

#endif
