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

#include <dr/x_kw.hxx>
#include <dr/Const.hxx>
#include <dr/Array.hxx>
#include <dr/InvalidFormatExcept.hxx>

#include <dr/net/RpcDecoder.hxx>
#include <dr/net/RpcEncoder.hxx>
#include <dr/net/RpcExcept.hxx>

#include <dr/net/RpcHelper.hxx>

DR_NET_NS_BEGIN

RpcHelper::MemberInfo::MemberInfo(const String &name_, long offs_, void (*reader_)(RpcDecoder *decoder, void *type_addr), void (*writer_)(RpcEncoder *decoder, void *type_addr)):
	name(name_),
	offset(offs_),
	reader_func(reader_),
	writer_func(writer_)
{
}

RpcHelper::StructMapper::StructMapper(const String *name0, long offs0, void (*reader0)(RpcDecoder *decoder, void *type_addr), const String *name1, ...)
{
	va_list vl;
	if (!name0)
		return;
	map[*name0] = new MemberInfo(*name0, offs0, reader0, NULL);
	va_start(vl, name1);
	for (name0 = name1; name0; name0 = va_arg(vl, String *)) {
		offs0 = va_arg(vl, long);
		reader0 = va_arg(vl, void (*)(RpcDecoder *, void *));
		map[*name0] = new MemberInfo(*name0, offs0, reader0, NULL);
	}
}

RpcHelper::StructMapper::~StructMapper()
{
}

void RpcHelper::readSkip(RpcDecoder *decoder, void *type_addr)
{
	decoder->readSkip();
}

void RpcHelper::readCharBool(RpcDecoder *decoder, void *type_addr)
{
	*((signed char *)type_addr) = decoder->readBool();
}

void RpcHelper::readInt32(RpcDecoder *decoder, void *type_addr)
{
	*((Sint32 *)type_addr) = decoder->readInt32();
}

void RpcHelper::readInt64(RpcDecoder *decoder, void *type_addr)
{
	*((Sint64 *)type_addr) = decoder->readInt64();
}

void RpcHelper::readDouble(RpcDecoder *decoder, void *type_addr)
{
	*((double *)type_addr) = decoder->readDouble();
}

void RpcHelper::readTime(RpcDecoder *decoder, void *type_addr)
{
	*((SysTime *)type_addr) = decoder->readTime();
}

void RpcHelper::readString(RpcDecoder *decoder, void *type_addr)
{
	*((String *)type_addr) = decoder->readString();
}

void RpcHelper::readBinary(RpcDecoder *decoder, void *type_addr)
{
	*((Blob *)type_addr) = decoder->readBinary();
}

void RpcHelper::readStruct(RpcDecoder *decoder, void *struct_addr, StructMapper *mapper)
{
	ssize_t cnt = decoder->readStructLength();
	while (decoder->checkStructNext(&cnt)) {
		String name = decoder->readMemberName();
		if (Hash<String, MemberInfo *>::kvpair *m = mapper->map.find(name)) {
			xtry {
				m->v->reader_func(decoder, (char *)struct_addr+m->v->offset);
			}
			xcatch (Except, ex) {
				xthrownew(RpcExcept(ex, String("failed to read ")+name));
			}
			xend;
		}
		else {
			xthrownew(InvalidFormatExcept("rpc::unknown_member", name));
		}
	}
}

void RpcHelper::readStringArray(RpcDecoder *decoder, void *array_addr)
{
	ssize_t cnt = decoder->readArrayLength();
	while (decoder->checkArrayNext(&cnt)) {
		((SArray<String> *)array_addr)->append(decoder->readString());
	}
}

void RpcHelper::readInt32Array(RpcDecoder *decoder, void *array_addr)
{
	ssize_t cnt = decoder->readArrayLength();
	while (decoder->checkArrayNext(&cnt)) {
		((SArray<Sint32> *)array_addr)->append(decoder->readInt32());
	}
}

void RpcHelper::readInt64Array(RpcDecoder *decoder, void *array_addr)
{
	ssize_t cnt = decoder->readArrayLength();
	while (decoder->checkArrayNext(&cnt)) {
		((SArray<Sint64> *)array_addr)->append(decoder->readInt64());
	}
}


DR_NET_NS_END
