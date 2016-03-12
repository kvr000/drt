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

#ifndef dr__io__Socket_sysiface__hxx__
# define dr__io__Socket_sysiface__hxx__

#include <dr/io/SocketStream.hxx>
#include <dr/io/SocketDatagram.hxx>

DR_IO_NS_BEGIN


/**
 * Base support for socket system interface
 */
class DR_IO_PUB Socket_sysiface_base
{
public:
	static void *			b_getOsHandle(Socket *handle);
	static void			b_setHandle(Socket *handle, void *os_handle);

	static const Blob &		b_getRemote_sysiface(SocketDatagramVirtual *handle);
};


DR_RINLINE void *Socket_sysiface_base::b_getOsHandle(Socket *handle)
{
	return handle->os_handle;
}

DR_RINLINE void Socket_sysiface_base::b_setHandle(Socket *handle, void *os_handle)
{
	handle->os_handle = os_handle;
}

DR_RINLINE const Blob &Socket_sysiface_base::b_getRemote_sysiface(SocketDatagramVirtual *handle)
{
	return handle->remote_sysiface;
}


DR_IO_NS_END

#include <dr/io/os/Socket_sysiface.os.hxx>

#endif
