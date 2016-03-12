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

#include <dr/io/SocketServer.hxx>
#include <dr/io/SocketStream.hxx>

#include <dr/io/dev/Socket_sysiface.hxx>

DR_IO_NS_BEGIN


DR_OBJECT_DEF(DR_IO_NS_STR, SocketServer, Socket);
DR_OBJECT_IMPL_SIMPLE(SocketServer);

DR_EXPORT_MET SocketServer::SocketServer(const String &domain, const String &proto):
	Socket()
{
	os_handle = Socket_sysiface::createServer(NULL, domain, proto);
}

DR_EXPORT_MET SocketServer::~SocketServer()
{
	Socket_sysiface::closeServer(this);
	Socket_sysiface::setHandleInvalid(this);
}

void SocketServer::listen(int max)
{
	Socket_sysiface::listen(this, max);
}

SocketStream *SocketServer::accept()
{
	return Socket_sysiface::accept(this);
}


DR_IO_NS_END
