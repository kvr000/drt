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
 * include: 	dr/BinaryReader.hxx
 * ns:		dr
 */

#include <dr/x_kw.hxx>
#include <dr/Const.hxx>

#include <dr/EndOfDataException.hxx>
#include <dr/OverflowException.hxx>

#include "_gen/BinaryMemReader-all.hxx"

DR_NS_BEGIN


/*drt
 * class:	BinaryMemReader
 * ancestor:	dr::BinaryReader
 *
 * at:	const Uint8 *			current_pos;
 * at:	const Uint8 *			end_pos;
 *
 * doc:{
 *	The class is intended to convert memory stream into basic data
 * }doc
 */

DR_MET(public)
BinaryMemReader::BinaryMemReader(const Blob *data)
{
	cache_pos = (Uint8 *)data->toStr();
	cache_end = cache_pos+data->getSize();
}

DR_MET(public)
BinaryMemReader::BinaryMemReader(const void *data, size_t data_size)
{
	cache_pos = (Uint8 *)data;
	cache_end = cache_pos+data_size;
}

DR_MET(public virtual)
BinaryMemReader::~BinaryMemReader()
{
}

DR_MET(public virtual)
bool BinaryMemReader::isEnd()
{
	return cache_pos == cache_end;
}

DR_MET(protected virtual)
void BinaryMemReader::extendCache(size_t needbytes, const String &value_name)
{
	if ((size_t)(cache_end-cache_pos) < needbytes)
		xthrownew(EndOfDataException("byte", value_name));
}


DR_NS_END
