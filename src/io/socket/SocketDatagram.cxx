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

#include <dr/io/NetAddress.hxx>
#include <dr/io/Socket.hxx>
#include <dr/io/dev/Socket_sysiface.hxx>

DR_IO_NS_BEGIN


DR_OBJECT_DEF(DR_IO_NS_STR, SocketDatagram, Socket);
DR_OBJECT_IMPL_SIMPLE(SocketDatagram);

DR_EXPORT_MET SocketDatagram::SocketDatagram(const String &domain, const String &proto):
	Socket()
{
	os_handle = Socket_sysiface::createDatagram(NULL, domain, proto);
}

DR_EXPORT_MET SocketDatagram::SocketDatagram(void *os_handle_, Mode mode_):
	Socket(os_handle_, mode_)
{
}

NetAddress *SocketDatagram::getRemote()
{
	return Socket_sysiface::getPeerName(this);
}

DR_EXPORT_MET SocketDatagram *SocketDatagram::openVirtual(NetAddress *remote_)
{
	return new SocketDatagramVirtual(this, remote_);
}

DR_EXPORT_MET ssize_t SocketDatagram::recvFrom(NetAddress **sender, void *buf, size_t buflen)
{
	Blob sender_sysiface;
	return Socket_sysiface::readfrom(this, sender, &sender_sysiface, buf, buflen);
}

DR_EXPORT_MET Blob SocketDatagram::recvFrom(NetAddress **sender, size_t buflen)
{
	Blob sender_sysiface;
	Blob out;
	out.unlock(Socket_sysiface::readfrom(this, sender, &sender_sysiface, out.lock(buflen), buflen));
	return out;
}

DR_EXPORT_MET ssize_t SocketDatagram::recvVirtual(SocketDatagram **conn, void *buf, size_t buflen)
{
	Ref<NetAddress> sender;
	Blob sender_sysiface;
	buflen = Socket_sysiface::readfrom(this, sender.mem(), &sender_sysiface, buf, buflen);
	DR_REF_XCHG(conn, (SocketDatagram *)new SocketDatagramVirtual(this, sender, sender_sysiface));
	return buflen;
}

DR_EXPORT_MET Blob SocketDatagram::recvVirtual(SocketDatagram **conn, size_t buflen)
{
	Ref<NetAddress> sender;
	Blob sender_sysiface;
	Blob out;
	out.unlock(Socket_sysiface::readfrom(this, sender.mem(), &sender_sysiface, out.lock(buflen), buflen));
	DR_REF_XCHG(conn, (SocketDatagram *)new SocketDatagramVirtual(this, sender, sender_sysiface));
	return out;
}


DR_OBJECT_DEF(DR_IO_NS_STR, SocketDatagramVirtual, SocketDatagram);
DR_OBJECT_IMPL_SIMPLE(SocketDatagramVirtual);

DR_EXPORT_MET SocketDatagramVirtual::SocketDatagramVirtual(SocketDatagram *owner_, NetAddress *remote_):
	SocketDatagram(owner_->os_handle, M_NOCLOSE),
	remote_sysiface(Socket_sysiface::getNetAddress(remote_)),
	remote(remote_, true),
	owner(owner_, true)
{
}

DR_EXPORT_MET SocketDatagramVirtual::SocketDatagramVirtual(SocketDatagram *owner_, NetAddress *remote_, const Blob &remote_sysiface_):
	SocketDatagram(owner_->os_handle, M_NOCLOSE),
	remote_sysiface(remote_sysiface_),
	remote(remote_, true),
	owner(owner_, true)
{
}

DR_EXPORT_MET ssize_t SocketDatagramVirtual::read(void *buf, size_t buflen)
{
	DR_AssertInvalid();
	return -1;
}

DR_EXPORT_MET ssize_t SocketDatagramVirtual::write(const void *buf, size_t buflen)
{
	return Socket_sysiface::writeto(this, buf, buflen);
}

NetAddress *SocketDatagramVirtual::getRemote()
{
	return remote.getDoref();
}


DR_IO_NS_END
