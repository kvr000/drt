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

#include <dr/EndOfDataException.hxx>

#include <dr/io/NetAddressInet4.hxx>

#include <dr/net/XmlRpcDecoder.hxx>

#include <dr/net/RpcDecoder.hxx>

DR_NET_NS_BEGIN


DR_OBJECT_DEF(DR_NET_NS_STR, RpcDecoder, Object);
DR_OBJECT_IMPL_IFACE1(RpcDecoder, SerializeDecoder);


RpcDecoder::RpcDecoder()
{
}

RpcDecoder::~RpcDecoder()
{
}

String RpcDecoder::readClassName()
{
	return Null();
}

void RpcDecoder::readValue(const ScalarPtr &value)
{
	switch (value.getType()) {
	case ScalarPtr::T_None:
		readSkip();
		break;

	case ScalarPtr::T_Bool:
		*(bool *)value.getPtr() = readBool();
		break;

	case ScalarPtr::T_Sint8:
		*(Sint8 *)value.getPtr() = readInt32();
		break;

	case ScalarPtr::T_Sint16:
		*(Sint16 *)value.getPtr() = readInt32();
		break;

	case ScalarPtr::T_Sint32:
		*(Sint32 *)value.getPtr() = readInt32();
		break;

	case ScalarPtr::T_Sint64:
		*(Sint64 *)value.getPtr() = readInt64();
		break;

	case ScalarPtr::T_Uint8:
		*(Uint8 *)value.getPtr() = readInt32();
		break;

	case ScalarPtr::T_Uint16:
		*(Uint16 *)value.getPtr() = readInt32();
		break;

	case ScalarPtr::T_Uint32:
		*(Uint32 *)value.getPtr() = readInt32();
		break;

	case ScalarPtr::T_Uint64:
		*(Uint64 *)value.getPtr() = readInt64();
		break;

	case ScalarPtr::T_Float:
		*(float *)value.getPtr() = readDouble();
		break;

	case ScalarPtr::T_Double:
		*(float *)value.getPtr() = readDouble();
		break;

	case ScalarPtr::T_LongDouble:
		*(float *)value.getPtr() = readDouble();
		break;

	case ScalarPtr::T_String:
		*(String *)value.getPtr() = readString();
		break;

	case ScalarPtr::T_Blob:
		*(Blob *)value.getPtr() = readBinary();
		break;

	case ScalarPtr::T_Object:
		((Object *)value.getPtr())->unserializeFrom(this);
		break;
	}
}

void RpcDecoder::setContent(const Blob &content_)
{
	content = content_;
	pos = (const unsigned char *)content.toStr();
	end = pos+content.getSize();
}

RpcDecoder *RpcDecoder::createDecoder(const Blob &data)
{
	Ref<RpcDecoder> decoder;
	decoder.setNoref(new XmlRpcDecoder());
	decoder->setContent(data);
	return decoder.getAndNull();
}


DR_NET_NS_END
