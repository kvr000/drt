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

#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <sys/socket.h>

#include <dr/io/Socket.hxx>
#include <dr/io/dev/Socket_sysiface.hxx>

#ifdef dr__io__Socket_sysiface_posix__hxx__

#include <dr/Const.hxx>
#include <dr/UnsupportedException.hxx>
#include <dr/io/IoException.hxx>
#include <dr/io/NonblockException.hxx>
#include <dr/io/NetAddress.hxx>
#include <dr/io/NetAddressInet4.hxx>
#include <dr/io/NetAddressLocal.hxx>
#include <dr/io/BadHostName.hxx>

#include <dr/x_kw.hxx>
#include <dr/pe_error.hxx>


DR_IO_NS_BEGIN


const String Socket_sysiface_posix::comp_name(Const::string(DR_IO_NS_STRP("Socket_sysiface_posix")));

const String Socket_sysiface_posix::domain_string(Const::string("domain"));
const String Socket_sysiface_posix::proto_string(Const::string("proto"));
const String Socket_sysiface_posix::address_family_string(Const::string("address_family"));

StringIndex socket_opt_vals(&Socket::opt_reuseaddr, SO_REUSEADDR, NULL);

void Socket_sysiface_posix::throwSysException(Socket *handle, const String &operation)
{
	switch (errno) {
	case EAGAIN:
#if EAGAIN != EWOULDBLOCK
	case EWOULDBLOCK:
#endif
		xthrownew(NonblockException(handle, operation, DRP_EAGAIN, errno));
	default:
		/* TODO */
		xthrownew(IoException(handle, operation, errno, errno));
	}
}

int Socket_sysiface_posix::getDomainNum(const String &domain)
{
	if (domain == Socket::domain_local)
		return PF_LOCAL;
	if (domain == Socket::domain_inet4)
		return PF_INET;
#ifdef PF_INET6
	if (domain == Socket::domain_inet6)
		return PF_INET6;
#endif

	xthrownew(UnsupportedException(NULL, comp_name, domain_string, domain));
	return -1;
}

int Socket_sysiface_posix::getProtoNum(const String &proto)
{
	struct protoent *pe;
	if ((pe = getprotobyname(proto.utf8().toStr())) == NULL)
		xthrownew(UnsupportedException(NULL, comp_name, proto_string, proto));

	return pe->p_proto;
}

Blob Socket_sysiface_posix::getNetAddress(NetAddress *addr_)
{
	Blob interp;
	String addrcl = addr_->classname();

	if (addrcl == NetAddressInet4::comp_name) {
		NetAddressInet4 *addr = (NetAddressInet4 *)addr_;
		sockaddr_in *osaddr = (sockaddr_in *)interp.lock(sizeof(sockaddr_in));
		osaddr->sin_family = AF_INET;
		osaddr->sin_port = htons(addr->getPort());
		osaddr->sin_addr.s_addr = htonl(addr->getHost());
		interp.unlock(sizeof(sockaddr_in));
	}
	else if (addrcl == NetAddressLocal::comp_name) {
		NetAddressLocal *addr = (NetAddressLocal *)addr_;
		BString convpath = addr->getPath().utf8();
		size_t size = offsetof(sockaddr_un, sun_path)+convpath.getSize()+1;
		sockaddr_un *osaddr = (sockaddr_un *)interp.lock(size);
		osaddr->sun_family = AF_UNIX;
		memcpy(osaddr->sun_path, convpath.toStr(), convpath.getSize());
		osaddr->sun_path[convpath.getSize()] = '\0';
		interp.unlock(size);
	}
	else {
		xthrownew(UnsupportedException(addr_, comp_name));
	}
	return interp;
}

NetAddress *Socket_sysiface_posix::createNetAddress(const Blob &addr_sysiface)
{
	struct sockaddr *sa = (struct sockaddr *)addr_sysiface.toStr();
	switch (sa->sa_family) {
	case AF_INET:
		{
			struct sockaddr_in *sinet = (struct sockaddr_in *)sa;
			return new NetAddressInet4(htonl(sinet->sin_addr.s_addr), htons(sinet->sin_port));
		}
	case AF_LOCAL:
		{
			struct sockaddr_un *sun = (struct sockaddr_un *)sa;
			return new NetAddressLocal(sun->sun_path);
		}
	default:
		xthrownew(UnsupportedException(NULL, comp_name, address_family_string, String().appendNumber(sa->sa_family)));
	}
	return NULL;
}

Uint32 Socket_sysiface_posix::lookupInet4Host(const String &addr)
{
	if (struct hostent *h = gethostbyname2(addr.utf8().toStr(), AF_INET)) {
		return ((Uint8)h->h_addr_list[0][0]<<24)|((Uint8)h->h_addr_list[0][1]<<16)|((Uint8)h->h_addr_list[0][2]<<8)|((Uint8)h->h_addr_list[0][3]<<0);
	}
	else {
		xthrownew(BadHostName(addr));
		return 0;
	}
}

void Socket_sysiface_posix::setSockOption(Socket *socket, const String &name, long value)
{
	int opt;
	if ((opt = socket_opt_vals.find(name)) < 0)
		xthrownew(UnsupportedException(socket, Socket::comp_name, name, "set"));
	switch (opt) {
	case SO_REUSEADDR:
		{
			int iv = value;
			if (setsockopt(getOsHandle(socket), SOL_SOCKET, opt, &iv, sizeof(iv)) != 0)
				throwSysException(socket, "setsockopt");
		}
		break;

	default:
		xthrownew(UnsupportedException(socket, Socket::comp_name, name, "long"));
		break;
	}
}

void Socket_sysiface_posix::setSockOptionStr(Socket *socket, const String &name, const Blob &value)
{
	int opt;
	if ((opt = socket_opt_vals.find(name)) < 0)
		xthrownew(UnsupportedException(socket, Socket::comp_name, name, String(value)));
	switch (opt) {
	default:
		DR_AssertInvalid();
		xthrownew(UnsupportedException(socket, Socket::comp_name, name, "blob"));
		break;
	}
}

long Socket_sysiface_posix::getSockOption(Socket *socket, const String &name)
{
	int opt;
	if ((opt = socket_opt_vals.find(name)) < 0)
		xthrownew(UnsupportedException(socket, Socket::comp_name, name, "read"));
	switch (opt) {
	case SO_REUSEADDR:
		{
			int iv;
			socklen_t len = sizeof(iv);
			if (getsockopt(getOsHandle(socket), SOL_SOCKET, opt, &iv, &len) != 0)
				throwSysException(socket, "getsockopt");
			return iv;
		}
		break;

	default:
		DR_AssertInvalid();
		xthrownew(UnsupportedException(socket, Socket::comp_name, name, "long"));
		break;
	}
	return 0;
}

Blob Socket_sysiface_posix::getSockOptionStr(Socket *socket, const String &name)
{
	int opt;
	if ((opt = socket_opt_vals.find(name)) < 0)
		xthrownew(UnsupportedException(socket, Socket::comp_name, name, "read"));
	switch (opt) {
	default:
		DR_AssertInvalid();
		xthrownew(UnsupportedException(socket, Socket::comp_name, name, "blob"));
		break;
	}
	return Null();
}


DR_IO_NS_END

#endif
