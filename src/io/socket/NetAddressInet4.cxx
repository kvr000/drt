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
 * include: 	dr/io/NetAddressInet.hxx
 * ns:		dr::io
 */

#include <dr/x_kw.hxx>

#include <dr/InvalidFormatException.hxx>
#include <dr/io/dev/Socket_sysiface.hxx>

#include <dr/io/NetAddressInet4.hxx>
#include "_gen/NetAddressInet4-all.hxx"

DR_IO_NS_BEGIN


/*drt
 * class:	NetAddressInet4
 * ancestor:	dr::io::NetAddressInet
 *
 * at:	Uint32				host;
 *
 * doc:{
 * 	Holder of generic values
 * }doc
 */

DR_MET(public)
NetAddressInet4::NetAddressInet4(const String &host_str, int port_):
	Super(port_),
	host(Socket_sysiface::lookupInet4Host(host_str))
{
}

DR_MET(public)
NetAddressInet4::NetAddressInet4(Uint32 host_, int port_):
	Super(port_),
	host(host_)
{
}

DR_MET(public)
NetAddressInet4::NetAddressInet4(const String &addr_str):
	Super(0)
{
	ssize_t p;
	if ((p = addr_str.rfind(":")) < 0) {
		xthrownew(InvalidFormatException("inet address", addr_str));
	}
	else {
		host = Socket_sysiface::lookupInet4Host(addr_str.left(p));
		BString sport = addr_str.mid(p+1).utf8();
		char *portend;
		port = (Uint16)strtol(sport, &portend, 0);
		for (; isspace(*portend); portend++) ;
		if (*portend != '\0') {
			xthrownew(InvalidFormatException("inet address", addr_str));
		}
	}
}

DR_MET(public virtual)
String NetAddressInet4::formatAddress()
{
	String faddr("inet4://");
	faddr.appendNumber((Uint8)(host>>24)).append(".").appendNumber((Uint8)(host>>16)).append(".").appendNumber((Uint8)(host>>8)).append(".").appendNumber((Uint8)(host)).append(":").appendNumber(port);
	return faddr;
}

DR_MET(public virtual)
Uint32 NetAddressInet4::getHost()
{
	return host;
}

DR_MET(public virtual)
String NetAddressInet4::getHostString()
{
	String afmt;
	afmt.appendNumber((Uint8)(host>>24)).append(".").appendNumber((Uint8)(host>>16)).append(".").appendNumber((Uint8)(host>>8)).append(".").appendNumber((Uint8)(host));
	return afmt;
}


DR_IO_NS_END
