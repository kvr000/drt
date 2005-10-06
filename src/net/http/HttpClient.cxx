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

/*drt
 * include:	dr/Hash.hxx
 * include:	dr/io/StreamBuffer.hxx
 * include:	dr/io/NetAddress.hxx
 * include:	dr/io/SocketStream.hxx
 * include:	dr/net/HttpConst.hxx
 *
 * ns:		dr::net
 */

#include <dr/x_kw.hxx>
#include <dr/Const.hxx>

#include <dr/EndOfDataException.hxx>
#include <dr/InvalidFormatException.hxx>

#include <dr/io/NetAddressInet4.hxx>

#include <dr/net/HttpException.hxx>

#include <dr/net/HttpClient.hxx>
#include "_gen/HttpClient-all.hxx"

DR_NET_NS_BEGIN


/*drt
 * class:	HttpClient
 * type:	object
 * ancestor:	dr::Object
 *
 * at:	StreamBuffer			read_stream;
 * at:	String				host;
 * at:	THash<String, String>		response_headers;
 * at:	Sint64				response_size;
 */

DR_MET(public)
HttpClient::HttpClient(const String &server_address_):
	read_stream(NULL),
	host(server_address_)
{
	if (host.find(":") < 0)
		host += ":80";
	createConnection(tref(new NetAddressInet4(host)));
}

DR_MET(public)
HttpClient::HttpClient(NetAddress *server_address):
	read_stream(NULL)
{
	createConnection(server_address);
}

DR_MET(public virtual)
void HttpClient::setTimeLimit(Time::SysTime abs_time)
{
	read_stream.setTimeLimit(abs_time);
}

DR_MET(public virtual)
int HttpClient::readHeaders(String *message)
{
	int resp_code;
	{
		ssize_t n = read_stream.readUntilChar('\n');
		const char *b = read_stream.ptrCache()->toStr();
		char *c;
		for (c = (char *)b+n; b < c && !isspace(*b); b++) ;
		if (b == c)
			xthrownew(EndOfDataException("http", "status"));
		resp_code = strtol(b, &c, 10);
		for (b += n; b > c && isspace(b[-1]); b--) ;
		for (; c < b && isspace(*c); c++) ;
		if (message)
			message->setUtf8(c, b-c);
		read_stream.ptrCache()->removeLeft(n+1);
	}

	for (;;) {
		ssize_t n = read_stream.readUntilChar('\n');
		const char *b = read_stream.ptrCache()->toStr();
		const char *e = b+n;
		const char *c;
		for (; e > b && isspace(e[-1]); e--) ;
		if (e == b) {
			read_stream.ptrCache()->removeLeft(n+1);
			break;
		}
		for (c = b; c < e && *c != ':'; c++) ;
		if (c == e)
			xthrownew(EndOfDataException("http", ":"));
		String key(String::createLowerUtf8(b, c-b));
		for (c++; c < e && isspace(*c); c++) ;
		String val(String::createLowerUtf8(c, e-c));
		read_stream.ptrCache()->removeLeft(n+1);
		response_headers[key] = val;
	}

	if (String *f = response_headers.accValue("content-length"))
		response_size = strtol(f->utf8(), NULL, 0);
	else
		response_size = -1;

	return resp_code;
}

DR_MET(public virtual)
String HttpClient::getHeader(const String &key)
{
	if (String *f = response_headers.accValue(key))
		return *f;
	return Null();
}

DR_MET(public virtual)
Blob HttpClient::readContent(size_t maxsize)
{
	Blob data;
	read_stream.read(&data, maxsize);
	return data;
}

DR_MET(public virtual)
ssize_t HttpClient::readContent(Blob *content, size_t maxsize)
{
	return read_stream.read(content, maxsize);
}

DR_MET(public virtual)
Blob HttpClient::readFullContent()
{
	Blob data;
	if (response_size >= 0) {
		read_stream.readFull(&data, response_size);
	}
	else {
		while (read_stream.tryExtendCacheAdd(1) > 0) ;
		data = read_stream.ptrCache()->cleanGet();
	}
	return data;
}

DR_MET(public virtual)
ssize_t HttpClient::readFullContent(Blob *content)
{
	if (response_size >= 0) {
		read_stream.readFull(content, response_size);
	}
	else {
		while (read_stream.tryExtendCacheAdd(1) > 0) ;
		*content = read_stream.ptrCache()->cleanGet();
	}
	return content->getSize();
}

DR_MET(protected virtual)
HttpClient::~HttpClient()
{
}

DR_MET(public virtual)
void HttpClient::sendRequest(const String &method, const String &uri, const THash<String, String> &headers, const Blob &content)
{
	BString request;
	request.append(method.isNullEmpty() ? "GET" : method.utf8().toStr()).append(" ").append(uri.utf8()).append(" HTTP/1.0\r\n");
	if (String *v = headers.accValue("host")) {
		(void)v;
		//request += "host: "; request += v->utf8(); request += "\r\n";
	}
	else if (!host.isNullEmpty()) {
		request.append("host: ").append(host.utf8()).append("\r\n");
	}
	for (THash<String, String>:: Node *f = headers.iterFirst(); f; f = headers.iterNext(f)) {
		request.append(f->k.utf8()).append(": ").append(f->v.utf8()).append("\r\n");
	}
	if (!content.isNull()) {
		request.append("content-length: ").appendNumber(content.getSize()).append("\r\n");
	}
	request.append("\r\n");
	read_stream.writeFull(request);
	read_stream.writeFull(content);
	((SocketStream *)read_stream.accHandle())->shutdown(2);
}

DR_MET(public virtual)
void HttpClient::partialRequest(const String &method, const String &uri, const THash<String, String> &headers)
{
	BString request;
	request.append(method.isNullEmpty() ? "GET" : method.utf8().toStr()).append(" ").append(uri.utf8()).append(" HTTP/1.0\r\n");
	if (String *v = headers.accValue("host")) {
		(void)v;
		//request += "host: "; request += v->utf8(); request += "\r\n";
	}
	else if (!host.isNullEmpty()) {
		request.append("host: ").append(host.utf8()).append("\r\n");
	}
	for (THash<String, String>:: Node *f = headers.iterFirst(); f; f = headers.iterNext(f)) {
		request.append(f->k.utf8()).append(": ").append(f->v.utf8()).append("\r\n");
	}
	read_stream.writeFull(request);
}

DR_MET(public virtual)
void HttpClient::partialHeader(const String &key, const String &value)
{
	BString request(key.utf8());
	request.append(": ").append(value.utf8()).append("\r\n");
	read_stream.writeFull(request);
}

DR_MET(public virtual)
void HttpClient::partialNoBody()
{
	BString request("\r\n");
	read_stream.writeFull(request);
	((SocketStream *)read_stream.accHandle())->shutdown(2);
}

DR_MET(public virtual)
void HttpClient::partialSendBody(const Blob &content)
{
	BString request("content-length: ");
	request.appendNumber(content.getSize()).append("\r\n\r\n");
	read_stream.writeFull(request);
	read_stream.writeFull(content);
}

DR_MET(public virtual)
void HttpClient::partialStartBodyChunked()
{
	BString request("transfer-encoding: chunked\r\n\r\n");
	read_stream.writeFull(request);
}

DR_MET(public virtual)
void HttpClient::partialBodyChunked(const Blob &data)
{
	BString request; request.appendNumber(data.getSize(), 16).append("\r\n");
	read_stream.writeFull(request);
	read_stream.writeFull(data);
	read_stream.writeFull(Blob("\r\n"));
}

DR_MET(public virtual)
void HttpClient::partialFinishBodyChunked()
{
	BString request("0\r\n\r\n");
	read_stream.writeFull(request);
}

DR_MET(public static)
Blob HttpClient::processEasy(const String &url)
{
	int code;
	String msg;
	String addr;
	String uri;
	if (url.find("http://") != 0)
		xthrownew(InvalidFormatException("URL", url));
	ssize_t uri_p;
	if ((uri_p = url.find("/", 7)) < 0) {
		addr = url.mid(7);
		uri = "/";
	}
	else {
		addr = url.mid(7, uri_p-7);
		uri = url.mid(uri_p);
	}

	HttpClient client(addr);
	client.sendRequest("GET", uri, THash<String, String>(), Null());
	if ((code = client.readHeaders(&msg)) != 200) {
		xthrownew(HttpException(code, msg));
	}
	return client.readFullContent();
}

DR_MET(protected virtual)
void HttpClient::createConnection(NetAddress *addr)
{
	ERef<SocketStream> server_conn(new SocketStream(SocketStream::domain_inet4, Socket::proto_tcp));
	server_conn->connect(addr);
	read_stream.reinit(server_conn);
}


DR_NET_NS_END
