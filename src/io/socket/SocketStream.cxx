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

#include <dr/Const.hxx>

#include <dr/io/SocketStream.hxx>

#include <dr/io/dev/Socket_sysiface.hxx>

DR_IO_NS_BEGIN


DR_OBJECT_DEF(DR_IO_NS_STR, SocketStream, Socket);
DR_OBJECT_IMPL_SIMPLE(SocketStream);

DR_EXPORT_DTS const String SocketServer::accept_string(Const::string("accept"));
DR_EXPORT_DTS const String SocketServer::listen_string(Const::string("listen"));

DR_EXPORT_MET SocketStream::SocketStream(const String &domain, const String &proto):
	Socket()
{
	os_handle = Socket_sysiface::createStreamed(NULL, domain, proto);
}

DR_EXPORT_MET SocketStream::SocketStream(NetAddress *remote_, void *os_handle_):
	Socket(os_handle_, M_READ|M_WRITE),
	remote(remote_, true)
{
}

DR_EXPORT_MET void SocketStream::connect(NetAddress *remote_)
{
	Socket_sysiface::connect(this, remote_);
	remote.updateRef_ts(remote_);
}

DR_EXPORT_MET NetAddress *SocketStream::getRemote()
{
	if (remote)
		return remote.getDoref();
	remote.updateUnref_ts(Socket_sysiface::getPeerName(this));
	return remote.getDoref();
}

DR_EXPORT_MET void SocketStream::shutdown(int direction)
{
	Socket_sysiface::shutdown(this, direction);
}


DR_IO_NS_END
