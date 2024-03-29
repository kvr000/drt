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
 *
 * ns:		dr::tenv
 */

#include <dr/Const.hxx>
#include <dr/Ref.hxx>

#include "_gen/TenvObject-all.hxx"

DR_TENV_NS_BEGIN


/*drt
 * class:	TenvObject
 * ancestor:	dr::Object
 * ifaces:	
 */

DR_MET(public)
TenvObject::TenvObject()
{
}

DR_MET(protected virtual)
TenvObject::~TenvObject()
{
}

DR_MET(public virtual abstract)
//void TenvObject::test(const String &arg = DR_INIT(Null()))


DR_TENV_NS_END
