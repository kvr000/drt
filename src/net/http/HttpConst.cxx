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

#include <dr/MethodConv.hxx>
#include <dr/EndOfDataException.hxx>

#include <dr/io/NetAddressInet4.hxx>

#include <dr/net/HttpConst.hxx>

DR_NET_NS_BEGIN


const String HttpConst::connection_string(Const::string("connection"));
const String HttpConst::head_string(Const::string("head"));
const String HttpConst::get_string(Const::string("get"));
const String HttpConst::post_string(Const::string("post"));
const String HttpConst::put_string(Const::string("put"));
const String HttpConst::delete_string(Const::string("delete"));
const String HttpConst::connect_string(Const::string("connect"));
const String HttpConst::options_string(Const::string("options"));
const String HttpConst::trace_string(Const::string("trace"));

const StringIndex HttpConst::method_type_map(
		&head_string, MT_Head,
		&get_string, MT_Get,
		&post_string, MT_Post,
		&put_string, MT_Put,
		&delete_string, MT_Delete,
		&connect_string, MT_Connect,
		&options_string, MT_Options,
		&trace_string, MT_Trace,
		NULL
		);


DR_NET_NS_END
