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

#ifndef dr__net__RpcDecoder__hxx__
# define dr__net__RpcDecoder__hxx__

#include <dr/net/def_net.hxx>

#include <dr/Hash.hxx>
#include <dr/io/StreamBuffer.hxx>
#include <dr/io/NetAddress.hxx>
#include <dr/io/SocketStream.hxx>

#include <dr/Object.hxx>


DR_NET_NS_BEGIN

DR_IO_NS_USE;


class RpcDecoder: public Object
{
	DR_OBJECT_DECL(RpcDecoder, Object);

public:
	/* constructor */		RpcDecoder();

protected:
	virtual				~RpcDecoder();

public:
	virtual void			setContent(const Blob &content);

public:
	/**
	 * reads the message header
	 *
	 * @return 0
	 * 	if the data is method call
	 * @return 1
	 * 	if the data contain response
	 * @return 2
	 * 	if the data contain fault
	 */
	virtual int			readHeader() = 0;
	virtual String			readMethodName() = 0;
	virtual void			readMethodResponse() = 0;
	virtual long			readFaultResponse(String *message) = 0;

	/**
	 * reads type of next element
	 *
	 * @return -1
	 * 	if at the end
	 * @return type id
	 * 	otherwise
	 */
	virtual int			readCheckType() = 0;

	/**
	 * skips the next type (even complex)
	 *
	 * @note in case the parser is at then end, exception is thrown
	 */
	virtual void			readSkip() = 0;
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

public:
	static RpcDecoder *		createDecoder(const Blob &data);

protected:
	Blob				content;
	const unsigned char *		pos;
	const unsigned char *		end;
};


DR_NET_NS_END

#endif
