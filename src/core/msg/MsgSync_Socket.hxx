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

#ifndef dr__MsgSync_Socket__hxx__
# define dr__MsgSync_Socket__hxx__

#include <dr/Object.hxx>
#include <dr/Thread.hxx>

DR_NS_BEGIN


/**
 * Socket synchronisation interface
 */
class DR_PUB MsgSync_Socket: public Iface
{
	DR_IFACE_DECL(MsgSync_Socket);
public:
	enum WaitMode
	{
		W_READ			= 1,
		W_WRITE			= 2,
		W_EXCEPT		= 4,
		W_PRECHECK		= 16,
	};

	virtual void			registerSocket(void *os_handle, WaitMode mode, const Eslot0<WaitMode> &precheck, const Eslot1<void, WaitMode> &notif) = 0;
	virtual void			unregisterSocket(void *os_handle) = 0;
};


DR_ENUM_OPS(MsgSync_Socket::WaitMode);


DR_NS_END

#endif
