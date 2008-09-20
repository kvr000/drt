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
 * include:	dr/Exception.hxx
 * include:	dr/net/RpcDecoder.hxx
 * include:	dr/net/RpcEncoder.hxx
 * include:	dr/net/HttpServer.hxx
 *
 * ns:		dr::net
 */

/*drt
 * class:	RpcServer
 * type:	object
 * ancestor:	dr::Object
 *
 * at:	Ref<HttpServer>			client_http;
 *
 * cnp:	String				system_listMethods_name("system.listMethods");
 */

#include <dr/x_kw.hxx>
#include <dr/MethodConv.hxx>
#include <dr/List.hxx>
#include <dr/net/XmlRpcEncoder.hxx>
#include <dr/net/FastRpcDecoder.hxx>
#include <dr/net/FastRpcEncoder.hxx>

#include <dr/net/RpcServer.hxx>
#include "_gen/RpcServer-all.hxx"

DR_NET_NS_BEGIN


DR_MET(public)
RpcServer::RpcServer(dr::io::SocketStream *client_conn_):
	client_http(new dr::net::HttpServer(client_conn_), false)
{
	client_http->setTimeLimit(Time::getTime()+Time::interFromSecondsUp(4));
}

DR_MET(public)
RpcServer::RpcServer(dr::net::HttpServer *client_http_):
	client_http(client_http_, true)
{
}

DR_MET(protected virtual)
RpcServer::~RpcServer()
{
}

DR_MET(protected abstract)
//const StringIndex *RpcServer::getMethodWrappers()

DR_MET(protected virtual)
void RpcServer::allocateResources(const String &method_name)
{
}

DR_MET(protected virtual)
void RpcServer::releaseResources()
{
}

DR_MET(protected virtual)
void RpcServer::reportError(const String &method_name, const String &error_desc, const Blob &content)
{
}

DR_MET(protected virtual)
void RpcServer::processUnknownMethod(const String &method_name, RpcEncoder *encoder)
{
	reportError(method_name, String("unknown method called"), encoder->getContent());
	encoder->writeHeader();
	encoder->writeFaultResponse(400, "Unknown method");
}

DR_MET(protected virtual)
void RpcServer::processException(const String &method_name, RpcEncoder *encoder, dr::Exception *ex)
{
	reportError(method_name, String("failed with exception: ").append(ex->stringify()), encoder->getContent());
	encoder->writeHeader();
	encoder->writeFaultResponse(1, String("method failed: ")+ex->stringify());
}

DR_MET(protected virtual)
void RpcServer::processFailure(const String &method_name, RpcEncoder *encoder, int fault_code)
{
	reportError(method_name, String("failed with error: ").appendNumber(fault_code), encoder->getContent());
	encoder->writeHeader();
	encoder->writeFaultResponse(fault_code, "method failed");
}

DR_MET(protected virtual)
void RpcServer::startResultStruct(dr::net::RpcEncoder *result, size_t member_cnt)
{
	result->writeHeader();
	result->writeMethodResponse();
	result->writeStructLength(member_cnt);
}

DR_MET(protected virtual)
void RpcServer::endResultStruct(dr::net::RpcEncoder *result)
{
	result->writeStructEnd();
	result->writeResponseEnd();
}

DR_MET(protected virtual)
void RpcServer::startResultArray(dr::net::RpcEncoder *result, size_t array_cnt)
{
	result->writeHeader();
	result->writeMethodResponse();
	result->writeArrayLength(array_cnt);
}

DR_MET(protected virtual)
void RpcServer::endResultArray(dr::net::RpcEncoder *result)
{
	result->writeArrayEnd();
	result->writeResponseEnd();
}

DR_MET(protected virtual)
void RpcServer::writeEmptyResult(RpcEncoder *result)
{
	this->startResultStruct(result, 0);
	this->endResultStruct(result);
}

DR_MET(protected virtual)
int RpcServer::execute_system_listMethods(dr::net::RpcEncoder *result, dr::net::RpcDecoder *params)
{
	SList<String> methods;
	getMethodWrappers()->appendToList(&methods);
	result->writeHeader();
	result->writeMethodResponse();
	result->writeArrayLength(methods.count());
	while (SList<String>::Node *n = methods.iterFirst()) {
		result->writeString(n->v);
		methods.remove(n);
	}
	result->writeArrayEnd();
	result->writeResponseEnd();
	return 0;
}

DR_MET(public virtual)
void RpcServer::run()
{
	xtry {
		for (;;) {
			client_http->setTimeLimit(Time::getTime()+Time::interFromSecondsUp(10));
			String req_uri = client_http->readRequest();
			if (req_uri.isNull())
				break;
			Blob body = client_http->readFullContent();
			xtry {
				ERef<RpcDecoder> decoder(RpcDecoder::createDecoder(body));
				ERef<RpcEncoder> encoder;
				encoder.setNoref(client_http->getHeader("accept").find(FastRpcEncoder::mime_type_string) >= 0 || decoder->classname() == FastRpcDecoder::comp_name ? (RpcEncoder *)new FastRpcEncoder : (RpcEncoder *)new XmlRpcEncoder);
				xtry {
					decoder->readHeader();
					String method = decoder->readMethodName();
					int (RpcServer::*func_handler)(dr::net::RpcEncoder *result, dr::net::RpcDecoder *params);
					if ((func_handler = MethodConv::funcToMptr((int (*)(RpcServer *this_, dr::net::RpcEncoder *result, dr::net::RpcDecoder *params))getMethodWrappers()->findPtr(method)))) {
						allocateResources(method);
						xtry {
							int error = (this->*func_handler)(encoder, decoder);
							if (error != 0) {
								processFailure(method, encoder, error);
							}
						}
						xcatch (dr::Exception, ex) {
							processException(method, encoder, ex);
						}
						xend;
						releaseResources();
					}
					else {
						processUnknownMethod(method, encoder);
					}
				}
				xcatchany {
					xsafe(releaseResources());
					xrethrowany;
				}
				xend;
				client_http->sendResponse(200, "OK");
				client_http->sendHeader("content-type", encoder->getMimeType());
				client_http->sendContent(encoder->getContent());
			}
			xcatch (Exception, ex) {
				client_http->sendResponse(400, "Bad Request");
				client_http->sendHeader("content-type", "text/plain; charset=utf8");
				client_http->sendContent(BString("Bad request, maybe not binary fast rpc call?: ")+ex->stringify().utf8());
			}
			xend;
		}
	}
	xcatch (dr::Exception, ex) {
	}
	xend;
}


DR_NET_NS_END
