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

#include <dr/io/Socket.hxx>
#include <dr/io/dev/Socket_sysiface.hxx>

DR_IO_NS_BEGIN


DR_OBJECT_DEF(DR_IO_NS_STR, Socket, Handle);
DR_OBJECT_IMPL_SIMPLE(Socket);

DR_EXPORT_DTS const String Socket::domain_local(Const::string("local"));
DR_EXPORT_DTS const String Socket::domain_inet4(Const::string("inet"));
DR_EXPORT_DTS const String Socket::domain_inet6(Const::string("inet6"));

DR_EXPORT_DTS const String Socket::proto_tcp(Const::string("tcp"));
DR_EXPORT_DTS const String Socket::proto_udp(Const::string("udp"));
DR_EXPORT_DTS const String Socket::proto_icmp(Const::string("icmp"));
DR_EXPORT_DTS const String Socket::proto_sctp(Const::string("sctp"));

DR_EXPORT_DTS const String Socket::socket_string(Const::string("socket"));
DR_EXPORT_DTS const String Socket::connect_string(Const::string("connect"));
DR_EXPORT_DTS const String Socket::shutdown_string(Const::string("shutdown"));
DR_EXPORT_DTS const String Socket::bind_string(Const::string("bind"));
DR_EXPORT_DTS const String Socket::getremote_string(Const::string("getremote"));
DR_EXPORT_DTS const String Socket::getlocal_string(Const::string("getlocal"));

DR_EXPORT_DTS const String Socket::opt_reuseaddr(Const::string("reuseaddr"));

Socket::Socket():
	Handle(M_READ|M_WRITE)
{
	Socket_sysiface::setHandleInvalid(this);
}

Socket::Socket(void *os_handle_, Mode mode_):
	Handle(mode_),
	os_handle(os_handle_)
{
}

Socket::~Socket()
{
	if (!(mode&M_NOCLOSE))
		Socket_sysiface::close(this);
}

ssize_t Socket::read(void *buf, size_t buflen)
{
	return Socket_sysiface::read(this, buf, buflen);
}

ssize_t Socket::write(const void *buf, size_t buflen)
{
	return Socket_sysiface::write(this, buf, buflen);
}

void Socket::connect(NetAddress *remote)
{
	Socket_sysiface::connect(this, remote);
}

void Socket::bind(NetAddress *local)
{
	Socket_sysiface::bind(this, local);
}

Handle::Mode Socket::waitData(Mode wait_mode, Sint64 nanosec)
{
	return Socket_sysiface::waitData(this, wait_mode, nanosec);
}

NetAddress *Socket::getLocalAddress()
{
	return Socket_sysiface::getLocalName(this);
}

void Socket::setSockOption(const String &name, long value)
{
	Socket_sysiface::setSockOption(this, name, value);
}

void Socket::setSockOptionStr(const String &name, const Blob &value)
{
	Socket_sysiface::setSockOptionStr(this, name, value);
}

long Socket::getSockOption(const String &name)
{
	return Socket_sysiface::getSockOption(this, name);
}

Blob Socket::getSockOptionStr(const String &name)
{
	return Socket_sysiface::getSockOptionStr(this, name);
}

void *Socket::getOsHandle()
{
	return os_handle;
}


DR_IO_NS_END
