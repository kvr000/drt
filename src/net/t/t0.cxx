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
#include <dr/pe_error.hxx>

#include <dr/Ref.hxx>
#include <dr/Exception.hxx>
#include <dr/ThreadSimple.hxx>

#include <dr/io/SocketServer.hxx>
#include <dr/io/SocketStream.hxx>
#include <dr/io/NetAddressInet4.hxx>

#include <dr/net/HttpClient.hxx>
#include <dr/net/HttpServer.hxx>
#include <dr/net/RpcDecoder.hxx>
#include <dr/net/XmlRpcEncoder.hxx>

#include <dr/tenv/tenv.hxx>

DR_IO_NS_USE;
DR_NET_NS_USE;
DR_TENV_NS_USE


#define TEST_HTTP
#define TEST_RPC

#ifdef TEST_HTTP
TENV_NS(http);

static void runServer(Ref<SocketServer> sserver)
{
	ERef<HttpServer> hserver(new HttpServer(tref(sserver->accept())));
	xtry {
		Blob body;
		hserver->setTimeLimit(Time::getTime()+Time::interFromSecondsUp(10));
		hserver->readRequest();
		body = hserver->readFullContent();
		hserver->sendResponse(200, "ok");
		body = Blob("resp:")+body;
		hserver->sendContent(body);
	}
	xcatch (Exception, ex) {
		Fatal::plog("http exception: %s\n", ex->stringify().utf8().toStr());
	}
	xend;
}

void createClientServer(Ref<HttpClient> *hclient, Ref<Thread> *hserver_thr)
{
	int sserver_port;

	ERef<SocketServer> sserver(new SocketServer(Socket::domain_inet4, Socket::proto_tcp));
	sserver->bind(tref(new NetAddressInet4(0, 0)));
	sserver->listen(-1);
	sserver_port = tref((NetAddressInet4 *)sserver->getLocalAddress()->getIfaceUnref(NetAddressInet4::comp_name))->getPort();
	hserver_thr->setNoref(ThreadSimple::go(Eslot(&runServer).a1Set(sserver)));

	String addr("localhost:"); addr.appendNumber(sserver_port);
	hclient->setNoref(new HttpClient(addr));
	(*hclient)->setTimeLimit(Time::getTime()+Time::interFromSecondsUp(10));
}

void testBasic()
{
	Ref<Thread> hserver_thr;
	xtry {
		Ref<HttpClient> hclient;
		createClientServer(&hclient, &hserver_thr);
		hclient->sendRequest("GET", "/", None(), Null());
		hclient->readHeaders(NULL);
		TENV_CHECK(hclient->readFullContent() == "resp:");
	}
	xcatch (Exception, ex) {
		Fatal::plog("http exception: %s\n", ex->stringify().utf8().toStr());
		return;
	}
	xend;
	if (hserver_thr)
		hserver_thr->wait();
}

void testChunked()
{
	Ref<Thread> hserver_thr;
	xtry {
		Ref<HttpClient> hclient;
		createClientServer(&hclient, &hserver_thr);
		hclient->partialRequest("POST", "/", None());
		hclient->partialStartBodyChunked();
		hclient->partialBodyChunked("part0");
		hclient->partialBodyChunked("next part longer than 16 bytes");
		hclient->partialFinishBodyChunked();
		hclient->readHeaders(NULL);
		TENV_CHECK(hclient->readFullContent() == "resp:part0next part longer than 16 bytes");
	}
	xcatch (Exception, ex) {
		Fatal::plog("http exception: %s\n", ex->stringify().utf8().toStr());
	}
	xend;
	if (hserver_thr)
		hserver_thr->wait();
}

void test()
{
	testBasic();
	testChunked();
	if (0) {
		Thread *perf_test[192];
		for (size_t i = 0; i < sizeof(perf_test)/sizeof(perf_test[0]); i++) {
			perf_test[i] = ThreadSimple::go(Eslot(&testBasic));
		}
		for (size_t i = 0; i < sizeof(perf_test)/sizeof(perf_test[0]); i++) {
			perf_test[i]->waitUnref();
		}
	}
}
TENV_NSE(http);
#endif


#ifdef TEST_RPC
TENV_NS(rpc);

void test()
{
	ssize_t sz;
	Blob rpc_call("<?xml version=\"1.0\"?><methodCall><methodName>some.unknown</methodName><params>"
			"<param><value><i4>534</i4></value></param>"
			"<param><value><array><data><value><string>bla&gt;&#10;</string></value></data></array></value></param>"
			"<param><value><struct><member><name>id</name><value><boolean>false</boolean></value></member><member><name>val</name><value><double>1.5</double></value></member></struct></value></param>"
			"<param><value><struct></struct></value></param>"
			"<param><value><base64>YWJjZA==</base64></value></param>"
			"<param><value><dateTime.iso8601>19990101T12:34:56+0100</dateTime.iso8601></value></param>"
			"</params></methodCall>");
	Ref<RpcDecoder> decoder;
	
	decoder.setNoref(RpcDecoder::createDecoder(rpc_call));
	TENV_CHECK(decoder->readHeader() == 0);
	TENV_CHECK(decoder->readMethodName() == "some.unknown");
	TENV_CHECK(decoder->readInt32() == 534);
	sz = decoder->readArrayLength();
	TENV_CHECK(decoder->checkArrayNext(&sz));
	TENV_CHECK(decoder->readString() == "bla>\n");
	TENV_CHECK(!decoder->checkArrayNext(&sz));
	sz = decoder->readStructLength();
	TENV_CHECK(decoder->checkStructNext(&sz));
	TENV_CHECK(decoder->readMemberName() == "id");
	TENV_CHECK(!decoder->readBool());
	TENV_CHECK(decoder->checkStructNext(&sz));
	TENV_CHECK(decoder->readMemberName() == "val");
	TENV_CHECK(decoder->readDouble() == 1.5);
	TENV_CHECK(!decoder->checkStructNext(&sz));
	sz = decoder->readStructLength();
	TENV_CHECK(!decoder->checkStructNext(&sz));
	TENV_CHECK(decoder->readBinary() == "abcd");
	TENV_CHECK(decoder->readTime() == Time::fromSeconds(915190496));

	Blob rpc_resp("<?xml version=\"1.0\"?><methodResponse><params><param><value><string>abcd</string></value></param></params></methodResponse>");
	decoder.setNoref(RpcDecoder::createDecoder(rpc_resp));
	TENV_CHECK(decoder->readHeader() == 1);
	decoder->readMethodResponse();
	TENV_CHECK(decoder->readString() == "abcd");

	String fault_str;
	Blob rpc_fault("<?xml version=\"1.0\"?><methodResponse><fault><value><struct><member><name>faultCode</name><value><int>5</int></value></member><member><name>faultString</name><value><string>unknown</string></value></member></struct></value></fault></methodResponse>");
	decoder.setNoref(RpcDecoder::createDecoder(rpc_fault));
	TENV_CHECK(decoder->readHeader() == 2);
	TENV_CHECK(decoder->readFaultResponse(&fault_str) == 5);
	TENV_CHECK(fault_str == "unknown");

	ERef<RpcEncoder> encoder(new XmlRpcEncoder);
	encoder->writeHeader();
	encoder->writeMethodName("some.unknown");
	encoder->writeInt(534);
	encoder->writeArrayLength(1);
	encoder->writeString("bla");
	encoder->writeArrayEnd();
	encoder->writeStructLength(2);
	encoder->writeMemberName("id");
	encoder->writeBool(false);
	encoder->writeMemberName("val");
	encoder->writeDouble(1.5);
	encoder->writeStructEnd();
	encoder->writeStructLength(0);
	encoder->writeStructEnd();
	encoder->writeBinary("abcd");
	encoder->writeTime(Time::fromSeconds(915190496));

	printf("%s\n", encoder->getContent().toStr());

}

TENV_NSE(rpc);
#endif


DR_TENV_MAIN()
{
	tenv_init();
#ifdef TEST_HTTP
	TENV_RUN(http);
#endif
#ifdef TEST_RPC
	TENV_RUN(rpc);
#endif
	return 0;
}
