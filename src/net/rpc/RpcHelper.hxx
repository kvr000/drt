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

#ifndef dr__net__RpcHelper__hxx__
# define dr__net__RpcHelper__hxx__

#include <dr/net/def_net.hxx>

#include <dr/Hash.hxx>
#include <dr/io/StreamBuffer.hxx>
#include <dr/io/NetAddress.hxx>
#include <dr/io/SocketStream.hxx>

#include <dr/Object.hxx>


DR_NET_NS_BEGIN

DR_IO_NS_USE;

class RpcEncoder;
class RpcDecoder;

#define DR_RPCHELPER_NAMEOFFS(struct, member) &Const::string(#member), ((char *)&((struct *)64)->member-(char *)64)


class DR_NET_PUB RpcHelper
{
public:
	struct MemberInfo: public Base
	{
		DR_CONSTRUCT			MemberInfo(const String &name, long offset, void (*reader_func)(RpcDecoder *decoder, void *type_addr), void (*writer_func)(RpcEncoder *encoder, void *type_addr));

		String				name;
		long				offset;
		void				(*reader_func)(RpcDecoder *decoder, void *type_addr);
		void				(*writer_func)(RpcEncoder *encoder, void *type_addr);
	};

	class StructMapper: public Base
	{
	public:
		DR_CONSTRUCT			StructMapper(const String *name0, long offs0, void (*reader0)(RpcDecoder *decoder, void *type_addr), const String *name1, ...);
		DR_DESTRUCT			~StructMapper();

		Hash<String, MemberInfo *>	map;
	};

	static void			readSkip(RpcDecoder *decoder, void *type_addr);
	static void			writeSkip(RpcDecoder *decoder, void *type_addr);
	static void			readCharBool(RpcDecoder *decoder, void *type_addr);
	static void			writeCharBool(RpcDecoder *decoder, void *type_addr);
	static void			readString(RpcDecoder *decoder, void *type_addr);
	static void			writeString(RpcDecoder *decoder, void *type_addr);
	static void			readInt32(RpcDecoder *decoder, void *type_addr);
	static void			writeInt32(RpcDecoder *decoder, void *type_addr);
	static void			readInt64(RpcDecoder *decoder, void *type_addr);
	static void			writeInt64(RpcDecoder *decoder, void *type_addr);
	static void			readDouble(RpcDecoder *decoder, void *type_addr);
	static void			writeDouble(RpcDecoder *decoder, void *type_addr);
	static void			readTime(RpcDecoder *decoder, void *type_addr);
	static void			writeTime(RpcDecoder *decoder, void *type_addr);
	static void			readBinary(RpcDecoder *decoder, void *type_addr);
	static void			writeBinary(RpcDecoder *decoder, void *type_addr);

	static void			readStringArray(RpcDecoder *decoder, void *type_addr);
	static void			writeStringArray(RpcDecoder *decoder, void *type_addr);
	static void			readInt32Array(RpcDecoder *decoder, void *type_addr);
	static void			writeInt32Array(RpcDecoder *decoder, void *type_addr);
	static void			readInt64Array(RpcDecoder *decoder, void *type_addr);
	static void			writeInt64Array(RpcDecoder *decoder, void *type_addr);

	template <typename T>
	DR_RINLINE static void		(*getReader())(RpcDecoder *, void *)	{ return T::RpcHelper__undefined_function(); }
	static void			readStruct(RpcDecoder *decoder, void *struct_addr, StructMapper *map);
};

template <>
DR_RINLINE
void (*RpcHelper::getReader<Sint32>())(RpcDecoder *, void *) { return &readInt32; }
template <>
DR_RINLINE
void (*RpcHelper::getReader<Sint64>())(RpcDecoder *, void *) { return &readInt64; }
template <>
DR_RINLINE
void (*RpcHelper::getReader<String>())(RpcDecoder *, void *) { return &readString; }
template <>
DR_RINLINE
void (*RpcHelper::getReader<Blob>())(RpcDecoder *, void *) { return &readBinary; }


DR_NET_NS_END

#endif
