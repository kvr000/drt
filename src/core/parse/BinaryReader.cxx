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
 * include: 	dr/Object.hxx
 * include: 	dr/Blob.hxx
 * include: 	dr/String.hxx
 * ns:		dr
 */

#include <dr/x_kw.hxx>
#include <dr/Const.hxx>

#include <dr/EndOfDataException.hxx>
#include <dr/OverflowException.hxx>

#include "_gen/BinaryReader-all.hxx"

DR_NS_BEGIN


/*drt
 * class:	BinaryReader
 * ancestor:	dr::Object
 *
 * at:	Uint8 *			cache_pos;
 * at:	Uint8 *			cache_end;
 *
 * doc:{
 *	The class is intended to convert generic stream into basic data
 * }doc
 */

DR_MET(public)
BinaryReader::BinaryReader()
{
}

DR_MET(protected virtual)
BinaryReader::~BinaryReader()
{
}

DR_MET(public virtual)
Uint8 BinaryReader::readNat8(const String &value_name)
{
	needBytes(1, value_name);
	cache_pos += 1;
	return (Uint8)(cache_pos[-1]<<0);
}

DR_MET(public virtual)
Uint16 BinaryReader::readNat16(const String &value_name)
{
	needBytes(2, value_name);
	cache_pos += 2;
	Uint16 val;
	memcpy(&val, cache_pos-2, 2);
	return val;
}

DR_MET(public virtual)
Uint32 BinaryReader::readNat32(const String &value_name)
{
	needBytes(4, value_name);
	cache_pos += 4;
	Uint32 val;
	memcpy(&val, cache_pos-4, 4);
	return val;
}

DR_MET(public virtual)
Uint64 BinaryReader::readNat64(const String &value_name)
{
	needBytes(8, value_name);
	cache_pos += 8;
	Uint64 val;
	memcpy(&val, cache_pos-8, 8);
	return val;
}

DR_MET(public virtual)
Uint8 BinaryReader::readLe8(const String &value_name)
{
	needBytes(1, value_name);
	cache_pos += 1;
	return (Uint8)(cache_pos[-1]<<0);
}

DR_MET(public virtual)
Uint16 BinaryReader::readLe16(const String &value_name)
{
	needBytes(2, value_name);
	cache_pos += 2;
	return (Uint16)(cache_pos[-2]<<0)|(Uint16)(cache_pos[-1]<<8);
}

DR_MET(public virtual)
Uint32 BinaryReader::readLe32(const String &value_name)
{
	needBytes(4, value_name);
	cache_pos += 4;
	return ((Uint32)cache_pos[-4]<<0)|((Uint32)cache_pos[-3]<<8)|((Uint32)cache_pos[-2]<<16)|((Uint32)cache_pos[-1]<<24);
}

DR_MET(public virtual)
Uint64 BinaryReader::readLe64(const String &value_name)
{
	needBytes(8, value_name);
	cache_pos += 8;
	return ((Uint64)cache_pos[-8]<<0)|((Uint64)cache_pos[-7]<<8)|((Uint64)cache_pos[-6]<<16)|((Uint64)cache_pos[-5]<<24)|((Uint64)cache_pos[-4]<<32)|((Uint64)cache_pos[-3]<<40)|((Uint64)cache_pos[-2]<<48)|((Uint64)cache_pos[-1]<<56);
}

DR_MET(public virtual)
Uint8 BinaryReader::readBe8(const String &value_name)
{
	needBytes(1, value_name);
	cache_pos += 1;
	return (Uint8)(cache_pos[-1]<<0);
}

DR_MET(public virtual)
Uint16 BinaryReader::readBe16(const String &value_name)
{
	needBytes(2, value_name);
	cache_pos += 2;
	return (Uint16)(cache_pos[-1]<<0)|(Uint16)(cache_pos[-2]<<8);
}

DR_MET(public virtual)
Uint32 BinaryReader::readBe32(const String &value_name)
{
	needBytes(4, value_name);
	cache_pos += 4;
	return ((Uint32)cache_pos[-1]<<0)|((Uint32)cache_pos[-2]<<8)|((Uint32)cache_pos[-3]<<16)|((Uint32)cache_pos[-4]<<24);
}

DR_MET(public virtual)
Uint64 BinaryReader::readBe64(const String &value_name)
{
	needBytes(8, value_name);
	cache_pos += 8;
	return ((Uint64)cache_pos[-1]<<0)|((Uint64)cache_pos[-2]<<8)|((Uint64)cache_pos[-3]<<16)|((Uint64)cache_pos[-4]<<24)|((Uint64)cache_pos[-5]<<32)|((Uint64)cache_pos[-6]<<40)|((Uint64)cache_pos[-7]<<48)|((Uint64)cache_pos[-8]<<56);
}

DR_MET(public virtual)
Uint32 BinaryReader::readVarint32(const String &value_name)
{
	Uint32 result = 0;
	for (;;) {
		needBytes(1, value_name);
		if (result >= 0x200000)
			xthrownew(OverflowException("reading varint"));
		if (*cache_pos < 0x80) {
			result = result*128+*cache_pos++;
			break;
		}
		else {
			result = result*128+(*cache_pos++&0x7f);
		}
	}
	return result;
}

DR_MET(public virtual)
Uint64 BinaryReader::readVarint64(const String &value_name)
{
	Uint64 result = 0;
	for (;;) {
		needBytes(1, value_name);
		if (result >= (Uint64)0x200000<<32)
			xthrownew(OverflowException("reading varint"));
		if (*cache_pos < 0x80) {
			result = result*128+*cache_pos++;
			break;
		}
		else {
			result = result*128+(*cache_pos++&0x7f);
		}
	}
	return result;
}

DR_MET(public virtual)
Blob BinaryReader::readBytes(size_t count, const String &value_name)
{
	needBytes(count, value_name);
	cache_pos += count;
	return Blob((char *)cache_pos-count, count);
}

DR_MET(public virtual)
void BinaryReader::readBytes(void *data, size_t count, const String &value_name)
{
	needBytes(count, value_name);
	cache_pos += count;
	memcpy(data, cache_pos-count, count);
}

DR_MET(public virtual)
Blob BinaryReader::readVarBlob(size_t max_size, const String &value_name)
{
	Uint64 size = readVarint64(value_name);
	if (size > max_size)
		xthrownew(OverflowException(String("too long blob: ")+value_name));
	return readBytes(size, value_name);
}

DR_MET(public virtual)
String BinaryReader::readVarString(size_t max_size, const String &value_name)
{
	return String::createUtf8(readVarBlob(max_size, value_name));
}

DR_MET(public abstract)
//bool BinaryReader::isEnd()

DR_MET(protected abstract)
//void BinaryReader::extendCache(size_t needbytes, const String &value_name)

DR_MET(protected inline)
//void BinaryReader::needBytes(size_t needbytes, const String &value_name)
//{
//	if ((size_t)(cache_end-cache_pos) < needbytes)
//		extendCache(needbytes, value_name);
//}


DR_NS_END
