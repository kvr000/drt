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

#ifndef dr__BinaryMemWriter__hxx__
# define dr__BinaryMemWriter__hxx__

#include <dr/Blob.hxx>
#include <dr/BString.hxx>

#include <dr/Object.hxx>

DR_NS_BEGIN


/**
 * Binary Parser
 * 
 * the class is intended to convert basic types and store them into memory
 */
class DR_PUB BinaryMemWriter: public Object
{
	DR_OBJECT_DECL(BinaryMemWriter, Object);

public:
	DR_CONSTRUCT			BinaryMemWriter(void *data, size_t length);
	DR_CONSTRUCT			BinaryMemWriter(Blob *data);
	//DR_CONSTRUCT			BinaryMemWriter(BString *data);

public:
	virtual				~BinaryMemWriter();

public:
	virtual ssize_t			pendingSize();
	virtual void *			currentPos();

	virtual void			writeNat8(Uint8 value, const String &value_name);
	virtual void			writeNat16(Uint16 value, const String &value_name);
	virtual void			writeNat32(Uint32 value, const String &value_name);
	virtual void			writeNat64(Uint64 value, const String &value_name);
	virtual void			writeLe8(Uint8 value, const String &value_name);
	virtual void			writeLe16(Uint16 value, const String &value_name);
	virtual void			writeLe32(Uint32 value, const String &value_name);
	virtual void			writeLe64(Uint64 value, const String &value_name);
	virtual void			writeBe8(Uint8 value, const String &value_name);
	virtual void			writeBe16(Uint16 value, const String &value_name);
	virtual void			writeBe32(Uint32 value, const String &value_name);
	virtual void			writeBe64(Uint64 value, const String &value_name);
	virtual void			writeVarint32(Uint32 value, const String &value_name);
	virtual void			writeVarint64(Uint64 value, const String &value_name);

	virtual void			writeBytes(const Blob &data, const String &value_name);
	//virtual void			writeBytes(const BString &data, const String &value_name);
	virtual void			writeBytes(const void *data, size_t size, const String &value_name);

protected:
	Uint8 *				current_pos;
	Uint8 *				end_pos;
};


DR_NS_END

#endif
