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
#include <dr/MsgSync.hxx>

DR_NS_BEGIN


DR_OBJECT_DEF(DR_NS_STR, MsgSync, Object);

DR_EXPORT_MET const Static *MsgSync::getObjectStatic() const
{
	String comp_name_local(Const::string(DR_NS_STRP("MsgSync")));
	if (comp_static)
		return comp_static;
	return createObjectStatic(comp_name_local, &comp_static, Super::getObjectStatic(), 0, NULL);
}


DR_EXPORT_MET MsgSync::MsgSync(Thread *thread_):
	DR_OBJECT_INIT(MsgSync, Object, ()),
	thread(thread_)
{
	return;
}

DR_EXPORT_MET bool MsgSync::threadXchg(MsgSync *new_sync)
{
	return true;
}


DR_NS_END
