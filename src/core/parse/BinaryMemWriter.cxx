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
 * include: 	dr/BinaryWriter.hxx
 * ns:		dr
 */

#include <dr/x_kw.hxx>
#include <dr/Const.hxx>

#include <dr/EndOfDataException.hxx>

#include "_gen/BinaryMemWriter-all.hxx"

DR_NS_BEGIN


/*drt
 * class:	BinaryMemWriter
 * ancestor:	dr::BinaryWriter
 *
 * at:	Uint8 *				current_pos;
 * at:	Uint8 *				end_pos;
 *
 * doc:{
 *	The class is intended to convert basic types into memory stream
 * }doc
 */

DR_MET(public)
BinaryMemWriter::BinaryMemWriter(Blob *data):
	current_pos((Uint8 *)data->toStr()),
	end_pos((Uint8 *)data->toStr()+data->getSize())
{
}

DR_MET(public)
BinaryMemWriter::BinaryMemWriter(void *data, size_t data_size):
	current_pos((Uint8 *)data),
	end_pos((Uint8 *)data+data_size)
{
}

DR_MET(public virtual)
BinaryMemWriter::~BinaryMemWriter()
{
}

DR_MET(public virtual)
ssize_t BinaryMemWriter::pendingSize()
{
	return end_pos-current_pos;
}

DR_MET(public virtual)
void *BinaryMemWriter::currentPos()
{
	return current_pos;
}

DR_MET(protected virtual)
void BinaryMemWriter::writeDirect(const void *data, size_t size, const String &value_name)
{
	if ((size_t)(end_pos-current_pos) < size)
		xthrownew(EndOfDataException("byte", value_name));
	memcpy(current_pos, data, size);
	current_pos += size;
}


DR_NS_END
