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

#ifndef dr__net__FastRpcEncoder__hxx__
# define dr__net__FastRpcEncoder__hxx__

#include <dr/net/def_net.hxx>

#include <dr/Hash.hxx>
#include <dr/io/StreamBuffer.hxx>
#include <dr/io/NetAddress.hxx>
#include <dr/io/SocketStream.hxx>

#include <dr/net/RpcEncoder.hxx>


DR_NET_NS_BEGIN

DR_IO_NS_USE;


class DR_NET_PUB FastRpcEncoder: public RpcEncoder
{
	DR_OBJECT_DECL(FastRpcEncoder, RpcEncoder);

public:
	/* constructor */		FastRpcEncoder();

	virtual				~FastRpcEncoder();

public:
	virtual String			getMimeType();

public:
	virtual void			writeHeader();
	virtual void			writeMethodName(const String &name);
	virtual void			writeMethodEnd();
	virtual void			writeMethodResponse();
	virtual void			writeResponseEnd();
	virtual void			writeFaultResponse(long code, const String &message);

	virtual void			writeBool(bool value);
	virtual void			writeInt(Sint32 value);
	virtual void			writeInt(Sint64 value);
	virtual void			writeDouble(double value);
	virtual void			writeString(const String &value);
	virtual void			writeBinary(const Blob &value);
	virtual void			writeTime(SysTime time);
	virtual void			writeStructLength(size_t count);
	virtual void			writeArrayLength(size_t count);
	virtual void			writeMemberName(const String &name);
	virtual void			writeArrayEnd();
	virtual void			writeStructEnd();

protected:
	void				writeGenLength(Uint8 type, size_t length);

protected:
	size_t				length_add;				/**< fastrpc2 indicator */

public:
	static const String		mime_type_string;
};


DR_NET_NS_END

#endif
