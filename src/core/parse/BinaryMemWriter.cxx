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

#include <dr/EndOfDataExcept.hxx>

#include <dr/BinaryMemWriter.hxx>

DR_NS_BEGIN


DR_OBJECT_IMPL_SIMPLE(BinaryMemWriter);
DR_OBJECT_DEF(DR_NS_STR, BinaryMemWriter, Object);


BinaryMemWriter::BinaryMemWriter(Blob *data):
	current_pos((Uint8 *)data->toStr()),
	end_pos((Uint8 *)data->toStr()+data->getSize())
{
}

#if 0
BinaryMemWriter::BinaryMemWriter(BString *data):
	current_pos((Uint8 *)data->toStr()),
	end_pos((Uint8 *)data->toStr()+data->getSize())
{
}
#endif

BinaryMemWriter::BinaryMemWriter(void *data, size_t data_size):
	current_pos((Uint8 *)data),
	end_pos((Uint8 *)data+data_size)
{
}

BinaryMemWriter::~BinaryMemWriter()
{
}

ssize_t BinaryMemWriter::pendingSize()
{
	return end_pos-current_pos;
}

void *BinaryMemWriter::currentPos()
{
	return current_pos;
}

void BinaryMemWriter::writeNat8(Uint8 value, const String &value_name)
{
	if (end_pos-current_pos < 1)
		xthrownew(EndOfDataExcept("Uint8", value_name));
	current_pos += 1;
	current_pos[-1] = value;
}

void BinaryMemWriter::writeNat16(Uint16 value, const String &value_name)
{
	if (end_pos-current_pos < 2)
		xthrownew(EndOfDataExcept("Uint16", value_name));
	current_pos += 2;
	memcpy(current_pos-2, &value, 2);
}

void BinaryMemWriter::writeNat32(Uint32 value, const String &value_name)
{
	if (end_pos-current_pos < 4)
		xthrownew(EndOfDataExcept("Uint32", value_name));
	current_pos += 4;
	memcpy(current_pos-4, &value, 4);
}

void BinaryMemWriter::writeNat64(Uint64 value, const String &value_name)
{
	if (end_pos-current_pos < 8)
		xthrownew(EndOfDataExcept("Uint64", value_name));
	current_pos += 8;
	memcpy(current_pos-8, &value, 8);
}

void BinaryMemWriter::writeLe8(Uint8 value, const String &value_name)
{
	if (end_pos-current_pos < 1)
		xthrownew(EndOfDataExcept("Uint8", value_name));
	current_pos += 1;
	current_pos[-1] = value;
}

void BinaryMemWriter::writeLe16(Uint16 value, const String &value_name)
{
	if (end_pos-current_pos < 2)
		xthrownew(EndOfDataExcept("Uint16", value_name));
	current_pos += 2;
	current_pos[-2] = value>>0;
	current_pos[-1] = value>>8;
}

void BinaryMemWriter::writeLe32(Uint32 value, const String &value_name)
{
	if (end_pos-current_pos < 4)
		xthrownew(EndOfDataExcept("Uint32", value_name));
	current_pos += 4;
	current_pos[-4] = value>>0;
	current_pos[-3] = value>>8;
	current_pos[-2] = value>>16;
	current_pos[-1] = value>>24;
}

void BinaryMemWriter::writeLe64(Uint64 value, const String &value_name)
{
	if (end_pos-current_pos < 8)
		xthrownew(EndOfDataExcept("Uint64", value_name));
	current_pos += 8;
	current_pos[-8] = (Uint8)(value>>0);
	current_pos[-7] = (Uint8)(value>>8);
	current_pos[-6] = (Uint8)(value>>16);
	current_pos[-5] = (Uint8)(value>>24);
	current_pos[-4] = (Uint8)(value>>32);
	current_pos[-3] = (Uint8)(value>>40);
	current_pos[-2] = (Uint8)(value>>48);
	current_pos[-1] = (Uint8)(value>>56);
}

void BinaryMemWriter::writeBe8(Uint8 value, const String &value_name)
{
	if (end_pos-current_pos < 1)
		xthrownew(EndOfDataExcept("Uint8", value_name));
	current_pos += 1;
	current_pos[-1] = value;
}

void BinaryMemWriter::writeBe16(Uint16 value, const String &value_name)
{
	if (end_pos-current_pos < 2)
		xthrownew(EndOfDataExcept("Uint16", value_name));
	current_pos += 2;
	current_pos[-1] = value>>0;
	current_pos[-2] = value>>8;
}

void BinaryMemWriter::writeBe32(Uint32 value, const String &value_name)
{
	if (end_pos-current_pos < 4)
		xthrownew(EndOfDataExcept("Uint32", value_name));
	current_pos += 4;
	current_pos[-1] = value>>0;
	current_pos[-2] = value>>8;
	current_pos[-3] = value>>16;
	current_pos[-4] = value>>24;
}

void BinaryMemWriter::writeBe64(Uint64 value, const String &value_name)
{
	if (end_pos-current_pos < 8)
		xthrownew(EndOfDataExcept("Uint64", value_name));
	current_pos += 8;
	current_pos[-1] = (Uint8)(value>>0);
	current_pos[-2] = (Uint8)(value>>8);
	current_pos[-3] = (Uint8)(value>>16);
	current_pos[-4] = (Uint8)(value>>24);
	current_pos[-5] = (Uint8)(value>>32);
	current_pos[-6] = (Uint8)(value>>40);
	current_pos[-7] = (Uint8)(value>>48);
	current_pos[-8] = (Uint8)(value>>56);
}

void BinaryMemWriter::writeVarint32(Uint32 value, const String &value_name)
{
	Uint8 b[5];
	ssize_t p = 0;
	b[0] = value&0x7f;
	while ((value /= 128) != 0) {
		b[++p] = value|0x80;
	}
	if (end_pos-current_pos <= (ssize_t)p)
		xthrownew(EndOfDataExcept("Varint", value_name));
	while (p >= 0)
		*current_pos++ = b[p--];
}

void BinaryMemWriter::writeVarint64(Uint64 value, const String &value_name)
{
	Uint8 b[9];
	ssize_t p = 0;
	b[0] = (Uint8)(value&0x7f);
	while ((value /= 128) != 0) {
		b[++p] = (Uint8)value|0x80;
	}
	if (end_pos-current_pos <= (ssize_t)p)
		xthrownew(EndOfDataExcept("Varint", value_name));
	while (p >= 0)
		*current_pos++ = b[p--];
}

void BinaryMemWriter::writeBytes(const void *data, size_t count, const String &value_name)
{
	if (end_pos-current_pos < (ssize_t)count)
		xthrownew(EndOfDataExcept("Array", value_name));
	current_pos += count;
	memcpy(current_pos-count, data, count);
}

void BinaryMemWriter::writeBytes(const Blob &data, const String &value_name)
{
	writeBytes(data.toStr(), data.getSize(), value_name);
}

#if 0
void BinaryMemWriter::writeBytes(const BString &data, const String &value_name)
{
	writeBytes(data.toStr(), data.getSize(), value_name);
}
#endif


DR_NS_END
