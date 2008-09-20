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

#ifndef dr__net__FastRpcDecoder__hxx__
# define dr__net__FastRpcDecoder__hxx__

#include <dr/net/def_net.hxx>

#include <dr/Hash.hxx>
#include <dr/io/StreamBuffer.hxx>
#include <dr/io/NetAddress.hxx>
#include <dr/io/SocketStream.hxx>

#include <dr/net/RpcDecoder.hxx>


DR_NET_NS_BEGIN


class DR_NET_PUB FastRpcDecoder: public RpcDecoder
{
	DR_OBJECT_DECL(FastRpcDecoder, RpcDecoder);

public:
	/* constructor */		FastRpcDecoder();

public:
	virtual				~FastRpcDecoder();

public:
	virtual int			readHeader();
	virtual String			readMethodName();
	virtual void			readMethodResponse();
	virtual long			readFaultResponse(String *message);

	virtual RpcType			readCheckType();
	virtual void			readSkip();
	virtual bool			readBool();
	virtual Sint32			readInt32();
	virtual Sint64			readInt64();
	virtual double			readDouble();
	virtual String			readString();
	virtual Blob			readBinary();
	virtual SysTime			readTime();
	virtual ssize_t			readStructLength();
	virtual ssize_t			readArrayLength();
	virtual String			readMemberName();
	virtual bool			checkArrayNext(ssize_t *count);
	virtual bool			checkStructNext(ssize_t *count);

protected:
	size_t				readGenLength(size_t length_octets);

protected:
	int				version_major;
	int				version_minor;
	size_t				length_add;
};


DR_NET_NS_END

#endif
