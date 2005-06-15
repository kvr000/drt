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
 * include:	dr/tenv/def.hxx
 * include: 	dr/Object.hxx
 * ns:		dr::tenv
 * class:	TestIdObject
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

#include "_gen/TestIdObject-all.hxx"

DR_TENV_NS_BEGIN


Refcnt TestIdObject::living_count = 0;

DR_MET(public)
TestIdObject::TestIdObject(int id_):
	id(id_),
	value(DR_STR(unknown))
{
	living_count++;
}

DR_MET(public)
TestIdObject::TestIdObject(const String &value_):
	value(value_)
{
	living_count++;
}

DR_MET(virtual protected)
TestIdObject::~TestIdObject()
{
	living_count--;
}

DR_MET(public virtual)
int TestIdObject::getId()
{
	return id;
}

DR_MET(public virtual)
String TestIdObject::getValue()
{
	return value;
}

DR_MET(public static)
int TestIdObject::countLiving()
{
	return living_count;
}

DR_MET(public static)
TestIdObject *TestIdObject::createInstance()
{
	return new TestIdObject(0);
}

DR_MET(public virtual)
int TestIdObject::cmp(const Iface *second) const
{
	if (TestIdObject *seco = (TestIdObject *)second->accCheckFinal(comp_name)) {
		return id == seco->id ? 0 : id < seco->id ? -1 : 1;
	}
	else {
		return 0;
	}
}

DR_MET(public virtual)
bool TestIdObject::eq(const Iface *second) const
{
	if (TestIdObject *seco = (TestIdObject *)second->accCheckFinal(comp_name)) {
		return id == seco->id;
	}
	else {
		return false;
	}
}

DR_MET(public virtual)
long TestIdObject::hash() const
{
	return id;
}


DR_TENV_NS_END
