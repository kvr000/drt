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

#include <dr/Const.hxx>
#include <dr/Ref.hxx>

#include <dr/testenv/TestObjectKV.hxx>

#include "_gen/TestObjectKV-def.hxx"

/*drt
 * include:	dr/testenv/def_testenv.hxx
 * include: 	dr/Object.hxx
 * ns:		dr::testenv
 */

DR_TESTENV_NS_BEGIN


/*drt
 * class:	TestObjectKV::Key
 * ancestor:	dr::Object
 * ifaces:
 * ap: int				id;
 */

DR_MET(public)
TestObjectKV::Key::Key(int id_):
	id(id_)
{
}


/*drt
 * class:	TestObjectKV
 * ancestor:	dr::Object
 * ifaces:	
 * at:	int				id;
 * at:	String				value;
 * at:	static Refcnt			living_count;
 *
 * use:		dr
 */

Refcnt TestObjectKV::living_count = 0;

DR_MET(public)
TestObjectKV::TestObjectKV(int id_, const String &value_):
	id(id_),
	value(value_)
{
	living_count++;
}

DR_MET(virtual protected)
TestObjectKV::~TestObjectKV()
{
	living_count--;
}

DR_MET(public virtual)
int TestObjectKV::getId()
{
	return id;
}

DR_MET(public virtual)
String TestObjectKV::getValue()
{
	return value;
}

DR_MET(public static)
int TestObjectKV::countLiving()
{
	return living_count;
}

DR_MET(public virtual)
long TestObjectKV::hash() const
{
	return id^value.getHash();
}

DR_MET(public virtual)
bool TestObjectKV::eq(const Iface *second) const
{
	if (TestObjectKV *seco = (TestObjectKV *)second->accCheckFinal(comp_name)) {
		return id == seco->id && value.beq(seco->value);
	}
	else {
		return false;
	}
}

DR_MET(public virtual)
int TestObjectKV::cmp(const Iface *second) const
{
	if (TestObjectKV *seco = (TestObjectKV *)second->accCheckFinal(comp_name)) {
		return id == seco->id ? value.bcmp(seco->value) : id < seco->id ? -1 : 1;
	}
	else {
		return 0;
	}
}

DR_MET(public virtual)
long TestObjectKV::keyHash(int key_id) const
{
	return id;
}

DR_MET(public virtual)
bool TestObjectKV::keyEq(const Iface *second, int key_id) const
{
	if (key_id != 0)
		return Super::keyEq(second, key_id);
	if (TestObjectKV::Key *seck = (TestObjectKV::Key *)second->accCheckFinal(Key::comp_name)) {
		return id == seck->id;
	}
	else if (TestObjectKV *seco = (TestObjectKV *)second->accCheckFinal(comp_name)) {
		return id == seco->id;
	}
	else {
		return false;
	}
}

DR_MET(public virtual)
int TestObjectKV::keyCmp(const Iface *second, int key_id) const
{
	if (key_id != 0)
		return Super::keyCmp(second, key_id);
	if (TestObjectKV::Key *seck = (TestObjectKV::Key *)second->accCheckFinal(Key::comp_name)) {
		return id == seck->id ? 0 : id < seck->id ? -1 : 1;
	}
	else if (TestObjectKV *seco = (TestObjectKV *)second->accCheckFinal(comp_name)) {
		return id == seco->id ? 0 : id < seco->id ? -1 : 1;
	}
	else {
		return false;
	}
}


DR_TESTENV_NS_END
