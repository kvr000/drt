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

#ifndef dr__io__NetAddressInet4__hxx__
# define dr__io__NetAddressInet4__hxx__

#include <dr/io/NetAddress.hxx>

DR_IO_NS_BEGIN


/**
 * Internet a.b.c.d network address
 */
class DR_IO_PUB NetAddressInet4: public NetAddress
{
	DR_OBJECT_DECL_SIMPLE(NetAddressInet4, NetAddress);

public:
	DR_CONSTRUCT			NetAddressInet4(Uint32 host_, int port_);
	DR_CONSTRUCT			NetAddressInet4(const String &host, int port_);
	DR_CONSTRUCT			NetAddressInet4(const String &full_addr);

public:
	String				formatAddress();

public:
	Uint16				getPort();
	Uint32				getHost();

protected:
	Uint32				host;
	Uint16				port;
};


DR_RINLINE NetAddressInet4::NetAddressInet4(Uint32 host_, int port_):
	host(host_),
	port(port_)
{
}

DR_RINLINE Uint32 NetAddressInet4::getHost()
{
	return host;
}

DR_RINLINE Uint16 NetAddressInet4::getPort()
{
	return port;
}


DR_IO_NS_END

#endif
