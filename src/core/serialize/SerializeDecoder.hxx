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

#ifndef dr__SerializeDecoder__hxx__
# define dr__SerializeDecoder__hxx__

#include <dr/String.hxx>
#include <dr/Blob.hxx>
#include <dr/Time.hxx>
#include <dr/ScalarPtr.hxx>

#include <dr/Iface.hxx>

DR_NS_BEGIN


/**
 * Serialize Decoder
 *
 * This class provides abstract to load data dumps from various formats
 */
class DR_PUB SerializeDecoder: public Iface
{
	DR_IFACE_DECL(SerializeDecoder);

public:
	virtual void			readSkip() = 0;
	virtual void			readValue(const ScalarPtr &value) = 0;
	virtual bool			readBool() = 0;
	virtual Sint32			readInt32() = 0;
	virtual Sint64			readInt64() = 0;
	virtual double			readDouble() = 0;
	virtual String			readString() = 0;
	virtual Blob			readBinary() = 0;
	virtual SysTime			readTime() = 0;
	virtual ssize_t			readStructLength() = 0;
	virtual ssize_t			readArrayLength() = 0;
	virtual String			readMemberName() = 0;
	virtual bool			checkArrayNext(ssize_t *count) = 0;
	virtual bool			checkStructNext(ssize_t *count) = 0;
	virtual String			readClassName() = 0;
};


DR_NS_END

#endif
