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

#ifndef dr__io__Socket_sysiface_wnt__hxx__
# define dr__io__Socket_sysiface_wnt__hxx__

#include <winsock2.h>

#include <dr/Exception.hxx>

#include <dr/io/NetAddress.hxx>
#include <dr/io/dev/Socket_sysiface.hxx>
#include <dr/io/SocketStream.hxx>
#include <dr/io/SocketDatagram.hxx>
#include <dr/io/SocketServer.hxx>

DR_IO_NS_BEGIN


/**
 * Socket POSIX system interface
 */
class DR_IO_PUB Socket_sysiface_wnt: public Socket_sysiface_base
{
public: /* generic interface */
	static void			throwSysException(Socket *handle, const String &operation, int wsa_error);

	static SOCKET			getOsHandle(Socket *handle);
	static bool			isHandleInvalid(Socket *handle);
	static void			setHandleInvalid(Socket *handle);

public: /* connection interface */
	static void *			createStreamed(Socket *handle, const String &domain, const String &proto);
	static void *			createDatagram(Socket *handle, const String &domain, const String &proto);

	static void			close(Socket *handle);

	static void			connect(Socket *handle, NetAddress *addr);
	static void			shutdown(Socket *handle, int direction);
	static void			bind(Socket *handle, NetAddress *local);

	static void			setSockOption(Socket *socket, const String &name, long value);
	static void			setSockOptionStr(Socket *socket, const String &name, const Blob &value);
	static long			getSockOption(Socket *socket, const String &name);
	static Blob			getSockOptionStr(Socket *socket, const String &name);

	static NetAddress *		getLocalName(Socket *handle);
	static NetAddress *		getPeerName(Socket *handle);

	static ssize_t			read(Socket *handle, void *buf, size_t buflen);
	static ssize_t			write(Socket *handle, const void *buf, size_t buflen);

	static ssize_t			readfrom(SocketDatagram *handle, NetAddress **recvaddr, Blob *sender_sysiface, void *buf, size_t buflen);
	static ssize_t			writeto(SocketDatagramVirtual *handle, const void *buf, size_t buflen);

	static Handle::Mode		waitData(Socket *handle, Handle::Mode mode, Sint64 ns_timeout);

public: /* server interface */
	static void *			createServer(SocketServer *handle, const String &domain, const String &proto);
	static void			closeServer(SocketServer *handle);

	static void			listen(SocketServer *handle, int max);
	static SocketStream *		accept(SocketServer *handle);

public:
	static Blob			getNetAddress(NetAddress *addr);
	static NetAddress *		createNetAddress(const Blob &addr_sysiface);

public:
	static Uint32			lookupInet4Host(const String &shost);

protected:
	static int			getDomainNum(const String &domain);
	static int			getProtoNum(const String &proto);

	static int			mapWsaErrorToPosix(int error);

public:
	static const String		comp_name;

public:
	static const String		domain_string;
	static const String		proto_string;
	static const String		address_family_string;
};


DR_RINLINE SOCKET Socket_sysiface_wnt::getOsHandle(Socket *handle)
{
	return (SOCKET)b_getOsHandle(handle);
}

DR_RINLINE bool Socket_sysiface_wnt::isHandleInvalid(Socket *handle)
{
	return (SOCKET)getOsHandle(handle) == INVALID_SOCKET;
}

DR_RINLINE void Socket_sysiface_wnt::setHandleInvalid(Socket *handle)
{
	b_setHandle(handle, (void *)INVALID_SOCKET);
}

DR_RINLINE void *Socket_sysiface_wnt::createStreamed(Socket *handle, const String &domain, const String &proto)
{
	SOCKET sd;
	
	if ((sd = ::WSASocket(getDomainNum(domain), SOCK_STREAM, getProtoNum(proto), NULL, 0, 0)) == INVALID_SOCKET)
		throwSysException(handle, Socket::socket_string, WSAGetLastError());
	return (void *)(SintPtr)sd;
}

DR_RINLINE void *Socket_sysiface_wnt::createDatagram(Socket *handle, const String &domain, const String &proto)
{
	SOCKET sd;
	
	if ((sd = ::WSASocket(getDomainNum(domain), SOCK_DGRAM, getProtoNum(proto), NULL, 0, 0)) == INVALID_SOCKET)
		throwSysException(handle, Socket::socket_string, WSAGetLastError());
	return (void *)(SintPtr)sd;
}

DR_RINLINE void Socket_sysiface_wnt::close(Socket *handle)
{
	SOCKET fd = getOsHandle(handle);
	if (fd != INVALID_SOCKET)
		CloseHandle((HANDLE)fd);
}

DR_RINLINE void Socket_sysiface_wnt::connect(Socket *handle, NetAddress *remote)
{
	Blob osaddr = getNetAddress(remote);

	if (::WSAConnect(getOsHandle(handle), (const sockaddr *)osaddr.toStr(), osaddr.getSize(), NULL, NULL, NULL, NULL) < 0) {
		throwSysException(handle, Socket::connect_string, WSAGetLastError());
	}
}

DR_RINLINE void Socket_sysiface_wnt::shutdown(Socket *handle, int direction)
{
	if (::shutdown(getOsHandle(handle), direction-1) < 0) {
		throwSysException(handle, Socket::shutdown_string, WSAGetLastError());
	}
}

DR_RINLINE void Socket_sysiface_wnt::bind(Socket *handle, NetAddress *local)
{
	Blob osaddr = getNetAddress(local);

	if (::bind(getOsHandle(handle), (const sockaddr *)osaddr.toStr(), osaddr.getSize()) < 0)
		throwSysException(handle, Socket::bind_string, WSAGetLastError());
}

DR_RINLINE NetAddress *Socket_sysiface_wnt::getLocalName(Socket *handle)
{
	Blob addr_sysiface;
	struct sockaddr *sender_low = (struct sockaddr *)addr_sysiface.lock(sizeof(struct sockaddr));
	int sender_len = sizeof(struct sockaddr);
	if ((::getsockname(getOsHandle(handle), sender_low, &sender_len)) < 0) {
		throwSysException(handle, Socket::getremote_string, WSAGetLastError());
	}
	addr_sysiface.unlock(sender_len);
	return createNetAddress(addr_sysiface);
}

DR_RINLINE NetAddress *Socket_sysiface_wnt::getPeerName(Socket *handle)
{
	Blob addr_sysiface;
	struct sockaddr *sender_low = (struct sockaddr *)addr_sysiface.lock(sizeof(struct sockaddr));
	int sender_len = sizeof(struct sockaddr);
	if ((::getpeername(getOsHandle(handle), sender_low, &sender_len)) < 0) {
		throwSysException(handle, Socket::getremote_string, WSAGetLastError());
	}
	addr_sysiface.unlock(sender_len);
	return createNetAddress(addr_sysiface);
}

DR_RINLINE ssize_t Socket_sysiface_wnt::read(Socket *handle, void *buf, size_t buflen)
{
	ssize_t br;
	if ((br = ::recv(getOsHandle(handle), (char *)buf, buflen, 0)) < 0)
		throwSysException(handle, Socket::read_string, WSAGetLastError());
	return br;
}

DR_RINLINE ssize_t Socket_sysiface_wnt::write(Socket *handle, const void *buf, size_t buflen)
{
	ssize_t br;
	if ((br = ::send(getOsHandle(handle), (const char *)buf, buflen, 0)) < 0)
		throwSysException(handle, Socket::write_string, WSAGetLastError());
	return br;
}

DR_RINLINE ssize_t Socket_sysiface_wnt::readfrom(SocketDatagram *handle, NetAddress **addr, Blob *addr_sysiface, void *buf, size_t buflen)
{
	ssize_t br;
	struct sockaddr *sender_low = (struct sockaddr *)addr_sysiface->lock(sizeof(struct sockaddr));
	int sender_len = sizeof(struct sockaddr);
	if ((br = ::recvfrom(getOsHandle(handle), (char *)buf, buflen, 0, sender_low, &sender_len)) < 0) {
		throwSysException(handle, Socket::read_string, WSAGetLastError());
	}
	addr_sysiface->unlock(sender_len);
	DR_REF_XCHG(addr, createNetAddress(*addr_sysiface));
	return br;
}

DR_RINLINE ssize_t Socket_sysiface_wnt::writeto(SocketDatagramVirtual *handle, const void *buf, size_t buflen)
{
	ssize_t br;
	const Blob &remote_sysiface = b_getRemote_sysiface(handle);
	if ((br = ::sendto(getOsHandle(handle), (const char *)buf, buflen, 0, (const sockaddr *)remote_sysiface.toStr(), remote_sysiface.getSize())) < 0) {
		throwSysException(handle, Socket::write_string, WSAGetLastError());
	}
	return br;
}

DR_RINLINE void *Socket_sysiface_wnt::createServer(SocketServer *handle, const String &domain, const String &proto)
{
	SOCKET sd;
	
	if ((sd = ::WSASocket(getDomainNum(domain), SOCK_STREAM, getProtoNum(proto), NULL, 0, 0)) == INVALID_SOCKET)
		throwSysException(handle, Socket::socket_string, WSAGetLastError());
	return (void *)(SintPtr)sd;
}

DR_RINLINE void Socket_sysiface_wnt::closeServer(SocketServer *handle)
{
	SOCKET fd = getOsHandle(handle);
	if (fd != INVALID_SOCKET)
		CloseHandle((HANDLE)fd);
}

DR_RINLINE void Socket_sysiface_wnt::listen(SocketServer *handle, int max)
{
	if (::listen(getOsHandle(handle), max < 0 ? SOMAXCONN : 0) < 0) {
		throwSysException(handle, handle->listen_string, WSAGetLastError());
	}
}

DR_RINLINE SocketStream *Socket_sysiface_wnt::accept(SocketServer *handle)
{
	SOCKET newfd;
	Blob addr_sysiface;
	struct sockaddr *sender_low = (struct sockaddr *)addr_sysiface.lock(sizeof(struct sockaddr));
	int sender_len = sizeof(struct sockaddr);
	if ((newfd = ::accept(getOsHandle(handle), sender_low, &sender_len)) == INVALID_SOCKET) {
		throwSysException(handle, handle->accept_string, WSAGetLastError());
	}
	addr_sysiface.unlock(sender_len);
	DR_TRY {
		ERef<NetAddress> remote(createNetAddress(addr_sysiface));
		return new SocketStream(remote, (void *)(UintPtr)newfd);
	}
	DR_CATCHANY {
		CloseHandle((HANDLE)newfd);
		DR_RETHROWANY;
	}
	DR_ENDTRY
}

DR_RINLINE Handle::Mode Socket_sysiface_wnt::waitData(Socket *handle, Handle::Mode mode, Sint64 ns_timeout)
{
	SOCKET sock = getOsHandle(handle);
	WSANETWORKEVENTS events;
	HANDLE ev = WSACreateEvent();
	if (!ev) {
		throwSysException(handle, handle->wait_string, WSAGetLastError());
	}
	if (WSAEventSelect(sock, ev, ((mode&Handle::M_READ) ? FD_READ|FD_ACCEPT|FD_CLOSE : 0) | ((mode&Handle::M_WRITE) ? FD_WRITE|FD_CONNECT : 0) | ((mode&Handle::M_EXCEPT) ? FD_OOB : 0)) != 0) {
		CloseHandle(ev);
		throwSysException(handle, handle->wait_string, WSAGetLastError());
	}
	int ret = WaitForSingleObject(ev, (DWORD)(ns_timeout < 0 ? INFINITE : ns_timeout/1000000));
	WSAEventSelect(sock, NULL, 0);
	if (ret == WAIT_OBJECT_0)
		WSAEnumNetworkEvents(sock, ev, &events);
	CloseHandle(ev);
	if (ret == WAIT_OBJECT_0) {
		return (Handle::Mode)(((events.lNetworkEvents&(FD_READ|FD_ACCEPT|FD_CLOSE)) ? Handle::M_READ : 0) | ((events.lNetworkEvents&(FD_OOB)) ? 0 : Handle::M_WRITE) | ((events.lNetworkEvents&(FD_WRITE|FD_CONNECT)) ? 0 : Handle::M_EXCEPT));
	}
	else if (ret == -1) {
		throwSysException(handle, handle->wait_string, WSAGetLastError());
		return (Handle::Mode)0;
	}
	else {
		return (Handle::Mode)0;
	}
}


DR_IO_NS_END

#endif
