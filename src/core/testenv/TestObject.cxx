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

/*drt
 * include:	dr/testenv/def_testenv.hxx
 * include: 	dr/Object.hxx
 * ns:		dr::testenv
 * class:	TestObject
 * ancestor:	dr::Object
 * ifaces:	
 * at:	int				id;
 * at:	String				value;
 * at:	static Refcnt			living_count;
 *
 * use:		dr
 */

#include <dr/Const.hxx>
#include <dr/Ref.hxx>

#include <dr/testenv/TestObject.hxx>

#include "_gen/TestObject-def.hxx"

DR_TESTENV_NS_BEGIN


Refcnt TestObject::living_count = 0;

DR_MET(public)
TestObject::TestObject(int id_):
	id(id_)
{
	living_count++;
}

DR_MET(public)
TestObject::TestObject(const String &value_):
	value(value_)
{
	living_count++;
}

DR_MET(virtual protected)
TestObject::~TestObject()
{
	living_count--;
}

DR_MET(public virtual)
int TestObject::getId()
{
	return id;
}

DR_MET(public virtual)
String TestObject::getValue()
{
	return value;
}

DR_MET(public static)
int TestObject::countLiving()
{
	return living_count;
}

DR_MET(public virtual)
int TestObject::cmp(const Iface *second) const
{
	ERef<Object> obj(second->ref());
	if (obj->classname() == comp_name) {
		int si = ((const TestObject *)*obj)->id;
		return id == si ? 0 : id < si ? -1 : 1;
	}
	return this == *obj ? 0 : this < *obj ? -1 : 1;
}

DR_MET(public virtual)
bool TestObject::eq(const Iface *second) const
{
	ERef<Object> obj(second->ref());
	return obj->classname() == comp_name && ((TestObject *)*obj)->id == id;
}

DR_MET(public virtual)
long TestObject::hash() const
{
	return id;
}


DR_TESTENV_NS_END
