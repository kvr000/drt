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

#include <dr/EndOfDataException.hxx>

#include "_gen/BinaryBlobWriter-all.hxx"

DR_NS_BEGIN


/*drt
 * class:	BinaryBlobWriter
 * ancestor:	dr::BinaryWriter
 *
 * at:	Blob				content;
 *
 * doc:{
 *	The class is intended to convert basic types into memory stream
 * }doc
 */

DR_MET(public)
BinaryBlobWriter::BinaryBlobWriter()
{
}

DR_MET(public virtual)
BinaryBlobWriter::~BinaryBlobWriter()
{
}

DR_MET(public virtual)
Blob BinaryBlobWriter::getContent()
{
	return content;
}

DR_MET(public virtual)
void BinaryBlobWriter::nullify()
{
	content.setNull();
}

DR_MET(protected virtual)
void BinaryBlobWriter::writeDirect(const void *data, size_t size, const String &value_name)
{
	content.append((const char *)data, size);
}


DR_NS_END
