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

#include <winsock2.h>
#include <windows.h>

#include <dr/x_kw.hxx>
#include <dr/pe_error.hxx>
#include <dr/io/Socket.hxx>
#include <dr/io/dev/Socket_sysiface.hxx>
#include <dr/io/dev/File_sysiface.hxx>
#include <dr/io/dev/File_sysiface_wnt.hxx>

#include <dr/Const.hxx>
#include <dr/io/BadHostName.hxx>
#include <dr/UnsupportedExcept.hxx>
#include <dr/io/IoExcept.hxx>
#include <dr/io/NonblockExcept.hxx>
#include <dr/io/NetAddress.hxx>
#include <dr/io/NetAddressInet4.hxx>
#include <dr/io/NetAddressLocal.hxx>


DR_IO_NS_BEGIN


const String Socket_sysiface_wnt::comp_name(Const::string("Socket_sysiface_wnt"));

const String Socket_sysiface_wnt::domain_string(Const::string("domain"));
const String Socket_sysiface_wnt::proto_string(Const::string("proto"));
const String Socket_sysiface_wnt::address_family_string(Const::string("address_family"));

StringIndex socket_opt_vals(&Socket::opt_reuseaddr, SO_REUSEADDR, NULL);

static bool initializeWsa()
{
	WSADATA data;
	if (WSAStartup(MAKEWORD(2, 2), &data) != 0) {
		DR_THROWNEW(UnsupportedExcept(NULL, Socket_sysiface_wnt::comp_name, "version", "2.2"));
	}
	return true;
}

static bool dummy0 = initializeWsa();

int Socket_sysiface_wnt::mapWsaErrorToPosix(int error)
{
	int error_map10000[] = {
		-1,
		-1,
		-1,
		-1,
		DRP_EINTR,			// WSAEINTR		(WSABASEERR+4)
		-1,
		-1,
		-1,
		-1,
		DRP_EBADF,			// WSAEBADF		(WSABASEERR+9)
		-1,
		-1,
		-1,
		DRP_EACCES,			// WSAEACCES		(WSABASEERR+13)
		DRP_EFAULT,			// WSAEFAULT		(WSABASEERR+14)
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		DRP_EINVAL,			// WSAEINVAL		(WSABASEERR+22)
		-1,
		DRP_EMFILE,			// WSAEMFILE		(WSABASEERR+24)
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		DRP_EAGAIN,			// WSAEWOULDBLOCK	(WSABASEERR+35)
		DRP_EINPROGRESS,		// WSAEINPROGRESS	(WSABASEERR+36) /* deprecated on WinSock2 */
		DRP_EALREADY,			// WSAEALREADY		(WSABASEERR+37)
		DRP_ENOTSOCK,			// WSAENOTSOCK		(WSABASEERR+38)
		DRP_EDESTADDRREQ,		// WSAEDESTADDRREQ	(WSABASEERR+39)
		DRP_EMSGSIZE,			// WSAEMSGSIZE		(WSABASEERR+40)
		DRP_EPROTOTYPE,			// WSAEPROTOTYPE	(WSABASEERR+41)
		DRP_ENOPROTOOPT,		// WSAENOPROTOOPT	(WSABASEERR+42)
		DRP_EPROTONOSUPPORT,		// WSAEPROTONOSUPPORT	(WSABASEERR+43)
		DRP_ESOCKTNOSUPPORT,		// WSAESOCKTNOSUPPORT	(WSABASEERR+44)
		DRP_EOPNOTSUPP,			// WSAEOPNOTSUPP	(WSABASEERR+45)
		DRP_EPFNOSUPPORT,		// WSAEPFNOSUPPORT	(WSABASEERR+46)
		DRP_EAFNOSUPPORT,		// WSAEAFNOSUPPORT	(WSABASEERR+47)
		DRP_EADDRINUSE,			// WSAEADDRINUSE	(WSABASEERR+48)
		DRP_EADDRNOTAVAIL,		// WSAEADDRNOTAVAIL	(WSABASEERR+49)
		DRP_ENETDOWN,			// WSAENETDOWN		(WSABASEERR+50)
		DRP_ENETUNREACH,		// WSAENETUNREACH	(WSABASEERR+51)
		DRP_ENETRESET,			// WSAENETRESET		(WSABASEERR+52)
		DRP_ECONNABORTED,		// WSAECONNABORTED	(WSABASEERR+53)
		DRP_ECONNRESET,			// WSAECONNRESET	(WSABASEERR+54)
		DRP_ENOBUFS,			// WSAENOBUFS		(WSABASEERR+55)
		DRP_EISCONN,			// WSAEISCONN		(WSABASEERR+56)
		DRP_ENOTCONN,			// WSAENOTCONN		(WSABASEERR+57)
		DRP_ESHUTDOWN,			// WSAESHUTDOWN		(WSABASEERR+58)
		DRP_ETOOMANYREFS,		// WSAETOOMANYREFS	(WSABASEERR+59)
		DRP_ETIMEDOUT,			// WSAETIMEDOUT		(WSABASEERR+60)
		DRP_ECONNREFUSED,		// WSAECONNREFUSED	(WSABASEERR+61)
		DRP_ELOOP,			// WSAELOOP		(WSABASEERR+62)
		DRP_ENAMETOOLONG,		// WSAENAMETOOLONG	(WSABASEERR+63)
		DRP_EHOSTDOWN,			// WSAEHOSTDOWN		(WSABASEERR+64)
		DRP_EHOSTUNREACH,		// WSAEHOSTUNREACH	(WSABASEERR+65)
		DRP_ENOTEMPTY,			// WSAENOTEMPTY		(WSABASEERR+66)
		DRP_EPROCLIM,			// WSAEPROCLIM		(WSABASEERR+67)
		DRP_EUSERS,			// WSAEUSERS		(WSABASEERR+68)
		DRP_EDQUOT,			// WSAEDQUOT		(WSABASEERR+69)
		DRP_ESTALE,			// WSAESTALE		(WSABASEERR+70)
		DRP_EREMOTE,			// WSAEREMOTE		(WSABASEERR+71)
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		DRP_ENOTSUP,			// WSASYSNOTREADY	(WSABASEERR+91)
		DRP_ENOTSUP,			// WSAVERNOTSUPPORTED	(WSABASEERR+92)
		DRP_ENOTSUP,			// WSANOTINITIALISED	(WSABASEERR+93)
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		DRP_ENOTCONN,			// WSAEDISCON		(WSABASEERR+101)
		-1,				// WSAENOMORE		(WSABASEERR+102)
		DRP_EINTR,			// WSAECANCELLED	(WSABASEERR+103)
		DRP_EINVAL,			// WSAEINVALIDPROCTABLE	(WSABASEERR+104)
		DRP_EINVAL,			// WSAEINVALIDPROVIDER	(WSABASEERR+105)
		DRP_EINVAL,			// WSAEPROVIDERFAILEDINIT	(WSABASEERR+106)
		-1,				// WSASYSCALLFAILURE	(WSABASEERR+107)
		-1,				// WSASERVICE_NOT_FOUND	(WSABASEERR+108)
		-1,				// WSATYPE_NOT_FOUND	(WSABASEERR+109)
		-1,				// WSA_E_NO_MORE	(WSABASEERR+110)
		-1,				// WSA_E_CANCELLED	(WSABASEERR+111)
		DRP_ECONNREFUSED,		// WSAEREFUSED		(WSABASEERR+112)
	};

	if (error < 10000) {
	}
	else if ((unsigned)error < 10000+sizeof(error_map10000)/sizeof(error_map10000[0])) {
		if (error_map10000[error-10000] >= 0)
			return error_map10000[error-10000];
	}
	return File_sysiface_wnt::mapWntErrorToPosix(error);
	//return DRP_ENOTSUP;
}

void Socket_sysiface_wnt::throwSysException(Socket *handle, const String &operation, int wsa_err)
{
	switch (wsa_err) {
	case WSAEWOULDBLOCK:
		DR_THROWNEW(NonblockExcept(handle, operation, DRP_EAGAIN, wsa_err));
	default:
		/* TODO */
		DR_THROWNEW(IoExcept(handle, operation, mapWsaErrorToPosix(wsa_err), wsa_err));
	}
}

int Socket_sysiface_wnt::getDomainNum(const String &domain)
{
	if (domain == Socket::domain_inet4)
		return PF_INET;
#ifdef PF_INET6
	if (domain == Socket::domain_inet6)
		return PF_INET6;
#endif

	DR_THROWNEW(UnsupportedExcept(NULL, comp_name, domain_string, domain));
	return -1;
}

int Socket_sysiface_wnt::getProtoNum(const String &proto)
{
#ifdef DR_OS_WCE
	if (proto == "tcp")
		return IPPROTO_TCP;
	else if (proto == "udp")
		return IPPROTO_UDP;
	else if (proto == "ip")
		return IPPROTO_IP;
	else
		xthrownew(UnsupportedExcept(NULL, comp_name, proto_string, proto));
	return -1;
#else
	struct protoent *pe;
	if ((pe = getprotobyname(proto.utf8().toStr())) == NULL) {
		int err = WSAGetLastError();
		(void)err;
		DR_THROWNEW(UnsupportedExcept(NULL, comp_name, proto_string, proto));
	}
	return pe->p_proto;
#endif
}

Blob Socket_sysiface_wnt::getNetAddress(NetAddress *addr_)
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
#if 0
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
#endif
	else {
		DR_THROWNEW(UnsupportedExcept(addr_, comp_name));
	}
	return interp;
}

NetAddress *Socket_sysiface_wnt::createNetAddress(const Blob &addr_sysiface)
{
	struct sockaddr *sa = (struct sockaddr *)addr_sysiface.toStr();
	switch (sa->sa_family) {
	case AF_INET:
		{
			struct sockaddr_in *sinet = (struct sockaddr_in *)sa;
			return new NetAddressInet4(htonl(sinet->sin_addr.s_addr), htons(sinet->sin_port));
		}
#if 0
	case AF_LOCAL:
		{
			struct sockaddr_un *sun = (struct sockaddr_un *)sa;
			return new NetAddressLocal(sun->sun_path);
		}
#endif
	default:
		DR_THROWNEW(UnsupportedExcept(NULL, comp_name, address_family_string, String().appendNumber(sa->sa_family)));
	}
	return NULL;
}

Uint32 Socket_sysiface_wnt::lookupInet4Host(const String &addr)
{
	if (struct hostent *h = gethostbyname(addr.utf8().toStr())) {
		return ((Uint8)h->h_addr_list[0][0]<<24)|((Uint8)h->h_addr_list[0][1]<<16)|((Uint8)h->h_addr_list[0][2]<<8)|((Uint8)h->h_addr_list[0][3]<<0);
	}
	else {
		xthrownew(BadHostName(addr));
		return 0;
	}
}

void Socket_sysiface_wnt::setSockOption(Socket *socket, const String &name, long value)
{
	int opt;
	if ((opt = socket_opt_vals.find(name)) < 0)
		xthrownew(UnsupportedExcept(socket, Socket::comp_name, name, "set"));
	switch (opt) {
	case SO_REUSEADDR:
		{
			int iv = value;
			if (setsockopt(getOsHandle(socket), SOL_SOCKET, opt, (const char *)&iv, sizeof(iv)) != 0)
				throwSysException(socket, "setsockopt", WSAGetLastError());
		}
		break;

	default:
		xthrownew(UnsupportedExcept(socket, Socket::comp_name, name, "long"));
		break;
	}
}

void Socket_sysiface_wnt::setSockOptionStr(Socket *socket, const String &name, const Blob &value)
{
	int opt;
	if ((opt = socket_opt_vals.find(name)) < 0)
		xthrownew(UnsupportedExcept(socket, Socket::comp_name, name, String(value)));
	switch (opt) {
	default:
		DR_AssertInvalid();
		xthrownew(UnsupportedExcept(socket, Socket::comp_name, name, "blob"));
		break;
	}
}

long Socket_sysiface_wnt::getSockOption(Socket *socket, const String &name)
{
	int opt;
	if ((opt = socket_opt_vals.find(name)) < 0)
		xthrownew(UnsupportedExcept(socket, Socket::comp_name, name, "read"));
	switch (opt) {
	case SO_REUSEADDR:
		{
			int iv;
			int len = sizeof(iv);
			if (getsockopt(getOsHandle(socket), SOL_SOCKET, opt, (char *)&iv, &len) != 0)
				throwSysException(socket, "getsockopt", WSAGetLastError());
			return iv;
		}
		break;

	default:
		DR_AssertInvalid();
		xthrownew(UnsupportedExcept(socket, Socket::comp_name, name, "long"));
		break;
	}
	return 0;
}

Blob Socket_sysiface_wnt::getSockOptionStr(Socket *socket, const String &name)
{
	int opt;
	if ((opt = socket_opt_vals.find(name)) < 0)
		xthrownew(UnsupportedExcept(socket, Socket::comp_name, name, "read"));
	switch (opt) {
	default:
		DR_AssertInvalid();
		xthrownew(UnsupportedExcept(socket, Socket::comp_name, name, "blob"));
		break;
	}
	return Null();
}


DR_IO_NS_END
