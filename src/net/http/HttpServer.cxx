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

#include <dr/x_kw.hxx>
#include <dr/Const.hxx>

#include <dr/MethodConv.hxx>
#include <dr/EndOfDataExcept.hxx>
#include <dr/UnsupportedExcept.hxx>
#include <dr/TimeoutExcept.hxx>
#include <dr/Thread.hxx>

#include <dr/io/NetAddressInet4.hxx>

#include <dr/net/HttpServer.hxx>

DR_NET_NS_BEGIN


DR_OBJECT_DEF(DR_NET_NS_STR, HttpServer, Object);
DR_OBJECT_IMPL_SIMPLE(HttpServer);

const StringIndex HttpServer::header_process_map(
		&HttpConst::connection_string, (SintPtr)MethodConv::methodToFptr(&HttpServer::processHeaderConnection),
		NULL
		);


HttpServer::HttpServer(SocketStream *stream):
	read_stream(stream),
	method_type(HttpConst::MT_Unknown),
	connection_state(0)
{
}

HttpServer::~HttpServer()
{
}

SocketStream *HttpServer::getSocket()
{
	return iref((SocketStream *)read_stream.accHandle());
}

String HttpServer::readRequest()
{
	if (connection_state >= 2)
		return Null();
	connection_state = 0;

	transfer_encoding = HttpConst::TE_Basic;

	xtry {
		{
			ssize_t n;
			xtry {
				n = read_stream.tryReadUntilChar('\n');
			}
			xcatch (TimeoutExcept, ex) {
				if (read_stream.ptrCache()->getSize() != 0)
					xthrowref(ex);
				n = -1;
			}
			xend;
			if (n < 0) {
				if (read_stream.ptrCache()->getSize() == 0) {
					connection_state = 2;
					return Null();
				}
				xthrownew(EndOfDataExcept("http", "method"));
			}
			const char *b = read_stream.ptrCache()->toStr();
			const char *a = b;
			char *c;
			for (c = (char *)b+n; b < c && !isspace(*b); b++);
			if (b == c)
				xthrownew(EndOfDataExcept("http", "method"));
			req_method = String::createLowerUtf8(a, b-a);
			if ((method_type = (HttpConst::MethodType)HttpConst::method_type_map.find(req_method)) < 0) {
				xthrownew(UnsupportedExcept(this, "HttpServer", "method", req_method));
			}
			for (; b < c && isspace(*b); b++);
			for (a = b; b < c && !isspace(*b); b++);
			req_uri.setUtf8(a, b-a);
			for (; b < c && isspace(*b); b++);
			if (b == c) {
				req_body_size = -1;
				req_version = 0x100;
				connection_state = 2;
				read_stream.ptrCache()->removeLeft(n+1);
				return req_uri;
			}
			for (; b < c && *b++ != '/';);
			if (b >= c) {
				req_version = 0x100;
			}
			else {
				req_version = strtol(b, (char **)&b, 0)*256;
				if (b >= c || *b++ != '.') {
					req_version = 0x100;
				}
				else {
					req_version += strtol(b, (char **)&b, 0);
				}
			}
			read_stream.ptrCache()->removeLeft(n+1);
		}

		for (;;) {
			ssize_t n = read_stream.readUntilChar('\n');
			if (n < 0) {
				xthrownew(EndOfDataExcept("http", "header"));
			}
			const char *b = read_stream.ptrCache()->toStr();
			const char *e = b+n;
			const char *c;
			for (; e > b && isspace(e[-1]); e--);
			if (e == b) {
				read_stream.ptrCache()->removeLeft(n+1);
				break;
			}
			for (c = b; c < e && *c != ':'; c++);
			if (c == e)
				xthrownew(EndOfDataExcept("http", ":"));
			String key(String::createLowerUtf8(b, c-b));
			for (c++; c < e && isspace(*c); c++);
			String val(String::createLowerUtf8(c, e-c));
			read_stream.ptrCache()->removeLeft(n+1);
			processHeader(key, val);
		}

		if (String *f = req_headers.accValue("content-length")) {
			req_body_size = strtol(f->utf8(), NULL, 0);
		}
		else {
			switch (method_type) {
			case HttpConst::MT_Head:
			case HttpConst::MT_Get:
			case HttpConst::MT_Delete:
			case HttpConst::MT_Connect:
			case HttpConst::MT_Options:
			case HttpConst::MT_Trace:
				req_body_size = 0;
				break;

			default:
				req_body_size = -1;
				break;
			}
		}

		if (String *f = req_headers.accValue("transfer-encoding")) {
			if (f->eqUtf8("chunked", 7)) {
				{
					ssize_t n = read_stream.readUntilChar('\n');
					req_body_size = strtol(read_stream.ptrCache()->toStr(), NULL, 16);
					read_stream.shrinkCache(n+1);
				}
				if (req_body_size == 0) {
					for (;;) {
						ssize_t n = read_stream.readUntilChar('\n');
						const char *b = read_stream.ptrCache()->toStr();
						const char *e = b+n;
						const char *c;
						for (; e > b && isspace(e[-1]); e--);
						if (e == b) {
							read_stream.ptrCache()->removeLeft(n+1);
							break;
						}
						for (c = b; c < e && *c != ':'; c++);
						if (c == e)
							xthrownew(EndOfDataExcept("http", ":"));
						String key(String::createLowerUtf8(b, c-b));
						for (c++; c < e && isspace(*c); c++);
						String val(String::createLowerUtf8(c, e-c));
						read_stream.ptrCache()->removeLeft(n+1);
					}
				}
				else {
					transfer_encoding = HttpConst::TE_Chunked;
				}
			}
		}

		if (connection_state == 0) {
			connection_state = req_version <= 0x100 ? 2 : 1;
		}
	}
	xcatchany {
		connection_state = 2;
		xrethrowany;
	}
	xend;
	return req_uri;
}

HttpConst::MethodType HttpServer::getMethod()
{
	return method_type;
}

void HttpServer::processHeader(const String &header, const String &value)
{
	req_headers[header] = value;
	if (void *func = header_process_map.findPtr(header)) {
		(this->*MethodConv::funcToMptr((void (*)(HttpServer *, const String &, const String &))func))(header, value);
	}
}

void HttpServer::processHeaderConnection(const String &header, const String &value)
{
	String lowvalue(String::createLower(value));
	if (lowvalue == "close")
		connection_state = 2;
	else if (lowvalue == "keep-alive")
		connection_state = 1;
	else
		connection_state = 2;
}

String HttpServer::getHeader(const String &key)
{
	if (String *f = req_headers.accValue(key))
		return *f;
	return Null();
}

bool HttpServer::updateNextChunk()
{
	{
		ssize_t n = read_stream.readUntilChar('\n');
		read_stream.shrinkCache(n+1);
	}
	{
		ssize_t n = read_stream.readUntilChar('\n');
		req_body_size = strtol(read_stream.ptrCache()->toStr(), NULL, 16);
		read_stream.shrinkCache(n+1);
	}
	if (req_body_size == 0) {
		for (;;) {
			ssize_t n = read_stream.readUntilChar('\n');
			const char *b = read_stream.ptrCache()->toStr();
			const char *e = b+n;
			const char *c;
			for (; e > b && isspace(e[-1]); e--);
			if (e == b) {
				read_stream.ptrCache()->removeLeft(n+1);
				break;
			}
			for (c = b; c < e && *c != ':'; c++);
			if (c == e)
				xthrownew(EndOfDataExcept("http", ":"));
			String key(String::createLowerUtf8(b, c-b));
			for (c++; c < e && isspace(*c); c++);
			String val(String::createLowerUtf8(c, e-c));
			read_stream.ptrCache()->removeLeft(n+1);
			//req_headers[key] = val;
		}
		transfer_encoding = HttpConst::TE_Basic;
		return false;
	}
	return true;
}

Blob HttpServer::readContent(size_t maxsize)
{
	Blob data;
	if (req_body_size == 0) {
		if (transfer_encoding == HttpConst::TE_Chunked) {
			if (!updateNextChunk())
				return Blob();
		}
		else {
			return Blob();
		}
	}
	if (req_body_size >= 0 && maxsize > req_body_size)
		maxsize = req_body_size;
	maxsize = read_stream.read(&data, maxsize);
	req_body_size -= maxsize;
	return data;
}

ssize_t HttpServer::readContent(Blob *content, size_t maxsize)
{
	if (req_body_size == 0) {
		if (transfer_encoding == HttpConst::TE_Chunked) {
			if (!updateNextChunk())
				return 0;
		}
		else {
			return 0;
		}
	}
	if (req_body_size >= 0 && maxsize > req_body_size)
		maxsize = req_body_size;
	maxsize = read_stream.read(content, maxsize);
	req_body_size -= maxsize;
	return maxsize;
}

Blob HttpServer::readFullContent()
{
	Blob data;
	if (req_body_size >= 0) {
		for (;;) {
			if (req_body_size == 0) {
				if (transfer_encoding == HttpConst::TE_Chunked) {
					if (!updateNextChunk())
						break;
				}
				else {
					break;
				}
			}
			req_body_size -= read_stream.readExtendAdd(&data, req_body_size);
		}
	}
	else {
		while (read_stream.tryExtendCacheAdd(1) > 0) ;
		data = read_stream.ptrCache()->cleanGet();
	}
	return data;
}

ssize_t HttpServer::readFullContent(Blob *content)
{
	if (req_body_size >= 0) {
		req_body_size -= read_stream.readFull(content, req_body_size);
	}
	else {
		while (read_stream.tryExtendCacheAdd(1) > 0);
		*content = read_stream.ptrCache()->cleanGet();
	}
	return content->getSize();
}

void HttpServer::setCloseConnection()
{
	connection_state = 2;
}

void HttpServer::sendResponse(int code, const String &message)
{
	BString resp("HTTP/");
	resp.appendNumber(req_version/256).append(".").appendNumber(req_version&255).append(" ");
	resp.appendNumber(code).append(" ").append(message.utf8()).append("\r\n");
	if (connection_state >= 2)
		resp.append("Connection: Close\r\n");
	else if (req_version < 0x101 && connection_state == 1) {
		resp.append("Connection: Keep-Alive\r\n");
	}
	read_stream.writeFull(resp);
}

void HttpServer::sendHeader(const String &header, const String &value)
{
	Blob row(header.utf8()); row.append(": ").append(value.utf8()).append("\r\n");
	read_stream.writeFull(row);
}

void HttpServer::sendContent(const Blob &content)
{
	BString cs("content-length: "); cs.appendNumber(content.getSize()).append("\r\n\r\n");
	read_stream.writeFull(cs);
	read_stream.writeFull(content);
	if (connection_state >= 2)
		((SocketStream *)read_stream.accHandle())->shutdown(3);
}


DR_NET_NS_END
