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

#ifndef dr__net__XmlRpcDecoder__hxx__
# define dr__net__XmlRpcDecoder__hxx__

#include <dr/net/def_net.hxx>

#include <dr/Hash.hxx>
#include <dr/io/StreamBuffer.hxx>
#include <dr/io/NetAddress.hxx>
#include <dr/io/SocketStream.hxx>

#include <dr/net/RpcDecoder.hxx>


DR_NET_NS_BEGIN


class DR_NET_PUB XmlRpcDecoder: public RpcDecoder
{
	DR_OBJECT_DECL(XmlRpcDecoder, RpcDecoder);

public:
	/* constructor */		XmlRpcDecoder();

public:
	virtual				~XmlRpcDecoder();

public:
	virtual int			readHeader();
	virtual String			readMethodName();
	virtual void			readMethodResponse();
	virtual long			readFaultResponse(String *message);

	virtual int			readCheckType();
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
	int				moveToNextElement(const char **start, size_t *length);
	void				moveToNextElementReqStart(const BString &name);
	void				moveToNextElementReqEnd(const BString &name);
	size_t				moveToNextValue(const char **type_start);
	void				skipValueEnd(const char *type_start, size_t type_length);
	void				readContent(const char **start, size_t *length);
	String				convertToString(const char *str, size_t length);

protected:
	int				version_major;
	int				version_minor;
	size_t				length_add;
};


DR_NET_NS_END

#endif
