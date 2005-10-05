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

#include <dr/tenv/TestIdObjectKV.hxx>

#include "_gen/TestIdObjectKV-def.hxx"

/*drt
 * include:	dr/tenv/def.hxx
 * include: 	dr/Object.hxx
 * ns:		dr::tenv
 */

DR_TENV_NS_BEGIN


/*drt
 * class:	TestIdObjectKV::Key
 * ancestor:	dr::Object
 * ifaces:
 *
 * ap: int				id;
 */

DR_MET(public)
TestIdObjectKV::Key::Key(int id_):
	id(id_)
{
}


/*drt
 * class:	TestIdObjectKV
 * ancestor:	dr::Object
 * ifaces:	
 *
 * at:	int				id;
 * at:	String				value;
 * at:	static Refcnt			living_count;
 *
 * use:		dr
 */

Refcnt TestIdObjectKV::living_count = 0;

DR_MET(public)
TestIdObjectKV::TestIdObjectKV(int id_, const String &value_):
	id(id_),
	value(value_)
{
	living_count++;
}

DR_MET(virtual protected)
TestIdObjectKV::~TestIdObjectKV()
{
	living_count--;
}

DR_MET(public virtual)
int TestIdObjectKV::getId()
{
	return id;
}

DR_MET(public virtual)
String TestIdObjectKV::getValue()
{
	return value;
}

DR_MET(public static)
int TestIdObjectKV::countLiving()
{
	return living_count;
}

DR_MET(public virtual)
long TestIdObjectKV::hash() const
{
	return id^value.getHash();
}

DR_MET(public virtual)
bool TestIdObjectKV::eq(const Iface *second) const
{
	if (TestIdObjectKV *seco = (TestIdObjectKV *)second->accCheckFinal(comp_name)) {
		return id == seco->id && value.beq(seco->value);
	}
	else {
		return false;
	}
}

DR_MET(public virtual)
int TestIdObjectKV::cmp(const Iface *second) const
{
	if (TestIdObjectKV *seco = (TestIdObjectKV *)second->accCheckFinal(comp_name)) {
		return id == seco->id ? value.bcmp(seco->value) : id < seco->id ? -1 : 1;
	}
	else {
		return 0;
	}
}

DR_MET(public virtual)
long TestIdObjectKV::hashKey(int key_id) const
{
	return id;
}

DR_MET(public virtual)
bool TestIdObjectKV::eqKey(const Iface *second, int key_id) const
{
	if (key_id != 0)
		return Super::eqKey(second, key_id);
	if (TestIdObjectKV::Key *seck = (TestIdObjectKV::Key *)second->accCheckFinal(Key::comp_name)) {
		return id == seck->id;
	}
	else if (TestIdObjectKV *seco = (TestIdObjectKV *)second->accCheckFinal(comp_name)) {
		return id == seco->id;
	}
	else {
		return false;
	}
}

DR_MET(public virtual)
int TestIdObjectKV::cmpKey(const Iface *second, int key_id) const
{
	if (key_id != 0)
		return Super::cmpKey(second, key_id);
	if (TestIdObjectKV::Key *seck = (TestIdObjectKV::Key *)second->accCheckFinal(Key::comp_name)) {
		return id == seck->id ? 0 : id < seck->id ? -1 : 1;
	}
	else if (TestIdObjectKV *seco = (TestIdObjectKV *)second->accCheckFinal(comp_name)) {
		return id == seco->id ? 0 : id < seco->id ? -1 : 1;
	}
	else {
		return false;
	}
}


DR_TENV_NS_END
