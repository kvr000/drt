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

#ifndef dr__io__Socket_sysiface_posix__hxx__
# define dr__io__Socket_sysiface_posix__hxx__

#include <errno.h>
#include <sys/socket.h>
#include <sys/poll.h>

#include <dr/Except.hxx>

#include <dr/io/NetAddress.hxx>
#include <dr/io/dev/Socket_sysiface.hxx>
#include <dr/io/SocketStream.hxx>
#include <dr/io/SocketDatagram.hxx>
#include <dr/io/SocketServer.hxx>

DR_IO_NS_BEGIN


/**
 * Socket POSIX system interface
 */
class DR_IO_PUB Socket_sysiface_posix: public Socket_sysiface_base
{
public: /* generic interface */
	static void			throwSysException(Socket *handle, const String &operation);

	static int			getOsHandle(Socket *handle);
	static bool			isHandleInvalid(Socket *handle);
	static void			setHandleInvalid(Socket *handle);

public: /* connection interface */
	static void *			createStreamed(Socket *handle, const String &domain, const String &proto);
	static void *			createDatagram(Socket *handle, const String &domain, const String &proto);

	static void			close(Socket *handle);

	static void			connect(Socket *handle, NetAddress *addr);
	static void			bind(Socket *handle, NetAddress *local);
	static void			shutdown(Socket *handle, int direction);

	static NetAddress *		getLocalName(Socket *handle);
	static NetAddress *		getPeerName(Socket *handle);

	static ssize_t			read(Socket *handle, void *buf, size_t buflen);
	static ssize_t			write(Socket *handle, const void *buf, size_t buflen);

	static ssize_t			readfrom(SocketDatagram *handle, NetAddress **recvaddr, Blob *sender_sysiface, void *buf, size_t buflen);
	static ssize_t			writeto(SocketDatagramVirtual *handle, const void *buf, size_t buflen);

	static Handle::Mode		waitData(Socket *handle, Handle::Mode mode, Sint64 ns_timeout);

	static void			setSockOption(Socket *handle, const String &name, long value);
	static void			setSockOptionStr(Socket *handle, const String &name, const BString &value);
	static long			getSockOption(Socket *handle, const String &name);
	static BString			getSockOptionStr(Socket *handle, const String &name);

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

public:
	static const String		comp_name;

	static const String		domain_string;
	static const String		proto_string;
	static const String		address_family_string;
};


DR_RINLINE int Socket_sysiface_posix::getOsHandle(Socket *handle)
{
	return (int)(SintPtr)b_getOsHandle(handle);
}

DR_RINLINE bool Socket_sysiface_posix::isHandleInvalid(Socket *handle)
{
	return getOsHandle(handle) < 0;
}

DR_RINLINE void Socket_sysiface_posix::setHandleInvalid(Socket *handle)
{
	b_setHandle(handle, (void *)(SintPtr)-1);
}

DR_RINLINE void *Socket_sysiface_posix::createStreamed(Socket *handle, const String &domain, const String &proto)
{
	int sd;
	
	if ((sd = ::socket(getDomainNum(domain), SOCK_STREAM, getProtoNum(proto))) < 0)
		throwSysException(handle, Socket::socket_string);
	return (void *)(SintPtr)sd;
}

DR_RINLINE void *Socket_sysiface_posix::createDatagram(Socket *handle, const String &domain, const String &proto)
{
	int sd;
	
	if ((sd = ::socket(getDomainNum(domain), SOCK_DGRAM, getProtoNum(proto))) < 0)
		throwSysException(handle, Socket::socket_string);
	return (void *)(SintPtr)sd;
}

DR_RINLINE void Socket_sysiface_posix::close(Socket *handle)
{
	int fd = getOsHandle(handle);
	if (fd >= 0)
		::close(fd);
}

DR_RINLINE void Socket_sysiface_posix::connect(Socket *handle, NetAddress *remote)
{
	Blob osaddr = getNetAddress(remote);

	if (::connect(getOsHandle(handle), (const sockaddr *)osaddr.toStr(), osaddr.getSize()) < 0)
		throwSysException(handle, Socket::connect_string);
}

DR_RINLINE void Socket_sysiface_posix::shutdown(Socket *handle, int direction)
{
	if (::shutdown(getOsHandle(handle), direction-1) < 0)
		throwSysException(handle, Socket::shutdown_string);
}

DR_RINLINE void Socket_sysiface_posix::bind(Socket *handle, NetAddress *local)
{
	Blob osaddr = getNetAddress(local);

	if (::bind(getOsHandle(handle), (const sockaddr *)osaddr.toStr(), osaddr.getSize()) < 0)
		throwSysException(handle, Socket::bind_string);
}

DR_RINLINE NetAddress *Socket_sysiface_posix::getLocalName(Socket *handle)
{
	Blob addr_sysiface;
	struct sockaddr *sender_low = (struct sockaddr *)addr_sysiface.lock(sizeof(struct sockaddr));
	socklen_t sender_len = sizeof(struct sockaddr);
	if ((::getsockname(getOsHandle(handle), sender_low, &sender_len)) < 0) {
		throwSysException(handle, handle->getremote_string);
	}
	addr_sysiface.unlock(sender_len);
	return createNetAddress(addr_sysiface);
}

DR_RINLINE NetAddress *Socket_sysiface_posix::getPeerName(Socket *handle)
{
	Blob addr_sysiface;
	struct sockaddr *sender_low = (struct sockaddr *)addr_sysiface.lock(sizeof(struct sockaddr));
	socklen_t sender_len = sizeof(struct sockaddr);
	if ((::getpeername(getOsHandle(handle), sender_low, &sender_len)) < 0) {
		throwSysException(handle, handle->getremote_string);
	}
	addr_sysiface.unlock(sender_len);
	return createNetAddress(addr_sysiface);
}

DR_RINLINE ssize_t Socket_sysiface_posix::read(Socket *handle, void *buf, size_t buflen)
{
	ssize_t br;
	if ((br = ::read(getOsHandle(handle), buf, buflen)) < 0)
		throwSysException(handle, Socket::read_string);
	return br;
}

DR_RINLINE ssize_t Socket_sysiface_posix::write(Socket *handle, const void *buf, size_t buflen)
{
	ssize_t br;
	if ((br = ::write(getOsHandle(handle), buf, buflen)) < 0)
		throwSysException(handle, Socket::write_string);
	return br;
}

DR_RINLINE ssize_t Socket_sysiface_posix::readfrom(SocketDatagram *handle, NetAddress **addr, Blob *addr_sysiface, void *buf, size_t buflen)
{
	ssize_t br;
	struct sockaddr *sender_low = (struct sockaddr *)addr_sysiface->lock(sizeof(struct sockaddr));
	socklen_t sender_len = sizeof(struct sockaddr);
	if ((br = ::recvfrom(getOsHandle(handle), buf, buflen, 0, sender_low, &sender_len)) < 0) {
		throwSysException(handle, Socket::read_string);
	}
	addr_sysiface->unlock(sender_len);
	DR_REF_XCHG(addr, createNetAddress(*addr_sysiface));
	return br;
}

DR_RINLINE ssize_t Socket_sysiface_posix::writeto(SocketDatagramVirtual *handle, const void *buf, size_t buflen)
{
	ssize_t br;
	const Blob &remote_sysiface = b_getRemote_sysiface(handle);
	if ((br = ::sendto(getOsHandle(handle), buf, buflen, 0, (const sockaddr *)remote_sysiface.toStr(), remote_sysiface.getSize())) < 0) {
		printf("throwing sys exception: %d\n", errno);
		throwSysException(handle, Socket::write_string);
	}
	return br;
}

DR_RINLINE void *Socket_sysiface_posix::createServer(SocketServer *handle, const String &domain, const String &proto)
{
	int sd;
	
	if ((sd = ::socket(getDomainNum(domain), SOCK_STREAM, getProtoNum(proto))) < 0)
		throwSysException(handle, Socket::socket_string);
	return (void *)(SintPtr)sd;
}

DR_RINLINE void Socket_sysiface_posix::closeServer(SocketServer *handle)
{
	int fd = getOsHandle(handle);
	if (fd >= 0)
		::close(fd);
}

DR_RINLINE void Socket_sysiface_posix::listen(SocketServer *handle, int max)
{
	if (::listen(getOsHandle(handle), max < 0 ? SOMAXCONN : 0) < 0) {
		throwSysException(handle, handle->listen_string);
	}
}

DR_RINLINE SocketStream *Socket_sysiface_posix::accept(SocketServer *handle)
{
	int newfd;
	Blob addr_sysiface;
	struct sockaddr *sender_low = (struct sockaddr *)addr_sysiface.lock(sizeof(struct sockaddr));
	socklen_t sender_len = sizeof(struct sockaddr);
	if ((newfd = ::accept(getOsHandle(handle), sender_low, &sender_len)) < 0) {
		throwSysException(handle, handle->accept_string);
	}
	addr_sysiface.unlock(sender_len);
	DR_TRY {
		ERef<NetAddress> remote(createNetAddress(addr_sysiface));
		return new SocketStream(remote, (void *)(UintPtr)newfd);
	}
	DR_CATCHANY {
		::close(newfd);
		DR_RETHROWANY;
	}
	DR_ENDTRY
}

DR_RINLINE Handle::Mode Socket_sysiface_posix::waitData(Socket *handle, Handle::Mode mode, Sint64 ns_timeout)
{
	struct pollfd fd;
	fd.fd = getOsHandle(handle);
	fd.events = ((mode&Handle::M_READ) ? POLLIN : 0) | ((mode&Handle::M_WRITE) ? POLLOUT : 0) | ((mode&Handle::M_EXCEPT) ? POLLPRI : 0);
	fd.revents = fd.events;
	switch (poll(&fd, 1, ns_timeout >= 0 ? (ns_timeout+999999)/1000000 : -1)) {
	case 0:
		return (Handle::Mode)0;
	case 1:
		return (Handle::Mode)(((fd.revents&POLLIN) ? Handle::M_READ : 0) | ((fd.revents&POLLOUT) ? Handle::M_WRITE : 0) | ((fd.revents&POLLPRI) ? Handle::M_EXCEPT : 0));
	default:
		throwSysException(handle, handle->wait_string);
		return (Handle::Mode)0;
	}
}


DR_IO_NS_END

#endif
