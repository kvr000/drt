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

#ifndef dr__net__RpcExcept__hxx__
# define dr__net__RpcExcept__hxx__

#include <dr/Exception.hxx>

#include <dr/net/def_net.hxx>

DR_NET_NS_BEGIN


/**
 * Io Exception
 *
 * Caused by input/output failure
 */
class DR_NET_PUB RpcException: public Exception
{
	DR_THROW_DECL_SIMPLE(RpcException, Exception);

public:
	/**
	 * construct RpcException:
	 *
	 * @param handle
	 * 	handle that caused the exception
	 */
	DR_CONSTRUCT			RpcException(const String &description);

	/**
	 * construct RpcException:
	 *
	 * @param previous
	 * 	specifies exception that caused this one, the reference is lost
	 */
	DR_CONSTRUCT			RpcException(Throwable *previous, const String &description);

public:
	/**
	 * get description
	 *
	 * @return
	 * 	formatted error description
	 */
	virtual String			stringify() const;

protected:
	String				description;
};


DR_NET_NS_END

#endif
