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

#include <dr/Thread.hxx>
#include <dr/dev/ThreadDummy.hxx>

DR_NS_BEGIN


DR_OBJECT_DEF(DR_NS_STR, ThreadDummy, Thread);

//DR_OBJECT_IMPL_SIMPLE(ThreadDummy);
DR_EXPORT_MET const Static *ThreadDummy::getObjectStatic() const
{
	if (comp_static)
		return comp_static;
	return createObjectStatic(Const::string(DR_NS_STRP("ThreadDummy")), &comp_static, Super::getObjectStatic(), 0, NULL);
}

void *ThreadDummy::run()
{
	DR_AssertInvalid();
	return NULL;
}


DR_NS_END
