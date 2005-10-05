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

#include <dr/EndOfDataException.hxx>
#include <dr/OverflowException.hxx>
#include <dr/InvalidFormatException.hxx>
#include <dr/MemBase64.hxx>

#include <dr/io/NetAddressInet4.hxx>
#include <dr/net/RpcHelper.hxx>

#include <dr/net/XmlRpcDecoder.hxx>

DR_NET_NS_BEGIN


DR_OBJECT_DEF(DR_NET_NS_STR, XmlRpcDecoder, RpcDecoder);
DR_OBJECT_IMPL_SIMPLE(XmlRpcDecoder);


XmlRpcDecoder::XmlRpcDecoder():
	empty_el(NULL)
{
}

XmlRpcDecoder::~XmlRpcDecoder()
{
}

/**
 * moves the pos to the next element, returning the content in start/length
 *
 * @return 0
 * 	if the starting element was found
 * @return 1
 * 	if the end element was found
 */
int XmlRpcDecoder::moveToNextElement(const char **start, size_t *length)
{
	if (empty_el != NULL) {
		pos = empty_el;
		empty_el = NULL;
		for (;; pos++) {
			if (pos == end)
				xthrownew(EndOfDataException("char", "element name"));
			if (isalnum(*pos) || *pos == '_' || *pos == '.')
				continue;
			break;
		}
		*length = (const char *)pos-*start;
		for (; ; pos++) {
			if (pos == end)
				xthrownew(EndOfDataException("char", "element"));
			if (isspace(*pos))
				continue;
			break;
		}
		if (*pos != '/') {
			xthrownew(InvalidFormatException("end tag", "/"));
		}
		pos++;
		for (; ; pos++) {
			if (pos == end)
				xthrownew(EndOfDataException("char", "element"));
			if (isspace(*pos))
				continue;
			break;
		}
		if (*pos != '>') {
			xthrownew(InvalidFormatException("end tag", ">"));
		}
		pos++;
		return 1;
	}
	for (;;) {
next_step:
		if (pos == end) {
			xthrownew(EndOfDataException("char", "element"));
		}
		if (isspace(*pos)) {
			pos++;
			continue;
		}
		switch (*pos) {
		case '<':
			for (++pos; ; pos++) {
				if (pos == end)
					xthrownew(EndOfDataException("char", "element"));
				if (isspace(*pos))
					continue;
				break;
			}
			if (*pos == '/') {
				for (++pos; ; pos++) {
					if (pos == end)
						xthrownew(EndOfDataException("char", "element"));
					if (isspace(*pos))
						continue;
					break;
				}
				*start = (const char *)pos;
				for (; ; pos++) {
					if (pos == end)
						xthrownew(EndOfDataException("char", "element name"));
					if (isalnum(*pos) || *pos == '_' || *pos == '.')
						continue;
					break;
				}
				*length = (const char *)pos-*start;
				for (; ; pos++) {
					if (pos == end)
						xthrownew(EndOfDataException("char", "element"));
					if (isspace(*pos))
						continue;
					break;
				}
				if (*pos != '>') {
					xthrownew(InvalidFormatException("end tag", ">"));
				}
				pos++;
				return 1;
			}
			else if (*pos == '!') {
				if (pos < end+3 && memcmp(++pos, "--", 2) == 0) {
					for (pos += 2; ; pos++) {
						if (pos >= end-2)
							xthrownew(EndOfDataException("char", "element"));
						if (pos[0] == '-' && pos[1] == '-' && pos[2] == '>') {
							pos += 3;
							goto next_step;
						}
					}
				}
				else {
					xthrownew(InvalidFormatException("comment", "--"));
				}
			}
			else {
				for (; ; pos++) {
					if (pos == end)
						xthrownew(EndOfDataException("char", "element"));
					if (isspace(*pos))
						continue;
					break;
				}
				*start = (const char *)pos;
				for (; ; pos++) {
					if (pos == end)
						xthrownew(EndOfDataException("char", "element name"));
					if (isalnum(*pos) || *pos == '_' || *pos == '.')
						continue;
					break;
				}
				*length = (const char *)pos-*start;
				for (; ; pos++) {
					if (pos == end)
						xthrownew(EndOfDataException("char", "element"));
					if (isspace(*pos))
						continue;
					break;
				}
				if (*pos == '/') {
					empty_el = (const unsigned char *)*start;
					for (++pos; ; pos++) {
						if (pos == end)
							xthrownew(EndOfDataException("char", "element"));
						if (isspace(*pos))
							continue;
						break;
					}
				}
				if (*pos != '>') {
					xthrownew(InvalidFormatException("end tag", ">"));
				}
				pos++;
				return 0;
			}
			break;

		default:
			xthrownew(InvalidFormatException("xml", "unexpected character"));
		}
	}
}

size_t XmlRpcDecoder::moveToNextValue(const char **type_start)
{
	for (;;) {
		size_t type_length;
		if (moveToNextElement(type_start, &type_length) != 0) {
			xthrownew(InvalidFormatException("xmlrpc::value", String((const char *)pos, end-pos)));
		}
		if (type_length == 5 && memcmp(*type_start, "value", type_length) == 0) {
			if (moveToNextElement(type_start, &type_length) != 0) {
				xthrownew(InvalidFormatException("xmlrpc::value", String(*type_start, type_length)));
			}
			return type_length;
		}
		if (type_length == 5 && memcmp(*type_start, "param", type_length) == 0)
			continue;
		xthrownew(InvalidFormatException("xmlrpc::value", String(*type_start, type_length)));
	}
}

void XmlRpcDecoder::skipValueEnd(const char *type_start, size_t type_length)
{
	const char *el_start;
	size_t el_length;
	if (moveToNextElement(&el_start, &el_length) != 1) {
		xthrownew(InvalidFormatException("xmlrpc", String("endvalue")));
	}
	moveToNextElementReqEnd("value");
	const unsigned char *save_pos = pos;
	if (moveToNextElement(&el_start, &el_length) != 1 || el_length != 5 || memcmp(el_start, "param", el_length) != 0) {
		pos = save_pos;
	}
}

void XmlRpcDecoder::moveToNextElementReqStart(const BString &tag_name)
{
	const char *start;
	size_t length;
	if (moveToNextElement(&start, &length) != 0 || length != tag_name.getSize() || memcmp(start, tag_name, length) != 0) {
		xthrownew(InvalidFormatException("xmlrpc", String(tag_name)));
	}
}

void XmlRpcDecoder::moveToNextElementReqEnd(const BString &tag_name)
{
	const char *start;
	size_t length;
	if (moveToNextElement(&start, &length) != 1 || length != tag_name.getSize() || memcmp(start, tag_name, length) != 0) {
		xthrownew(InvalidFormatException("xmlrpc", String(tag_name)));
	}
}

void XmlRpcDecoder::readContent(const char **cont_start, size_t *cont_length)
{
	*cont_start = (const char *)pos;
	if (empty_el == NULL) {
		for (; pos < end && *pos != '<'; pos++) ;
	}
	*cont_length = (const char *)pos-*cont_start;
}

String XmlRpcDecoder::convertToString(const char *str, size_t len)
{
	BString out;
	char *os = out.lock(len);
	while (len-- > 0) {
		switch (*str++) {
		case '&':
			if (memcmp(str, "lt;", 3) == 0) {
				*os++ = '<';
			}
			else if (memcmp(str, "gt;", 3) == 0) {
				*os++ = '>';
			}
			else if (memcmp(str, "quot;", 3) == 0) {
				*os++ = '\"';
			}
			else {
				xthrownew(InvalidFormatException("xmlspecial", "&"));
			}
			break;

		default:
			*os++ = str[-1];
			break;
		}
	}
	out.unlock(os-out.toStr());
	return String(out);
}

int XmlRpcDecoder::readHeader()
{
	const char *el_start;
	size_t el_length;
	if (pos+5 >= end) {
		xthrownew(InvalidFormatException("rpc::header", "incorrect length"));
	}
	if (memcmp(pos, "<?xml", 5) != 0)
		xthrownew(InvalidFormatException("rpc::header", "unexpected header"));
	pos += 5;
	for (;;) {
		if (pos >= end) {
			xthrownew(InvalidFormatException("rpc::header", "missing end of element"));
		}
		if (*pos++ == '>')
			break;
	}
	const unsigned char *save_pos = pos;
	if (moveToNextElement(&el_start, &el_length) != 0) {
		xthrownew(InvalidFormatException("xmlrpc", "header"));
	}
	if (el_length == 10 && memcmp(el_start, "methodCall", el_length) == 0) {
		pos = save_pos;
		return 0;
	}
	else if (el_length == 14 && memcmp(el_start, "methodResponse", el_length) == 0) {
		if (moveToNextElement(&el_start, &el_length) != 0) {
			xthrownew(InvalidFormatException("xmlrpc", "header"));
		}
		if (el_length == 6 && memcmp(el_start, "params", el_length) == 0) {
			pos = save_pos;
			return 1;
		}
		else if (el_length == 5 && memcmp(el_start, "fault", el_length) == 0) {
			pos = save_pos;
			return 2;
		}
		else {
			xthrownew(InvalidFormatException("xmlrpc", "header"));
			return -1;
		}
	}
	else {
		xthrownew(InvalidFormatException("xmlrpc", "header"));
		return -1;
	}
}

RpcDecoder::RpcType XmlRpcDecoder::readCheckType()
{
retry_element:
	const unsigned char *save_pos = pos;
	const char *el_start;
	size_t el_length;
	if (pos >= end)
		return RT_End;
	switch (moveToNextElement(&el_start, &el_length)) {
	case -1:
		xthrownew(InvalidFormatException("xmlrpc", "value"));
		return RT_End;

	case 0:
		if (el_length == 5 && memcmp(el_start, "param", el_length) == 0)
			goto retry_element;
		if (el_length == 5 && memcmp(el_start, "value", el_length) == 0) {
			if (moveToNextElement(&el_start, &el_length) == 0) {
				if (el_length == 14 && memcmp(el_start, "methodResponse", el_length) == 0) {
					if (moveToNextElement(&el_start, &el_length) != 0) {
						xthrownew(InvalidFormatException("xmlrpc", "header"));
					}
					pos = save_pos;
					empty_el = NULL;
					if (el_length == 6 && memcmp(el_start, "params", el_length) == 0) {
						return RT_MethodResponse;
					}
					else if (el_length == 5 && memcmp(el_start, "fault", el_length) == 0) {
						return RT_FaultResponse;
					}
					return RT_MethodResponse;
				}
				pos = save_pos;
				empty_el = NULL;
				if ((el_length == 2 && memcmp(el_start, "i4", el_length) == 0) || (el_length == 2 && memcmp(el_start, "i8", el_length) == 0) || (el_length == 3 && memcmp(el_start, "int", el_length) == 0)) {
					return RT_Int;
				}
				else if (el_length == 7 && memcmp(el_start, "boolean", el_length) == 0) {
					return RT_Bool;
				}
				else if (el_length == 6 && memcmp(el_start, "double", el_length) == 0) {
					return RT_Double;
				}
				else if (el_length == 6 && memcmp(el_start, "string", el_length) == 0) {
					return RT_String;
				}
				else if (el_length == 16 && memcmp(el_start, "dateTime.iso8601", el_length) == 0) {
					return RT_Time;
				}
				else if (el_length == 6 && memcmp(el_start, "base64", el_length) == 0) {
					return RT_Binary;
				}
				else if (el_length == 6 && memcmp(el_start, "struct", el_length) == 0) {
					return RT_Struct;
				}
				else if (el_length == 5 && memcmp(el_start, "array", el_length) == 0) {
					return RT_Array;
				}
				else if (el_length == 10 && memcmp(el_start, "methodCall", el_length) == 0) {
					return RT_MethodName;
				}
				else {
					xthrownew(InvalidFormatException("xmlrpc", String(el_start, el_length)));
				}
			}
		}
		xthrownew(InvalidFormatException("xmlrpc", "value"));

	case 1:
		if ((el_length == 6 && memcmp(el_start, "params", el_length) == 0) || (el_length == 5 && memcmp(el_start, "fault", el_length) == 0)) {
			return RT_End;
		}
		xthrownew(InvalidFormatException("xmlrpc", "value"));
		break;
	}
	xthrownew(InvalidFormatException("xmlrpc", "value"));
	return RT_End;
}

void XmlRpcDecoder::readSkip()
{
	switch (readCheckType()) {
	case 1:
	case 8:
	case 7:
		readInt64();
		break;
	case 2:
		readBool();
		break;
	case 3:
		readDouble();
		break;
	case 4:
		readString();
		break;
	case 5:
		readTime();
		break;
	case 6:
		readBinary();
		break;
	case 10:
		{
			ssize_t sc = readStructLength();
			while (checkStructNext(&sc)) {
				readMemberName();
				readSkip();
			}
		}
		break;
	case 11:
		{
			ssize_t ac = readStructLength();
			while (checkArrayNext(&ac)) {
				readSkip();
			}
		}
		break;
	case 13:
		readMethodName();
		break;
	case 14:
		readMethodResponse();
		break;
	case 15:
		{
			String msg;
			readFaultResponse(&msg);
		}
		break;
	default:
		xthrownew(InvalidFormatException("rpc::type", "unknown type"));
	}
}

String XmlRpcDecoder::readMethodName()
{
	const char *cont_start;
	size_t cont_length;
	moveToNextElementReqStart("methodCall");
	moveToNextElementReqStart("methodName");
	readContent(&cont_start, &cont_length);
	moveToNextElementReqEnd("methodName");
	moveToNextElementReqStart("params");
	return convertToString(cont_start, cont_length);
}

void XmlRpcDecoder::readMethodResponse()
{
	moveToNextElementReqStart("methodResponse");
	moveToNextElementReqStart("params");
}

struct Fault_rpc
{
	Sint32				faultCode;
	String				faultString;

	static RpcHelper::StructMapper	rpc_map;
};

RpcHelper::StructMapper Fault_rpc::rpc_map(
		DR_RPCHELPER_NAMEOFFS(Fault_rpc, faultCode), dr::net::RpcHelper::getReader<Sint32>(),
		DR_RPCHELPER_NAMEOFFS(Fault_rpc, faultString), dr::net::RpcHelper::readString,
		NULL
		);

long XmlRpcDecoder::readFaultResponse(String *message)
{
	moveToNextElementReqStart("methodResponse");
	moveToNextElementReqStart("fault");
	Fault_rpc fault;
	RpcHelper::readStruct(this, &fault, &fault.rpc_map);
	if (message)
		*message = fault.faultString;
	moveToNextElementReqEnd("fault");
	moveToNextElementReqEnd("methodResponse");
	return fault.faultCode;
}

Sint32 XmlRpcDecoder::readInt32()
{
	const char *el_start;
	size_t el_length;
	el_length = moveToNextValue(&el_start);
	if ((el_length == 2 && memcmp(el_start, "i4", el_length) == 0) || (el_length == 2 && memcmp(el_start, "i8", el_length) == 0) || (el_length == 3 && memcmp(el_start, "int", el_length) == 0)) {
		const char *cont_start;
		size_t cont_length;
		readContent(&cont_start, &cont_length);
		skipValueEnd(el_start, el_length);
		Sint32 val = 0;
		while (cont_length-- > 0) {
			val = val*10+*cont_start++-'0';
		}
		return val;
	}
	else {
		xthrownew(InvalidFormatException("rpc::int::code", String::createUtf8(el_start, el_length)));
		return 0;
	}
}

Sint64 XmlRpcDecoder::readInt64()
{
	const char *el_start;
	size_t el_length;
	el_length = moveToNextValue(&el_start);
	if ((el_length == 2 && memcmp(el_start, "i4", el_length) == 0) || (el_length == 2 && memcmp(el_start, "i8", el_length) == 0) || (el_length == 3 && memcmp(el_start, "int", el_length) == 0)) {
		const char *cont_start;
		size_t cont_length;
		readContent(&cont_start, &cont_length);
		skipValueEnd(el_start, el_length);
		Sint64 val = 0;
		while (cont_length-- > 0) {
			val = val*10+*cont_start++-'0';
		}
		return val;
	}
	else {
		xthrownew(InvalidFormatException("rpc::int::code", String::createUtf8(el_start, el_length)));
		return 0;
	}
}

bool XmlRpcDecoder::readBool()
{
	const char *el_start;
	size_t el_length;
	el_length = moveToNextValue(&el_start);
	if (el_length == 7 && memcmp(el_start, "boolean", el_length) == 0) {
		const char *cont_start;
		size_t cont_length;
		readContent(&cont_start, &cont_length);
		skipValueEnd(el_start, el_length);
		if (cont_length == 4 && memcmp(cont_start, "true", cont_length) == 0)
			return true;
		else if (cont_length == 5 && memcmp(cont_start, "false", cont_length) == 0)
			return false;
		else if (cont_length == 1 && (*cont_start == '0' || *cont_start == '1'))
			return *cont_start-'0';
		xthrownew(InvalidFormatException("rpc::bool::content", String::createUtf8(cont_start, cont_length)));
		return false;
	}
	else {
		xthrownew(InvalidFormatException("rpc::bool::code", String::createUtf8(el_start, el_length)));
		return false;
	}
}

double XmlRpcDecoder::readDouble()
{
	const char *el_start;
	size_t el_length;
	el_length = moveToNextValue(&el_start);
	if (el_length == 6 && memcmp(el_start, "double", el_length) == 0) {
		const char *cont_start;
		size_t cont_length;
		readContent(&cont_start, &cont_length);
		skipValueEnd(el_start, el_length);
		double val = strtod(cont_start, NULL);
		return val;
	}
	else {
		xthrownew(InvalidFormatException("rpc::double::code", String::createUtf8(el_start, el_length)));
		return 0;
	}
}

String XmlRpcDecoder::readString()
{
	const char *el_start;
	size_t el_length;
	el_length = moveToNextValue(&el_start);
	if (el_length == 6 && memcmp(el_start, "string", el_length) == 0) {
		const char *cont_start;
		size_t cont_length;
		readContent(&cont_start, &cont_length);
		skipValueEnd(el_start, el_length);
		return String::createUtf8(cont_start, cont_length).replace("&lt;", "<").replace("&gt;", ">").replace("&quot;", "\"");
	}
	else {
		xthrownew(InvalidFormatException("rpc::string::code", String::createUtf8(el_start, el_length)));
		return Null();
	}
}

Blob XmlRpcDecoder::readBinary()
{
	const char *el_start;
	size_t el_length;
	el_length = moveToNextValue(&el_start);
	if (el_length == 6 && memcmp(el_start, "base64", el_length) == 0) {
		const char *cont_start;
		size_t cont_length;
		readContent(&cont_start, &cont_length);
		skipValueEnd(el_start, el_length);
		Blob val;
		val.unlock(MemBase64::decodeBase64(val.lock(cont_length*2/3+2), cont_start, cont_length));
		return val;
	}
	else {
		xthrownew(InvalidFormatException("rpc::base64::code", String::createUtf8(el_start, el_length)));
		return Null();
	}
}

SysTime XmlRpcDecoder::readTime()
{
	const char *el_start;
	size_t el_length;
	el_length = moveToNextValue(&el_start);
	if (el_length == 16 && memcmp(el_start, "dateTime.iso8601", el_length) == 0) {
		const char *cont_start;
		size_t cont_length;
		readContent(&cont_start, &cont_length);
		skipValueEnd(el_start, el_length);
		int year = 0, mon = 0, mday = 0, hour = 0, min = 0, sec = 0, tz = 0;
		if (cont_length < 1 || !isdigit(*cont_start))
			xthrownew(InvalidFormatException("rpc::date::format", String(cont_start, cont_length)));
		for (size_t i = 4; i > 0 && cont_length > 0 && isdigit(*cont_start); i--, cont_length--)
			year = year*10+*cont_start++-'0';
		if (cont_length > 0 && *cont_start == '-') {
			cont_length--; cont_start++;
		}
		if (cont_length < 1 || !isdigit(*cont_start))
			xthrownew(InvalidFormatException("rpc::date::format", String(cont_start, cont_length)));
		for (size_t i = 2; i > 0 && cont_length > 0 && isdigit(*cont_start); i--, cont_length--)
			mon = mon*10+*cont_start++-'0';
		if (cont_length > 0 && *cont_start == '-') {
			cont_length--; cont_start++;
		}
		if (cont_length < 1 || !isdigit(*cont_start))
			xthrownew(InvalidFormatException("rpc::date::format", String(cont_start, cont_length)));
		for (size_t i = 2; i > 0 && cont_length > 0 && isdigit(*cont_start); i--, cont_length--)
			mday = mday*10+*cont_start++-'0';
		if (cont_length > 0 && (*cont_start == 'T' || *cont_start == 't' || *cont_start == ' ')) {
			cont_length--; cont_start++;
		}
		if (cont_length >= 1) {
			if (!isdigit(*cont_start))
				xthrownew(InvalidFormatException("rpc::date::format", String(cont_start, cont_length)));
			for (size_t i = 2; i > 0 && cont_length > 0 && isdigit(*cont_start); i--, cont_length--)
				hour = hour*10+*cont_start++-'0';
			if (cont_length > 0 && *cont_start == ':') {
				cont_length--; cont_start++;
			}
			if (cont_length >= 1) {
				if (!isdigit(*cont_start))
					xthrownew(InvalidFormatException("rpc::date::format", String(cont_start, cont_length)));
				for (size_t i = 2; i > 0 && cont_length > 0 && isdigit(*cont_start); i--, cont_length--)
					min = min*10+*cont_start++-'0';
				if (cont_length > 0 && *cont_start == ':') {
					cont_length--; cont_start++;
				}
				if (cont_length >= 1) {
					if (!isdigit(*cont_start))
						xthrownew(InvalidFormatException("rpc::date::format", String(cont_start, cont_length)));
					for (size_t i = 2; i > 0 && cont_length > 0 && isdigit(*cont_start); i--, cont_length--)
						sec = sec*10+*cont_start++-'0';
					if (cont_length > 0 && *cont_start == ':') {
						cont_length--; cont_start++;
					}
				}
				tz = atol(cont_start);
				tz = tz-40*(tz/100);
			}
		}
		return Time::convertUtcTime(year, mon-1, mday-1, hour, min, sec, -tz);
	}
	else {
		xthrownew(InvalidFormatException("rpc::bool::code", String::createUtf8(el_start, el_length)));
		return 0;
	}
}

ssize_t XmlRpcDecoder::readStructLength()
{
	const char *el_start;
	size_t el_length;
	el_length = moveToNextValue(&el_start);
	if (el_length == 6 && memcmp(el_start, "struct", el_length) == 0) {
		return -1;
	}
	else {
		xthrownew(InvalidFormatException("rpc::struct::code", String::createUtf8(el_start, el_length)));
		return -1;
	}
}

ssize_t XmlRpcDecoder::readArrayLength()
{
	const char *el_start;
	size_t el_length;
	el_length = moveToNextValue(&el_start);
	if (el_length == 5 && memcmp(el_start, "array", el_length) == 0) {
		moveToNextElementReqStart("data");
		return -1;
	}
	else {
		xthrownew(InvalidFormatException("rpc::array::code", String::createUtf8(el_start, el_length)));
		return -1;
	}
}

String XmlRpcDecoder::readMemberName()
{
	const char *cont_start;
	size_t cont_length;
	moveToNextElementReqStart("member");
	moveToNextElementReqStart("name");
	readContent(&cont_start, &cont_length);
	moveToNextElementReqEnd("name");
	return String(cont_start, cont_length);
}

bool XmlRpcDecoder::checkArrayNext(ssize_t *count)
{
	const unsigned char *save_pos = pos;
	const char *el_start;
	size_t el_length;
	if (moveToNextElement(&el_start, &el_length) != 1 || el_length != 4 || memcmp(el_start, "data", el_length) != 0) {
		pos = save_pos;
		return true;
	}
	skipValueEnd("array", 5);
	return false;
}

bool XmlRpcDecoder::checkStructNext(ssize_t *count)
{
retry:
	const unsigned char *save_pos = pos;
	const char *el_start;
	size_t el_length;
	if (moveToNextElement(&el_start, &el_length) == 1) {
		if (el_length == 6 && memcmp(el_start, "member", el_length) == 0) {
			goto retry;
		}
		else if (el_length == 6 && memcmp(el_start, "struct", el_length) == 0) {
			pos = save_pos;
			skipValueEnd("struct", 6);
			return false;
		}
		else {
			xthrownew(InvalidFormatException("rpc::struct::code", String(el_start, el_length)));
			return false;
		}
	}
	else {
		if (el_length == 6 && memcmp(el_start, "member", el_length)) {
			save_pos = pos;
			if (moveToNextElement(&el_start, &el_length) != 1) {
				pos = save_pos;
				return true;
			}
		}
		if (el_length != 6 || memcmp(el_start, "struct", el_length) != 0) {
			pos = save_pos;
			return true;
		}
		xthrownew(InvalidFormatException("rpc::struct::code", String(el_start, el_length)));
		return false;
	}
}


DR_NET_NS_END
