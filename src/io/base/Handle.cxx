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

#include <dr/io/Handle.hxx>

DR_IO_NS_BEGIN


DR_OBJECT_DEF(DR_IO_NS_STR, Handle, Object);
DR_OBJECT_IMPL_SIMPLE(Handle);

const String Handle::close_string(Const::string("close"));
const String Handle::read_string(Const::string("read"));
const String Handle::write_string(Const::string("write"));
const String Handle::seek_string(Const::string("seek"));
const String Handle::truncate_string(Const::string("truncate"));
const String Handle::wait_string(Const::string("wait"));
const String Handle::lock_string(Const::string("lock"));

DR_EXPORT_MET Blob Handle::read(size_t size)
{
	Blob out;
	void *plain = out.lock(size);
	size = read(plain, size);
	out.unlock(size);
	return out;
}

DR_EXPORT_MET ssize_t Handle::read(Blob *buf, size_t size, size_t offs)
{
	Blob out;
	void *plain = buf->lockKeep(offs+size)+offs;
	size = read(plain, size);
	buf->unlock(offs+size);
	return size;
}

DR_EXPORT_MET ssize_t Handle::write(const Blob &bytes)
{
	return write(bytes.toStr(), bytes.getSize());
}

DR_EXPORT_MET ssize_t Handle::read(void *buf, size_t size)
{
	DR_AssertInvalid();
	return 0;
}

DR_EXPORT_MET ssize_t Handle::write(const void *buf, size_t size)
{
	DR_AssertInvalid();
	return 0;
}

DR_EXPORT_MET FileOffset Handle::seek(FileOffset offset, int how)
{
	DR_AssertInvalid();
	return 0;
}

DR_EXPORT_MET void Handle::setBlocking(bool blocking)
{
	DR_AssertInvalid();
}

DR_EXPORT_MET Handle::Mode Handle::waitData(Mode wait_mode, Sint64 nanosec)
{
	return wait_mode;
}


DR_IO_NS_END
