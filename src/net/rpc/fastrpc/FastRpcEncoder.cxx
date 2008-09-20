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

#include <dr/net/FastRpcEncoder.hxx>

DR_NET_NS_BEGIN


DR_OBJECT_DEF(DR_NET_NS_STR, FastRpcEncoder, RpcEncoder);
DR_OBJECT_IMPL_SIMPLE(FastRpcEncoder);

const String FastRpcEncoder::mime_type_string(Const::string("application/x-frpc"));


FastRpcEncoder::FastRpcEncoder():
	length_add(1)
{
}

FastRpcEncoder::~FastRpcEncoder()
{
}

String FastRpcEncoder::getMimeType()
{
	return mime_type_string;
}

void FastRpcEncoder::writeGenLength(Uint8 type, size_t length)
{
	char c[9];
	if (length > 0xff) {
		c[2] = (Uint8)(length>>8);
		if ((Uint32)length > 0xffff) {
			c[3] = (Uint8)(length>>16);
			c[4] = (Uint8)(length>>24);
			if (sizeof(length) > 4 && (Uint64)length&((Uint64)1<<32)) {
				c[5] = (Uint8)((Uint64)length>>32);
				c[6] = (Uint8)((Uint64)length>>40);
				c[7] = (Uint8)((Uint64)length>>48);
				c[8] = (Uint8)((Uint64)length>>56);
				type |= 7;
			}
			else {
				type |= 3;
			}
		}
		else {
			type |= 1;
		}
	}
	else {
		type |= 0;
	}
	c[1] = (Uint8)(length>>0);
	if ((type&7) == 7 && length_add == 0)
		type--;
	c[0] = type+(1-length_add);
	content.append(c, (type&7)+2);
}

void FastRpcEncoder::writeHeader()
{
	if (length_add > 0)
		content.append("\xCA\x11\x02\x00", 4);
	else
		content.append("\xCA\x11\x01\x00", 4);
}

void FastRpcEncoder::writeMethodName(const String &name)
{
	char ar[2] = { '\x68', name.utf8().getSize() };
	content.append(ar, 2).append(name.utf8());
}

void FastRpcEncoder::writeMethodEnd()
{
}

void FastRpcEncoder::writeMethodResponse()
{
	content.appendChar('\x70');
}

void FastRpcEncoder::writeResponseEnd()
{
}

void FastRpcEncoder::writeFaultResponse(long code, const String &message)
{
	content.appendChar('\x78');
	writeInt((Sint64)code);
	writeString(message);
}

void FastRpcEncoder::writeInt(Sint32 value)
{
	Uint8 type;
	char c[5];
	if (length_add > 0) {
		if (value < 0) {
			type = 0x40;
			value = -value;
		}
		else {
			type = 0x38;
		}
		if (value <= 0xffff) {
			if (value <= 0xff) {
				type |= 0;
			}
			else {
				type |= 1;
			}
		}
		else {
			type |= 3;
		}
	}
	else {
		if (value < 0) {
#if 0
			if (value >= -128)
				type = 0x08;
			else if (value >= -32768)
				type = 0x09;
			else
				type = 0x0b;
#else
			type = 0x0b;
#endif
		}
		else {
			if (value < 128)
				type = 0x08;
			else if (value < 32768)
				type = 0x09;
			else
				type = 0x0b;
		}
	}
	c[0] = type+(1-length_add);
	Mem::copyLe(c+1, &value, 4);
	content.append(c, (type&7)+2);
}

void FastRpcEncoder::writeInt(Sint64 value)
{
	Uint8 type;
	char c[9];
	if (length_add > 0) {
		if (value < 0) {
			type = 0x40;
			value = -value;
		}
		else {
			type = 0x38;
		}
		if (value <= 0xffff) {
			if (value <= 0xff) {
				type |= 0;
			}
			else {
				type |= 1;
			}
		}
		else if (value <= 0xffffffff) {
			type |= 3;
		}
		else {
			type |= 7;
		}
	}
	else {
		if (value < 0) {
#if 0
			if (value >= -128)
				type = 0x09;
			else if (value >= -(Sint64)1<<32)
				type = 0x0c;
			else
				type = 0x0f;
#else
			type = 0x0f;
#endif
		}
		else {
			if (value < 128)
				type = 0x08;
			else if (value < (Sint64)1<<16)
				type = 0x09;
			else if (value < (Sint64)1<<32)
				type = 0x0b;
			else
				type = 0x0f;
		}
		if ((type&7) == 7 && length_add == 0)
			type--;
	}
	c[0] = type+(1-length_add);
	Mem::copyLe(c+1, &value, 8);
	content.append(c, (type&7)+2);
}

void FastRpcEncoder::writeBool(bool value)
{
	content.appendChar(0x10|(char)value);
}

void FastRpcEncoder::writeDouble(double value)
{
	char c[9];

	Mem::copyLe(c+1, &value, 8);
	c[0] = 0x18;
	content.append(c, 9);
}

void FastRpcEncoder::writeString(const String &str)
{
	BString us(str.utf8());
	writeGenLength(0x20, us.getSize());
	content.append(us);
}

void FastRpcEncoder::writeBinary(const Blob &data)
{
	writeGenLength(0x30, data.getSize());
	content.append(data);
}

void FastRpcEncoder::writeTime(SysTime tvalue)
{
	int year, mon, day, hour, min, sec, wday = 7;
	char c[11];
	c[0] = 0x28;
	memset(c+1, 0, 10);
	Time::timeToUtcCalendar(tvalue, &year, &mon, &day, &hour, &min, &sec);
	tvalue = Time::toSeconds(tvalue);
	c[1] = 0;
	c[2] = (Uint8)(tvalue>>0);
	c[3] = (Uint8)(tvalue>>8);
	c[4] = (Uint8)(tvalue>>16);
	c[5] = (Uint8)(tvalue>>24);
	c[6] = ((wday))+((sec&0x1f)<<3);
	c[7] = ((sec&0x20)>>5)+((min&0x3f)<<1)+((hour&0x01)<<7);
	c[8] = ((hour&0x1e)>>1)+(((day+1)&0x0f)<<4);
	c[9] = (((day+1)&0x1f)>>4)+(((mon+1)&0x0f)<<1)+(((year-1600)&0x07)<<5);
	c[10] = (((year-1600)&0x7f8)>>3);
	content.append(c, 11);
}

void FastRpcEncoder::writeArrayLength(size_t length)
{
	writeGenLength(0x58, length);
}

void FastRpcEncoder::writeStructLength(size_t length)
{
	writeGenLength(0x50, length);
}

void FastRpcEncoder::writeMemberName(const String &name)
{
	BString us(name.utf8());
	if (us.getSize() > 255)
		xthrownew(InvalidFormatException("member name too long", "string"));
	char c = us.getSize();
	content.append(&c, 1);
	content.append(us);
}

void FastRpcEncoder::writeArrayEnd()
{
}

void FastRpcEncoder::writeStructEnd()
{
}


DR_NET_NS_END
