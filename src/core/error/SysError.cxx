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

#include <string.h>
#include <stdio.h>

#include <dr/Const.hxx>
#include <dr/SysError.hxx>

#ifdef DR_CXX_VC
static char errbuf[16];
# define strerror(x) ((char *)sprintf(errbuf, "error %d", x), errbuf)
#endif

DR_NS_BEGIN


DR_THROW_DEF(DR_NS_STR, SysError, Error);

DR_THROW_IMPL_SIMPLE(SysError);

DR_EXPORT_MET SysError::SysError(int sys_error_):
	DR_THROW_INIT(SysError, Error, ()),
	sys_error(sys_error_)
{
}

DR_EXPORT_MET String SysError::stringify() const
{
	String desc("unexpected system error occured: (");
	desc.appendNumber(sys_error).append(") ").append(strerror(sys_error));
	return desc;
}

DR_EXPORT_MET int SysError::getSysError() const
{
	return sys_error;
}


DR_NS_END
