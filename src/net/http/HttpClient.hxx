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

#ifndef dr__net__HttpClient__hxx__
# define dr__net__HttpClient__hxx__

#include <dr/net/def_net.hxx>

#include <dr/Hash.hxx>
#include <dr/io/StreamBuffer.hxx>
#include <dr/io/NetAddress.hxx>
#include <dr/io/SocketStream.hxx>

#include <dr/Object.hxx>


DR_NET_NS_BEGIN

DR_IO_NS_USE;


class DR_NET_PUB HttpClient: public Object
{
	DR_OBJECT_DECL(HttpClient, Object);

public:
	/* constructor */		HttpClient(const String &server_addr);
	/* constructor */		HttpClient(NetAddress *server_addr);

protected:
	virtual				~HttpClient();

public:
	virtual void			setTimeLimit(SysTime abs_time)		{ read_stream.setTimeLimit(abs_time); }

public:
	virtual void			sendRequest(const String &method, const String &uri, const THash<String, String> &headers, const Blob &content = Null());
	virtual void			partialRequest(const String &method, const String &uri, const THash<String, String> &headers);
	virtual void			partialHeader(const String &key, const String &value);
	virtual void			partialNoBody();
	virtual void			partialSendBody(const Blob &data);
	virtual void			partialStartBodyChunked();
	virtual void			partialBodyChunked(const Blob &data);
	virtual void			partialFinishBodyChunked();

public:
	virtual int			readHeaders(String *message);
	virtual String			getHeader(const String &header);
	virtual Blob			readContent(size_t maxsize);
	virtual ssize_t			readContent(Blob *content, size_t maxsize);
	virtual Blob			readFullContent();
	virtual ssize_t			readFullContent(Blob *content);

	static Blob			processEasy(const String &url);

protected:
	virtual void			createConnection(NetAddress *server_addr);

protected:
	StreamBuffer			read_stream;
	String				host;
	THash<String, String>		response_headers;
	ssize_t				response_size;
};


DR_NET_NS_END

#endif
