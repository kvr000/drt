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

/*drt
 * include:	dr/Exception.hxx
 * include:	dr/net/def_net.hxx
 *
 * ns:		dr::net
 */

#include <dr/x_kw.hxx>

#include <dr/net/HttpException.hxx>
#include "_gen/HttpException-all.hxx"

DR_NET_NS_BEGIN


/*drt
 * class:	HttpException
 * type:	exception
 * ancestor:	dr::Exception
 *
 * at:	int				code;			///< 0 - start, 1 - keep-alive, 2 - close
 * at:	String				message;
 */

DR_MET(public)
HttpException::HttpException(int code_, const String &message_):
	code(code_),
	message(message_)
{
}

DR_MET(protected virtual)
HttpException::~HttpException()
{
}

DR_MET(public virtual)
String HttpException::stringify() const
{
	String desc;
	desc.append("http communication error occurred: ").appendNumber(code).append(" ").append(message);
	return desc;
}


DR_NET_NS_END
