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

#include <dr/EndOfDataException.hxx>
#include <dr/OverflowException.hxx>

#include <dr/BinaryMemReader.hxx>

DR_NS_BEGIN


DR_OBJECT_IMPL_SIMPLE(BinaryMemReader);
DR_OBJECT_DEF(DR_NS_STR, BinaryMemReader, Object);


BinaryMemReader::BinaryMemReader(const Blob *data):
	current_pos((Uint8 *)data->toStr()),
	end_pos((Uint8 *)data->toStr()+data->getSize())
{
}

#if 0
BinaryMemReader::BinaryMemReader(const BString *data):
	current_pos((Uint8 *)data->toStr()),
	end_pos((Uint8 *)data->toStr()+data->getSize())
{
}
#endif

BinaryMemReader::BinaryMemReader(const void *data, size_t data_size):
	current_pos((Uint8 *)data),
	end_pos((Uint8 *)data+data_size)
{
}

BinaryMemReader::~BinaryMemReader()
{
}

ssize_t BinaryMemReader::pendingSize()
{
	return end_pos-current_pos;
}

Uint8 BinaryMemReader::readNat8(const String &value_name)
{
	if (end_pos-current_pos < 1)
		xthrownew(EndOfDataException("Uint8", value_name));
	current_pos += 1;
	return (Uint8)(current_pos[-1]<<0);
}

Uint16 BinaryMemReader::readNat16(const String &value_name)
{
	if (end_pos-current_pos < 2)
		xthrownew(EndOfDataException("Uint16", value_name));
	current_pos += 2;
	Uint16 val;
	memcpy(&val, current_pos-2, 2);
	return val;
}

Uint32 BinaryMemReader::readNat32(const String &value_name)
{
	if (end_pos-current_pos < 4)
		xthrownew(EndOfDataException("Uint32", value_name));
	current_pos += 4;
	Uint32 val;
	memcpy(&val, current_pos-4, 4);
	return val;
}

Uint64 BinaryMemReader::readNat64(const String &value_name)
{
	if (end_pos-current_pos < 8)
		xthrownew(EndOfDataException("Uint64", value_name));
	current_pos += 8;
	Uint64 val;
	memcpy(&val, current_pos-8, 8);
	return val;
}

Uint8 BinaryMemReader::readLe8(const String &value_name)
{
	if (end_pos-current_pos < 1)
		xthrownew(EndOfDataException("Uint8", value_name));
	current_pos += 1;
	return (Uint8)(current_pos[-1]<<0);
}

Uint16 BinaryMemReader::readLe16(const String &value_name)
{
	if (end_pos-current_pos < 2)
		xthrownew(EndOfDataException("Uint16", value_name));
	current_pos += 2;
	return (Uint16)(current_pos[-2]<<0)|(Uint16)(current_pos[-1]<<8);
}

Uint32 BinaryMemReader::readLe32(const String &value_name)
{
	if (end_pos-current_pos < 4)
		xthrownew(EndOfDataException("Uint32", value_name));
	current_pos += 4;
	return ((Uint32)current_pos[-4]<<0)|((Uint32)current_pos[-3]<<8)|((Uint32)current_pos[-2]<<16)|((Uint32)current_pos[-1]<<24);
}

Uint64 BinaryMemReader::readLe64(const String &value_name)
{
	if (end_pos-current_pos < 8)
		xthrownew(EndOfDataException("Uint64", value_name));
	current_pos += 8;
	return ((Uint64)current_pos[-8]<<0)|((Uint64)current_pos[-7]<<8)|((Uint64)current_pos[-6]<<16)|((Uint64)current_pos[-5]<<24)|((Uint64)current_pos[-4]<<32)|((Uint64)current_pos[-3]<<40)|((Uint64)current_pos[-2]<<48)|((Uint64)current_pos[-1]<<56);
}

Uint8 BinaryMemReader::readBe8(const String &value_name)
{
	if (end_pos-current_pos < 1)
		xthrownew(EndOfDataException("Uint8", value_name));
	current_pos += 1;
	return (Uint8)(current_pos[-1]<<0);
}

Uint16 BinaryMemReader::readBe16(const String &value_name)
{
	if (end_pos-current_pos < 2)
		xthrownew(EndOfDataException("Uint16", value_name));
	current_pos += 2;
	return (Uint16)(current_pos[-1]<<0)|(Uint16)(current_pos[-2]<<8);
}

Uint32 BinaryMemReader::readBe32(const String &value_name)
{
	if (end_pos-current_pos < 4)
		xthrownew(EndOfDataException("Uint32", value_name));
	current_pos += 4;
	return ((Uint32)current_pos[-1]<<0)|((Uint32)current_pos[-2]<<8)|((Uint32)current_pos[-3]<<16)|((Uint32)current_pos[-4]<<24);
}

Uint64 BinaryMemReader::readBe64(const String &value_name)
{
	if (end_pos-current_pos < 8)
		xthrownew(EndOfDataException("Uint64", value_name));
	current_pos += 8;
	return ((Uint64)current_pos[-1]<<0)|((Uint64)current_pos[-2]<<8)|((Uint64)current_pos[-3]<<16)|((Uint64)current_pos[-4]<<24)|((Uint64)current_pos[-5]<<32)|((Uint64)current_pos[-6]<<40)|((Uint64)current_pos[-7]<<48)|((Uint64)current_pos[-8]<<56);
}

Uint32 BinaryMemReader::readVarint32(const String &value_name)
{
	Uint32 result = 0;
	for (;;) {
		if (end_pos-current_pos < 1)
			xthrownew(EndOfDataException("Varint", value_name));
		if (result >= 0x200000)
			xthrownew(OverflowException("reading varint"));
		if (*current_pos < 0x80) {
			result = result*128+*current_pos++;
			break;
		}
		else {
			result = result*128+(*current_pos++&0x7f);
		}
	}
	return result;
}

Uint64 BinaryMemReader::readVarint64(const String &value_name)
{
	Uint64 result = 0;
	for (;;) {
		if (end_pos-current_pos < 1)
			xthrownew(EndOfDataException("Varint", value_name));
		if (result >= (Uint64)0x200000<<32)
			xthrownew(OverflowException("reading varint"));
		if (*current_pos < 0x80) {
			result = result*128+*current_pos++;
			break;
		}
		else {
			result = result*128+(*current_pos++&0x7f);
		}
	}
	return result;
}

Blob BinaryMemReader::readBytes(size_t count, const String &value_name)
{
	if (end_pos-current_pos < (ssize_t)count)
		xthrownew(EndOfDataException("Uint64", value_name));
	current_pos += count;
	return Blob((char *)current_pos-count, count);
}

void BinaryMemReader::readBytes(void *data, size_t count, const String &value_name)
{
	if (end_pos-current_pos < (ssize_t)count)
		xthrownew(EndOfDataException("Uint64", value_name));
	current_pos += count;
	memcpy(data, current_pos-count, count);
}


DR_NS_END
