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

#include <dr/x_kw.hxx>

#include <dr/InvalidFormatExcept.hxx>
#include <dr/io/dev/Socket_sysiface.hxx>

#include <dr/io/NetAddressInet4.hxx>

DR_IO_NS_BEGIN


DR_OBJECT_DEF(DR_IO_NS_STR, NetAddressInet4, NetAddress);
DR_OBJECT_IMPL_SIMPLE(NetAddressInet4);

NetAddressInet4::NetAddressInet4(const String &host_str, int port_):
	host(Socket_sysiface::lookupInet4Host(host_str)),
	port(port_)
{
}

NetAddressInet4::NetAddressInet4(const String &addr_str)
{
	ssize_t p;
	if ((p = addr_str.rfind(":")) < 0) {
		xthrownew(InvalidFormatExcept("host name", addr_str));
	}
	else {
		host = Socket_sysiface::lookupInet4Host(addr_str.left(p));
		port = (Uint16)atol(addr_str.mid(p+1).utf8().toStr());
	}
}

String NetAddressInet4::formatAddress()
{
	String faddr("inet4://");
	faddr.appendNumber((Uint8)(host>>24)).append(".").appendNumber((Uint8)(host>>16)).append(".").appendNumber((Uint8)(host>>8)).append(".").appendNumber((Uint8)(host)).append(":").appendNumber(port);
	return faddr;
}


DR_IO_NS_END
