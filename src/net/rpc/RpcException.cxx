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

#include <dr/net/RpcException.hxx>

DR_NET_NS_BEGIN


DR_THROW_DEF(DR_NET_NS_STR, RpcException, Exception);
DR_THROW_IMPL_SIMPLE(RpcException);

DR_EXPORT_MET RpcException::RpcException(const String &description_):
	description(description_)
{
}

DR_EXPORT_MET RpcException::RpcException(Throwable *previous_, const String &description_):
	Exception(previous_),
	description(description_)
{
}

DR_EXPORT_MET String RpcException::stringify() const
{
	String desc("rpc exception: ");
	desc.append(description);
	if (cause) {
		desc.append(": ");
		desc.append(cause->stringify());
	}
	return desc;
}


DR_NET_NS_END
