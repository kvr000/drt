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

#ifndef dr__net__HttpServer__hxx__
# define dr__net__HttpServer__hxx__

#include <dr/net/def_net.hxx>

#include <dr/Hash.hxx>
#include <dr/io/StreamBuffer.hxx>
#include <dr/io/NetAddress.hxx>
#include <dr/io/SocketStream.hxx>
#include <dr/net/HttpConst.hxx>

#include <dr/Object.hxx>


DR_NET_NS_BEGIN

DR_IO_NS_USE;


class HttpServer: public Object
{
	DR_OBJECT_DECL(HttpServer, Object);

public:
	/* constructor */		HttpServer(SocketStream *stream);

protected:
	virtual				~HttpServer();

public:
	virtual void			setTimeLimit(SysTime abs_time)		{ read_stream.setTimeLimit(abs_time); }

public:
	virtual SocketStream *		getSocket();

	/**
	 * @return URI
	 */
	virtual String			readRequest();
	virtual HttpConst::MethodType	getMethod();
	virtual String			getHeader(const String &header);
	virtual Blob			readContent(size_t maxsize);
	virtual ssize_t			readContent(Blob *content, size_t maxsize);
	virtual Blob			readFullContent();
	virtual ssize_t			readFullContent(Blob *content);

public:
	virtual void			setCloseConnection();
	virtual void			sendResponse(int code, const String &message);
	virtual void			sendHeader(const String &header, const String &value);
	virtual void			sendContent(const Blob &data);

protected:
	bool				updateNextChunk();

protected:
	virtual void			processHeader(const String &header, const String &value);
	virtual void			processHeaderConnection(const String &header, const String &value);

protected:
	StreamBuffer			read_stream;
	String				req_method;
	String				req_uri;
	unsigned			req_version;
	THash<String, String>		req_headers;
	Sint64				req_body_size;

	HttpConst::MethodType		method_type;
	HttpConst::TransferEncoding	transfer_encoding;

	int				connection_state;		/**< 0 - start, 1 - keep-alive, 2 - close */

	static const StringIndex	header_process_map;
};


DR_NET_NS_END

#endif
