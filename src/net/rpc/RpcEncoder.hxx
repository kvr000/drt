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

#ifndef dr__net__RpcEncoder__hxx__
# define dr__net__RpcEncoder__hxx__

#include <dr/net/def_net.hxx>

#include <dr/Hash.hxx>
#include <dr/io/StreamBuffer.hxx>
#include <dr/io/NetAddress.hxx>
#include <dr/io/SocketStream.hxx>

#include <dr/Object.hxx>


DR_NET_NS_BEGIN

DR_IO_NS_USE;


class DR_NET_PUB RpcEncoder: public Object, public SerializeEncoder
{
	DR_OBJECT_DECL(RpcEncoder, Object);
	DR_REDIR_BEHAV();

public:
	/* constructor */		RpcEncoder();

protected:
	virtual				~RpcEncoder();

public:
	virtual String			getMimeType() = 0;

public:
	virtual Blob			getContent();

public:
	virtual void			writeHeader() = 0;
	virtual void			writeMethodName(const String &name) = 0;
	virtual void			writeMethodEnd() = 0;
	virtual void			writeMethodResponse() = 0;
	virtual void			writeResponseEnd() = 0;
	virtual void			writeFaultResponse(long code, const String &message) = 0;

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
	virtual void			writeClassName(const String &name);
	virtual void			writeValue(const ScalarPtr &value);

protected:
	Blob				content;
};


DR_NET_NS_END

#endif
