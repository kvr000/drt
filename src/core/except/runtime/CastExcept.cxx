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
#include <dr/CastExcept.hxx>

DR_NS_BEGIN


DR_THROW_DEF(DR_NS_STR, CastExcept, RuntimeExcept);

DR_THROW_IMPL_SIMPLE(CastExcept);

DR_EXPORT_MET CastExcept::CastExcept(Object *obj_, const String &iface_):
	obj(obj_, (const Noref &)Noref()),
	iface(iface_)
{
}

DR_EXPORT_MET void CastExcept::throwInstance(Object *obj_, const String &iface_)
{
	DR_THROWNEW(CastExcept(obj_, iface_));
}

DR_EXPORT_MET String CastExcept::stringify() const
{
	return String("cast exception: ").append(obj->classname()).append("(").appendPointer((Object *)obj).append(")=>").append(iface);
}


DR_NS_END
