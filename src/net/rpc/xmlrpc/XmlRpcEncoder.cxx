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
#include <dr/Mem.hxx>

#include <dr/EndOfDataExcept.hxx>
#include <dr/OverflowExcept.hxx>
#include <dr/InvalidFormatExcept.hxx>
#include <dr/MemBase64.hxx>

#include <dr/io/NetAddressInet4.hxx>

#include <dr/net/XmlRpcEncoder.hxx>

DR_NET_NS_BEGIN


DR_OBJECT_DEF(DR_NET_NS_STR, XmlRpcEncoder, RpcEncoder);
DR_OBJECT_IMPL_SIMPLE(XmlRpcEncoder);


const String XmlRpcEncoder::mime_type_string(Const::string("text/xml"));

XmlRpcEncoder::XmlRpcEncoder():
	value_level(0)
{
}

XmlRpcEncoder::~XmlRpcEncoder()
{
}

String XmlRpcEncoder::getMimeType()
{
	return mime_type_string;
}

void XmlRpcEncoder::writeHeader()
{
	content.append("<?xml version=\"1.0\"?>\n");
}

void XmlRpcEncoder::writeMethodName(const String &name)
{
	content.append("<methodCall><methodName>").append(name.utf8()).append("</methodName><params>");
}

void XmlRpcEncoder::writeMethodResponse()
{
	content.append("<methodResponse><params>");
}

void XmlRpcEncoder::writeFaultResponse(long code, const String &message)
{
	content.append("<methodResponse><fault><value><struct><member><name>faultCode</name><value><int>").appendNumber((Sint64)code).
		append("</int></value></member><member><name>faultString</name><value><string>").append(message.utf8()).
		append("</string></value></member></struct></value></fault></methodResponse>");
}

void XmlRpcEncoder::writeInt(Sint32 value)
{
	if (value_level == 0)
		content.append("<param>");
	content.append("<value><int>").appendNumber(value).append("</int></value>");
	if (value_level == 0)
		content.append("</param>");
}

void XmlRpcEncoder::writeInt(Sint64 value)
{
	if (value_level == 0)
		content.append("<param>");
	if (value >= -0x80000000 && value <= 0x7fffffff)
		content.append("<value><int>").appendNumber((Sint32)value).append("</int></value>");
	else
		content.append("<value><i8>").appendNumber(value).append("</i8></value>");
	if (value_level == 0)
		content.append("</param>");
}

void XmlRpcEncoder::writeBool(bool value)
{
	if (value_level == 0)
		content.append("<param>");
	content.append("<value><boolean>").append(value ? "true" : "false").append("</boolean></value>");
	if (value_level == 0)
		content.append("</param>");
}

void XmlRpcEncoder::writeDouble(double value)
{
	char strv[32];
	sprintf(strv, "%g", value);
	if (value_level == 0)
		content.append("<param>");
	content.append("<value><double>").append(strv).append("</double></value>");
	if (value_level == 0)
		content.append("</param>");
}

void XmlRpcEncoder::writeString(const String &str)
{
	if (value_level == 0)
		content.append("<param>");
	content.append("<value><string>").append(str.utf8()).append("</string></value>");
	if (value_level == 0)
		content.append("</param>");
}

void XmlRpcEncoder::writeBinary(const Blob &data)
{
	if (value_level == 0)
		content.append("<param>");
	content.append("<value><base64>").append(MemBase64::encodeBase64(data)).append("</base64></value>");
	if (value_level == 0)
		content.append("</param>");
}

void XmlRpcEncoder::writeTime(SysTime tvalue)
{
	int year, mon, mday, hour, min, sec;
	char c[28];
	Time::timeToUtcCalendar(tvalue, &year, &mon, &mday, &hour, &min, &sec);
	sprintf(c, "%04d%02d%02dT%02d:%02d:%02d+0000", year, mon+1, mday+1, hour, min, sec);
	if (value_level == 0)
		content.append("<param>");
	content.append("<value><dateTime.iso8601>").append(c).append("</dateTime.iso8601></value>");
	if (value_level == 0)
		content.append("</param>");
}

void XmlRpcEncoder::writeArrayLength(size_t length)
{
	if (value_level == 0)
		content.append("<param>");
	content.append("<value><array><data>");
	value_level++;
}

void XmlRpcEncoder::writeStructLength(size_t length)
{
	if (value_level == 0)
		content.append("<param>");
	content.append("<value><struct>");
	struct_clean = true;
	value_level++;
}

void XmlRpcEncoder::writeMemberName(const String &name)
{
	if (!struct_clean) {
		content.append("</member>");
	}
	else {
		struct_clean = false;
	}
	content.append("<member><name>").append(name.utf8()).append("</name>");
}

void XmlRpcEncoder::writeArrayEnd()
{
	content.append("</data></array></value>");
	if (--value_level == 0)
		content.append("</param>");
}

void XmlRpcEncoder::writeStructEnd()
{
	if (!struct_clean) {
		content.append("</member>");
	}
	else {
		struct_clean = false;
	}
	content.append("</struct></value>");
	if (--value_level == 0)
		content.append("</param>");
}

void XmlRpcEncoder::writeMethodEnd()
{
	content.append("</params></methodCall>");
}

void XmlRpcEncoder::writeResponseEnd()
{
	content.append("</params></methodResponse>");
}


DR_NET_NS_END
