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

#include "_gen/BinaryWriter-all.hxx"

DR_NS_BEGIN


/*drt
 * class:	BinaryWriter
 * ancestor:	dr::Object
 *
 * doc:{
 *	The class is intended to convert basic types into byte stream
 * }doc
 */

DR_MET(public)
BinaryWriter::BinaryWriter()
{
}

DR_MET(protected virtual)
BinaryWriter::~BinaryWriter()
{
}

DR_MET(public virtual)
void BinaryWriter::writeNat8(Uint8 value, const String &value_name)
{
	writeDirect(&value, 1, value_name);
}

DR_MET(public virtual)
void BinaryWriter::writeNat16(Uint16 value, const String &value_name)
{
	writeDirect(&value, 2, value_name);
}

DR_MET(public virtual)
void BinaryWriter::writeNat32(Uint32 value, const String &value_name)
{
	writeDirect(&value, 4, value_name);
}

DR_MET(public virtual)
void BinaryWriter::writeNat64(Uint64 value, const String &value_name)
{
	writeDirect(&value, 8, value_name);
}

DR_MET(public virtual)
void BinaryWriter::writeLe8(Uint8 value, const String &value_name)
{
	writeDirect(&value, 1, value_name);
}

DR_MET(public virtual)
void BinaryWriter::writeLe16(Uint16 value, const String &value_name)
{
	Uint8 bin[2] = { value, value>>8 };
	writeDirect(bin, 2, value_name);
}

DR_MET(public virtual)
void BinaryWriter::writeLe32(Uint32 value, const String &value_name)
{
	Uint8 bin[4] = { value, value>>8, value>>16, value>>24 };
	writeDirect(bin, 4, value_name);
}

DR_MET(public virtual)
void BinaryWriter::writeLe64(Uint64 value, const String &value_name)
{
	Uint8 bin[8] = { value, value>>8, value>>16, value>>24, value>>32, value>>40, value>>48, value>>56 };
	writeDirect(bin, 8, value_name);
}

DR_MET(public virtual)
void BinaryWriter::writeBe8(Uint8 value, const String &value_name)
{
	writeDirect(&value, 1, value_name);
}

DR_MET(public virtual)
void BinaryWriter::writeBe16(Uint16 value, const String &value_name)
{
	Uint8 bin[2] = { value>>8, value };
	writeDirect(bin, 2, value_name);
}

DR_MET(public virtual)
void BinaryWriter::writeBe32(Uint32 value, const String &value_name)
{
	Uint8 bin[4] = { value>>24, value>>16, value>>8, value };
	writeDirect(bin, 4, value_name);
}

DR_MET(public virtual)
void BinaryWriter::writeBe64(Uint64 value, const String &value_name)
{
	Uint8 bin[8] = { value>>56, value>>48, value>>40, value>>32, value>>24, value>>16, value>>8, value };
	writeDirect(bin, 8, value_name);
}

DR_MET(public virtual)
void BinaryWriter::writeVarint(Uint32 value, const String &value_name)
{
	Uint8 b[5];
	ssize_t p = sizeof(b);
	b[--p] = value&0x7f;
	while ((value /= 128) != 0) {
		b[--p] = value|0x80;
	}
	writeDirect(b+p, sizeof(b)-p, value_name);
}

DR_MET(public virtual)
void BinaryWriter::writeVarint(Uint64 value, const String &value_name)
{
	Uint8 b[9];
	ssize_t p = sizeof(b);
	b[--p] = value&0x7f;
	while ((value /= 128) != 0) {
		b[--p] = value|0x80;
	}
	writeDirect(b+p, sizeof(b)-p, value_name);
}

DR_MET(public virtual)
void BinaryWriter::writeBytes(const void *data, size_t count, const String &value_name)
{
	writeDirect(data, count, value_name);
}

DR_MET(public virtual)
void BinaryWriter::writeBytes(const Blob &data, const String &value_name)
{
	writeDirect(data.toStr(), data.getSize(), value_name);
}

DR_MET(public virtual)
void BinaryWriter::writeVarBlob(const Blob &data, const String &value_name)
{
	writeVarint(data.getSize(), value_name);
	writeBytes(data, value_name);
}

DR_MET(public virtual)
void BinaryWriter::writeVarString(const String &data, const String &value_name)
{
	writeVarBlob(data.utf8(), value_name);
}

DR_MET(public abstract)
//void BinaryWriter::writeDirect(const void *data, size_t size, const String &value_name)


DR_NS_END
