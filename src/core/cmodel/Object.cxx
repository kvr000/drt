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

#include <dr/emits.hxx>
#include <dr/Const.hxx>
#include <dr/x_kw.hxx>

#include <dr/Object.hxx>

#include <dr/Static.hxx>
#include <dr/GroupRef.hxx>
#include <dr/WeakRef.hxx>
#include <dr/String.hxx>
#include <dr/CastExcept.hxx>
#include <dr/UnsupportedExcept.hxx>

DR_NS_BEGIN


#ifdef DR_RO_VTABLE

namespace sys
{
	int makeMemWritable(void *addr, size_t size);
};

#endif


DR_EXPORT_DTS const String Object::comp_name(Const::string(DR_NS_STRP("Object")));
DR_EXPORT_DTS const Static *Object::comp_static = NULL;


static WeakRef_g *default_weak_iface_refs[256];
DR_EXPORT_DTS Object::ObjectInfo Object::default_oi = {
	NULL,
	default_weak_iface_refs,
};

Object::Object():
	core_data(&default_oi),
	refcnt(0)
{
}

Object::~Object()
{
}

bool Object::unrefed()
{
	if (core_data->group_ref != NULL) {
		return core_data->group_ref->unref();
	}
	try {
		destroy();
	}
	catch (Throwable *ex) {
		if (Throwable::throw_overactive()) {
			Throwable::throw_inactivate_destroy(ex);
		}
		else {
			throw;
		}
	}
	return false;
}

void Object::rerefed()
{
	if (core_data->group_ref != NULL)
		core_data->group_ref->ref();
}

void Object::destroy()
{
	if (core_data != &default_oi) {
		DR_Assert(core_data->group_ref == NULL);
		core_data = &default_oi;
	}
	delete this;
}

void *Object::getIface(const String &iface) const
{
	StrData *ifd = iface.d;
	long ihash = ifd->hash >= 0 ? ifd->hash : ifd->updateHash();
	const Static *objst;
	Static::IfaceNode *node;

#ifndef DR_NO_ALIGNED_FUNC
	if (!((UintPtr)(objst = (*(const Static ***)this)[OVD_IFACE_DEF])&1)) {
		objst = getObjectStatic();
#ifndef DR_RO_VTABLE
		(*(const Static ***)this)[OVD_IFACE_DEF] = (const Static *)((UintPtr)objst+1);
#elif 1
		sys::makeMemWritable(&(*(const Static ***)this)[OVD_IFACE_DEF], sizeof(void *));
		(*(const Static ***)this)[OVD_IFACE_DEF] = (const Static *)((UintPtr)objst+1);
#endif
	}
	else {
		objst = (Static *)((UintPtr)objst-1);
	}
#else
	{
		objst = getObjectStatic();
	}
#endif
	for (node = objst->ifaces[ihash%(sizeof(objst->ifaces)/sizeof(objst->ifaces[0]))]; node; node = node->next) {
		if (node->name.d == ifd)
			goto found;
	}
	for (node = objst->ifaces[ihash%(sizeof(objst->ifaces)/sizeof(objst->ifaces[0]))]; node; node = node->next) {
		if (node->name.d->hash != ifd->hash)
			continue;
		if (node->name.d->beq(ifd))
			goto found;
	}
	CastExcept::throwInstance(ref(), iface);

found:
	SintPtr offs = node->offs;
	if (offs >= 0) {
		Atomic::inc(&refcnt);
		//Object::ref();
		return (char *)(UintPtr)this+offs;
	}
	else {
		if (void *ret = node->create_func(const_cast<Object *>(this), offs))
			return ret;
		CastExcept::throwInstance(ref(), iface);
		return NULL;
	}
}

void *Object::getIfaceUnref(const String &iface) const
{
	StrData *ifd = iface.d;
	long ihash = ifd->hash >= 0 ? ifd->hash : ifd->updateHash();
	const Static *objst;
	Static::IfaceNode *node;

#ifndef DR_NO_ALIGNED_FUNC
	if (!((UintPtr)(objst = (*(const Static ***)this)[OVD_IFACE_DEF])&1)) {
		objst = getObjectStatic();
#ifndef DR_RO_VTABLE
		(*(const Static ***)this)[OVD_IFACE_DEF] = (const Static *)((UintPtr)objst+1);
#elif 1
		sys::makeMemWritable(&(*(const Static ***)this)[OVD_IFACE_DEF], sizeof(void *));
		(*(const Static ***)this)[OVD_IFACE_DEF] = (const Static *)((UintPtr)objst+1);
#endif
	}
	else {
		objst = (Static *)((UintPtr)objst-1);
	}
#else
	{
		objst = getObjectStatic();
	}
#endif
	for (node = objst->ifaces[ihash%(sizeof(objst->ifaces)/sizeof(objst->ifaces[0]))]; node; node = node->next) {
		if (node->name.d == ifd)
			goto found;
	}
	for (node = objst->ifaces[ihash%(sizeof(objst->ifaces)/sizeof(objst->ifaces[0]))]; node; node = node->next) {
		if (node->name.d->hash != ifd->hash)
			continue;
		if (node->name.d->beq(ifd))
			goto found;
	}
	CastExcept::throwInstance(const_cast<Object *>(this), iface);

found:
	SintPtr offs = node->offs;
	if (offs >= 0) {
		return (char *)(UintPtr)this+offs;
	}
	else {
		DR_TRY {
			if (void *ret = node->create_func(const_cast<Object *>(this), offs)) {
				DR_TRY {
					unref();
				}
				DR_CATCHANY {
					((Iface *)ret)->unref();
					DR_RETHROWANY;
				}
				DR_ENDTRY;
				return ret;
			}
			else {
				CastExcept::throwInstance(const_cast<Object *>(this), iface);
				return NULL;
			}
		}
		DR_CATCHANY {
			unref();
			DR_RETHROWANY;
		}
		DR_ENDTRY;
	}
}

String Object::stringify() const
{
	String desc;
	desc.append(classname()).append("@").appendPointer(this);
	return desc;
}

long Object::hash() const
{
	return (long)this;
}

bool Object::eq(const Iface *obj2_) const
{
	ERef<Object> obj2((Object *)obj2_->getIface(Object::comp_name));
	return obj2 == this;
}

int Object::cmp(const Iface *obj2_) const
{
	ERef<Object> obj2((Object *)obj2_->ref());
	return this == obj2 ? 0 : (char *)(void *)this < (char *)(void *)obj2 ? -1 : 1;
}

void Object::serializeTo(SerializeEncoder *serializer) const
{
	xthrownew(UnsupportedExcept(this, classname(), "serialize", Null()));
}

void Object::unserializeFrom(SerializeDecoder *serializer)
{
	xthrownew(UnsupportedExcept(this, classname(), "unserialize", Null()));
}

void Object::dummy_func_static()
{
}

void Object::dummy_func_second()
{
}

void Object::dummy_func_third()
{
}

const Static *Object::getObjectStatic() const
{
	if (comp_static)
		return comp_static;

	Static *os = Static::create(Const::string(DR_NS_STRP("Object")), 1);
	Static::IfaceNode *free_node = (Static::IfaceNode *)(void *)(os+1);
	os->addNode(&free_node, comp_name, 0, NULL);

	if (!Atomic::cmpxchg((void **)(void *)&comp_static, NULL, os))
		os->free();
	return comp_static;
}

const Static *Object::createObjectStatic(const String &cname, const Static **out, const Static *parent,...)
{
	size_t count;
	va_list args;
	Static *os;
	Static::IfaceNode *free_node;

	if (*out)
		return *out;

	count = 1;
	count += parent->getCount();
	va_start(args, parent);
	for (;;) {
		SintPtr offs = va_arg(args, SintPtr);
		if (offs >= 0) {
			const Static *defs = va_arg(args, const Static *);
			if (!defs)
				break;
			count += defs->getCount();
		}
		else if (offs < 0) {
			va_arg(args, String *);
			va_arg(args, void *); // removed due to buggy VC: void *(*)(Object *, SintPtr)
			count++;
		}
	}
	va_end(args);

	os = Static::create(cname, count);
	free_node = (Static::IfaceNode *)(void *)(os+1);

	os->addNode(&free_node, cname, 0, NULL);
	os->merge(&free_node, 0, parent);
	va_start(args, parent);
	for (;;) {
		SintPtr offs = va_arg(args, SintPtr);
		if (offs >= 0) {
			Static *defs = va_arg(args, Static *);
			if (!defs)
				break;
			os->merge(&free_node, offs, defs);
		}
		else if (offs < 0) {
			String *if_name = va_arg(args, String *);
			void *(*create_func)(Object *, SintPtr) = (void *(*)(Object *, SintPtr))va_arg(args, void *);
			os->addNode(&free_node, *if_name, offs, create_func);
		}
	}
	va_end(args);

	if (!Atomic::cmpxchg((void **)out, NULL, os))
		os->free();

	return *out;
}


DR_NS_END
