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

#ifndef dr__BinaryMemReader__hxx__
# define dr__BinaryMemReader__hxx__

#include <dr/Blob.hxx>
#include <dr/BString.hxx>

#include <dr/Object.hxx>

DR_NS_BEGIN


/**
 * Binary Parser
 * 
 * the class is intended to convert memory into basic types
 */
class DR_PUB BinaryMemReader: public Object
{
	DR_OBJECT_DECL(BinaryMemReader, Object);

public:
	DR_CONSTRUCT			BinaryMemReader(const void *data, size_t length);
	DR_CONSTRUCT			BinaryMemReader(const Blob *data);
	//DR_CONSTRUCT			BinaryMemReader(const BString *data);

public:
	virtual				~BinaryMemReader();

public:
	virtual ssize_t			pendingSize();

	virtual Uint8			readNat8(const String &value_name);
	virtual Uint16			readNat16(const String &value_name);
	virtual Uint32			readNat32(const String &value_name);
	virtual Uint64			readNat64(const String &value_name);
	virtual Uint8			readLe8(const String &value_name);
	virtual Uint16			readLe16(const String &value_name);
	virtual Uint32			readLe32(const String &value_name);
	virtual Uint64			readLe64(const String &value_name);
	virtual Uint8			readBe8(const String &value_name);
	virtual Uint16			readBe16(const String &value_name);
	virtual Uint32			readBe32(const String &value_name);
	virtual Uint64			readBe64(const String &value_name);
	virtual Uint32			readVarint32(const String &value_name);
	virtual Uint64			readVarint64(const String &value_name);

	virtual Blob			readBytes(size_t size, const String &value_name);
	virtual void			readBytes(void *data, size_t size, const String &value_name);

protected:
	const Uint8 *			current_pos;
	const Uint8 *			end_pos;
};


DR_NS_END

#endif
