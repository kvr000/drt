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
 * include: 	dr/Object.hxx
 * ns:		dr
 * class:	DummyObject
 * ancestor:	dr::Object
 * ifaces:	
 * at:	static DummyObject *		instance;
 */

#include <dr/Const.hxx>
#include <dr/Ref.hxx>

#include <dr/DummyObject.hxx>

#include "_gen/DummyObject-def.hxx"

DR_NS_BEGIN


DummyObject *DummyObject::instance = NULL;

DR_MET(public)
DummyObject::DummyObject()
{
}

DR_MET(public static)
DummyObject *DummyObject::accInstance()
{
	if (!instance) {
		DummyObject *new_inst = new DummyObject();
		if (!Atomic::cmpxchg((void **)&instance, NULL, new_inst)) {
			new_inst->unref();
		}
	}
	return instance;
}

DR_MET(public static)
DummyObject *DummyObject::getInstance()
{
	return (DummyObject *)accInstance()->ref();
}


DR_NS_END
