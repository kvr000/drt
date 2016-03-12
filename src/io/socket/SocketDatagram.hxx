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

#ifndef dr__io__SocketDatagram__hxx__
# define dr__io__SocketDatagram__hxx__

#include <dr/Ref.hxx>

#include <dr/io/Socket.hxx>

DR_IO_NS_BEGIN


/**
 * Socket on top of datagram communication
 */
class DR_IO_PUB SocketDatagram: public Socket
{
	DR_OBJECT_DECL_SIMPLE(SocketDatagram, Socket);

public:
	DR_CONSTRUCT			SocketDatagram(const String &domain, const String &proto);
	DR_CONSTRUCT			SocketDatagram(void *os_handle, Mode mode);

public:
	virtual ssize_t			recvFrom(NetAddress **sender, void *buf, size_t buflen);
	virtual Blob			recvFrom(NetAddress **sender, size_t buflen);
	virtual ssize_t			recvVirtual(SocketDatagram **conn, void *buf, size_t buflen);
	virtual Blob			recvVirtual(SocketDatagram **conn, size_t buflen);

public:
	virtual NetAddress *		getRemote();

public:
	virtual SocketDatagram *	openVirtual(NetAddress *remote);

protected:
	friend class SocketDatagramVirtual;
};


/**
 * Virtual datagram communication
 *
 * All write operations are done via explicit specification of destination.
 * Read operation is not supported.
 */
class DR_IO_PUB SocketDatagramVirtual: public SocketDatagram
{
	DR_OBJECT_DECL_SIMPLE(SocketDatagramVirtual, SocketDatagram);

protected:
	DR_CONSTRUCT			SocketDatagramVirtual(SocketDatagram *owner, NetAddress *remote);
	DR_CONSTRUCT			SocketDatagramVirtual(SocketDatagram *owner, NetAddress *remote, const Blob &remote_sysiface);

public:
	DRF_REDIR_HANDLE_HIGH();
	virtual ssize_t			read(void *buf, size_t buflen);
	virtual ssize_t			write(const void *buf, size_t buflen);

public:
	virtual NetAddress *		getRemote();

protected:
	Blob				remote_sysiface;
	Ref<NetAddress>			remote;
	Ref<SocketDatagram>		owner;

private:
	friend class SocketDatagram;
	friend class Socket_sysiface_base;
};


DR_IO_NS_END

#endif
