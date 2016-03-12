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

#ifndef dr__io__Socket__hxx__
# define dr__io__Socket__hxx__

#include <dr/io/Handle.hxx>

DR_IO_NS_BEGIN


class NetAddress;


/**
 * Socket handles
 *
 * Ancestor of all socket implementations
 */
class DR_IO_PUB Socket: public Handle
{
	DR_OBJECT_DECL_SIMPLE(Socket, Handle);

protected:
	DR_CONSTRUCT			Socket();
	DR_CONSTRUCT			Socket(void *os_handle, Mode mode);
	virtual				~Socket();

public:
	DRF_REDIR_HANDLE_HIGH();

	virtual ssize_t			read(void *buf, size_t size);
	virtual ssize_t			write(const void *buf, size_t size);

	virtual void			connect(NetAddress *remote);
	virtual void			bind(NetAddress *local);

	virtual Mode			waitData(Mode wait_mode, Sint64 nanosec);

	virtual NetAddress *		getLocalAddress();

	virtual void			setSockOption(const String &option, long value);
	virtual void			setSockOptionStr(const String &option, const Blob &value);
	virtual long			getSockOption(const String &option);
	virtual Blob			getSockOptionStr(const String &option);

	virtual void *			getOsHandle();

protected:
	void *				os_handle;

public:
	static const String		domain_local;
	static const String		domain_inet4;
	static const String		domain_inet6;

	static const String		proto_ip;
	static const String		proto_tcp;
	static const String		proto_udp;
	static const String		proto_icmp;
	static const String		proto_sctp;

public:
	static const String		socket_string;
	static const String		connect_string;
	static const String		shutdown_string;
	static const String		bind_string;
	static const String		getremote_string;
	static const String		getlocal_string;

public:
	static const String		opt_reuseaddr;

private:
	friend class Socket_sysiface_base;
};


DR_IO_NS_END

#endif
