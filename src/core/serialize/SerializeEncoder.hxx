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

#ifndef dr__SerializeEncoder__hxx__
# define dr__SerializeEncoder__hxx__

#include <dr/String.hxx>
#include <dr/Blob.hxx>
#include <dr/Time.hxx>
#include <dr/ScalarPtr.hxx>

#include <dr/Iface.hxx>

DR_NS_BEGIN


/**
 * Serialize Encoder
 *
 * This interface provides abstract for writing object dumps to various formats
 */
class DR_PUB SerializeEncoder: public Iface
{
	DR_IFACE_DECL(SerializeEncoder);

public:
	virtual void			writeValue(const ScalarPtr &value) = 0;
	virtual void			writeBool(bool value) = 0;
	virtual void			writeInt(Sint32 value) = 0;
	virtual void			writeInt(Sint64 value) = 0;
	virtual void			writeDouble(double value) = 0;
	virtual void			writeString(const String &value) = 0;
	virtual void			writeBinary(const Blob &value) = 0;
	virtual void			writeTime(SysTime time) = 0;
	virtual void			writeStructLength(size_t count) = 0;
	virtual void			writeArrayLength(size_t count) = 0;
	virtual void			writeMemberName(const String &name) = 0;
	virtual void			writeArrayEnd() = 0;
	virtual void			writeStructEnd() = 0;
	virtual void			writeClassName(const String &name) = 0;
};


DR_NS_END

#endif
