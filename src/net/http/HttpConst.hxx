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

#ifndef dr__net__HttpConst__hxx__
# define dr__net__HttpConst__hxx__

#include <dr/net/def_net.hxx>

#include <dr/Hash.hxx>
#include <dr/io/StreamBuffer.hxx>
#include <dr/io/NetAddress.hxx>
#include <dr/io/SocketStream.hxx>

#include <dr/Object.hxx>


DR_NET_NS_BEGIN

DR_IO_NS_USE;


/**
 * Shared namespace for HTTP constants
 */
class HttpConst
{
public:
	enum MethodType
	{
		MT_Unknown,
		MT_Head,
		MT_Get,
		MT_Post,
		MT_Put,
		MT_Delete,
		MT_Connect,
		MT_Options,
		MT_Trace,
	};

	enum TransferEncoding
	{
		TE_Basic,
		TE_Chunked,
	};

public:
	static const String		connection_string;
	static const String		head_string;
	static const String		get_string;
	static const String		post_string;
	static const String		put_string;
	static const String		delete_string;
	static const String		connect_string;
	static const String		options_string;
	static const String		trace_string;

	static const StringIndex	method_type_map;
};


DR_NET_NS_END

#endif
