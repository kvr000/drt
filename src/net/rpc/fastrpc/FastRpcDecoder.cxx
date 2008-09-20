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

#include <dr/io/NetAddressInet4.hxx>

#include <dr/net/FastRpcDecoder.hxx>

DR_NET_NS_BEGIN


DR_OBJECT_DEF(DR_NET_NS_STR, FastRpcDecoder, RpcDecoder);
DR_OBJECT_IMPL_SIMPLE(FastRpcDecoder);


FastRpcDecoder::FastRpcDecoder()
{
}

FastRpcDecoder::~FastRpcDecoder()
{
}

size_t FastRpcDecoder::readGenLength(size_t length_octets)
{
	Uint64 length = 0;
	length_octets += length_add;
	if (pos > end+length_octets)
		xthrownew(EndOfDataException("char", "bool"));
	Mem::copyPartialLe(&length, sizeof(length), pos, length_octets); pos += length_octets;
	if (length != (size_t)length)
		xthrownew(OverflowException("size_t"));
	return (size_t)length;
}

int FastRpcDecoder::readHeader()
{
	if (pos > end+5)
		xthrownew(EndOfDataException("char", "bool"));
	if (memcmp(pos, "\xCA\x11", 2) != 0)
		xthrownew(InvalidFormatException("rpc::header", "unexpected header"));
	version_major = pos[2];
	version_minor = pos[3];
	length_add = (version_major >= 2);
	pos += 4;
	switch (*pos>>3) {
	case 0xd:
		return 0;
	case 0xe:
		return 1;
	case 0xf:
		return 2;
	default:
		xthrownew(InvalidFormatException("rpc::header", "unexpected first member"));
	}
	return -1;
}

RpcDecoder::RpcType FastRpcDecoder::readCheckType()
{
	if (pos >= end)
		return RT_End;
	switch (*pos>>3) {
	case 1:
	case 8:
	case 7:
		return RT_Int;
	case 2:
		return RT_Bool;
	case 3:
		return RT_Double;
	case 4:
		return RT_String;
	case 5:
		return RT_Time;
	case 6:
		return RT_Binary;
	case 10:
		return RT_Struct;
	case 11:
		return RT_Array;
	case 13:
		return RT_MethodName;
	case 14:
		return RT_MethodResponse;
	case 15:
		return RT_FaultResponse;
	}
	xthrownew(InvalidFormatException("rpc::type", "unknown type"));
	return RT_End;
}

void FastRpcDecoder::readSkip()
{
	switch (*pos>>3) {
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
			size_t sc = readStructLength();
			while (sc--) {
				readMemberName();
				readSkip();
			}
		}
		break;
	case 11:
		{
			size_t ac = readArrayLength();
			while (ac--) {
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

String FastRpcDecoder::readMethodName()
{
	if (pos > end+1)
		xthrownew(EndOfDataException("char", "string"));

	Uint8 type = *pos++;
	if (type != 0x68)
		xthrownew(InvalidFormatException("rpc::method::code", String::createNumber(type)));
	Uint8 len = *pos++;
	if (pos > end+len)
		xthrownew(EndOfDataException("char", "member"));
	pos += len;
	return String((const char *)pos-len, len);
}

void FastRpcDecoder::readMethodResponse()
{
	if (pos > end+1)
		xthrownew(EndOfDataException("char", "string"));

	Uint8 type = *pos++;
	if (type != 0x70)
		xthrownew(InvalidFormatException("rpc::methodresponse::code", String::createNumber(type)));
}

long FastRpcDecoder::readFaultResponse(String *message)
{
	if (pos > end+1)
		xthrownew(EndOfDataException("char", "string"));

	Uint8 type = *pos++;
	if (type != 0x78)
		xthrownew(InvalidFormatException("rpc::faultresponse::code", String::createNumber(type)));
	long result = readInt64();
	*message = readString();
	return result;
}

Sint32 FastRpcDecoder::readInt32()
{
	Sint32 value = 0;
	Uint8 type;
	if (pos >= end)
		xthrownew(EndOfDataException("char", "int size"));
	type = *pos++;
	switch (type>>3) {
	case 1:
		{
			size_t octets = (type&7);
			if (octets > 4)
				xthrownew(OverflowException("rpc::int32"));
			if (pos+octets > end)
				xthrownew(EndOfDataException("char", "int32"));
			octets *= 8;
			for (size_t i = 0; i < octets; i += 8)
				value += (Uint32)*pos++<<i;
			value |= value<<(sizeof(value)*8-octets)>>(sizeof(value)*8-octets);
		}
		break;

	case 7:
		{
			size_t octets = (type&7)+length_add;
			if (octets > 4)
				xthrownew(OverflowException("rpc::int32"));
			if (pos+octets > end)
				xthrownew(EndOfDataException("char", "int32"));
			octets *= 8;
			for (size_t i = 0; i < octets; i += 8)
				value += (Uint32)*pos++<<i;
		}
		break;

	case 8:
		{
			size_t octets = (type&7)+length_add;
			if (octets > 4)
				xthrownew(OverflowException("rpc::int32"));
			if (pos+octets > end)
				xthrownew(EndOfDataException("char", "int32"));
			octets *= 8;
			for (size_t i = 0; i < octets; i += 8)
				value += (Uint32)*pos++<<i;
			value = -value;
		}
		break;

	default:
		xthrownew(InvalidFormatException("rpc::int::code", String::createNumber(type)));
		break;
	}

	return value;
}

Sint64 FastRpcDecoder::readInt64()
{
	Sint64 value = 0;
	Uint8 type;
	if (pos >= end)
		xthrownew(EndOfDataException("char", "int size"));
	type = *pos++;
	switch (type>>3) {
	case 1:
		{
			size_t octets = (type&7);
			if (octets > 8)
				xthrownew(OverflowException("rpc::int64"));
			if (pos+octets > end)
				xthrownew(EndOfDataException("char", "int64"));
			octets *= 8;
			for (size_t i = 0; i < octets; i += 8)
				value += (Uint64)*pos++<<i;
			value |= value<<(sizeof(value)*8-octets)>>(sizeof(value)*8-octets);
		}
		break;

	case 7:
		{
			size_t octets = (type&7)+length_add;
			if (pos+octets > end)
				xthrownew(EndOfDataException("char", "int64"));
			octets *= 8;
			for (size_t i = 0; i < octets; i += 8)
				value += (Uint64)*pos++<<i;
		}
		break;

	case 8:
		{
			size_t octets = (type&7)+length_add;
			if (pos+octets > end)
				xthrownew(EndOfDataException("char", "int64"));
			octets *= 8;
			for (size_t i = 0; i < octets; i += 8)
				value += (Uint64)*pos++<<i;
			value = -value;
		}
		break;

	default:
		xthrownew(InvalidFormatException("rpc::int::code", String::createNumber(type)));
		break;
	}

	return value;
}

bool FastRpcDecoder::readBool()
{
	Uint8 type;

	if (pos >= end)
		xthrownew(EndOfDataException("char", "bool"));
	type = *pos++;
	if ((type&~1) != 0x10)
		xthrownew(InvalidFormatException("rpc::bool::code", String::createNumber(type)));
	return type&1;
}

double FastRpcDecoder::readDouble()
{
	double value;

	if (pos > end+9)
		xthrownew(EndOfDataException("char", "bool"));

	Uint8 type = *pos++;
	if (type != 0x18)
		xthrownew(InvalidFormatException("rpc::double::code", String::createNumber(type)));
	Mem::copyLe(&value, pos, 8); pos += 8;
	return value;
}

String FastRpcDecoder::readString()
{
	String value;

	if (pos >= end)
		xthrownew(EndOfDataException("char", "string"));

	Uint8 type = *pos++;
	if ((type&0xf8) != 0x20)
		xthrownew(InvalidFormatException("rpc::string::code", String::createNumber(type)));
	size_t str_len = readGenLength(type&0x7);
	if (pos > end+str_len)
		xthrownew(EndOfDataException("char", "string"));
	value.setUtf8((const char *)pos, str_len); pos += str_len;
	return value;
}

Blob FastRpcDecoder::readBinary()
{
	if (pos >= end)
		xthrownew(EndOfDataException("char", "string"));

	Uint8 type = *pos++;
	if ((type&0xf8) != 0x30)
		xthrownew(InvalidFormatException("rpc::string::code", String::createNumber(type)));
	size_t str_len = readGenLength(type&0x7);
	if (pos > end+str_len)
		xthrownew(EndOfDataException("char", "string"));
	pos += str_len;
	return Blob((const char *)pos-str_len, str_len);
}

SysTime FastRpcDecoder::readTime()
{
	SysTime value = 0;
	if (pos >= end)
		xthrownew(EndOfDataException("char", "time"));

	Uint8 type = *pos++;
	if (type != 0x28)
		xthrownew(InvalidFormatException("rpc::time::code", String::createNumber(type)));
	if (pos > end+10)
		xthrownew(EndOfDataException("char", "string"));
	Mem::copyPartialLe(&value, 8, pos+1, 4);
	if (value == 0) {
		value = Time::convertUtcTime(1600+((pos[9]<<3)|(pos[8]>>5)), ((pos[8]&0x1e)>>1)-1, (((pos[8]&1)<<4)|(pos[7]>>4))-1,
				((pos[7]&0xf)<<1)|(pos[6]>>7), (pos[6]&0x7e)>>1, ((pos[6]&0x01)<<5)|((pos[5]&0xf8)>>3), pos[0]*15);
	}
	else {
		value = Time::fromSeconds(value);
	}
	pos += 10;
	return value;
}

ssize_t FastRpcDecoder::readStructLength()
{
	if (pos >= end)
		xthrownew(EndOfDataException("char", "string"));

	Uint8 type = *pos++;
	if ((type&0xf8) != 0x50)
		xthrownew(InvalidFormatException("rpc::struct::code", String::createNumber(type)));
	return readGenLength(type&0x7);
}

ssize_t FastRpcDecoder::readArrayLength()
{
	if (pos >= end)
		xthrownew(EndOfDataException("char", "string"));

	Uint8 type = *pos++;
	if ((type&0xf8) != 0x58)
		xthrownew(InvalidFormatException("rpc::array::code", String::createNumber(type)));
	return readGenLength(type&0x7);
}

String FastRpcDecoder::readMemberName()
{
	if (pos >= end)
		xthrownew(EndOfDataException("char", "string"));

	Uint8 len = *pos++;
	if (pos > end+len)
		xthrownew(EndOfDataException("char", "member"));
	pos += len;
	return String((const char *)pos-len, len);
}

bool FastRpcDecoder::checkArrayNext(ssize_t *count)
{
	return --*count >= 0;
}

bool FastRpcDecoder::checkStructNext(ssize_t *count)
{
	return --*count >= 0;
}


DR_NET_NS_END
