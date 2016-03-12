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
#include <dr/io/File.hxx>

#include <dr/io/os/File_sysiface.os.hxx>

DR_IO_NS_BEGIN


DR_OBJECT_DEF(DR_IO_NS_STR, File, Handle);
DR_OBJECT_IMPL_SIMPLE(File);

DR_EXPORT_DTS const String File::open_string(Const::string("open"));
DR_EXPORT_DTS const String File::unlink_string(Const::string("unlink"));
DR_EXPORT_DTS const String File::rename_string(Const::string("rename"));
DR_EXPORT_DTS const String File::stat_string(Const::string("stat"));


File::File(const String &filename_, Mode mode_)
	: Handle(mode_),
	filename(filename_),
	os_handle(File_sysiface::getOsHandleInvalid())
{
	os_handle = File_sysiface::open(filename, mode);
}

File::File(Mode mode_):
	Handle(mode_),
	os_handle(File_sysiface::getOsHandleInvalid())
{
}

File *File::createFromOsHandle(void *os_handle_, Mode mode_)
{
	File *this_ = new File(mode_);
	this_->os_handle = os_handle_;
	return this_;
}

DR_EXPORT_MET File::~File()
{
}

void File::close()
{
	if (!File_sysiface::isHandleInvalid(this)) {
		File_sysiface::close(this);
		File_sysiface::setHandleInvalid(this);
	}
}

DR_EXPORT_MET ssize_t File::read(void *buf, size_t buflen)
{
	return File_sysiface::read(this, buf, buflen);
}

DR_EXPORT_MET ssize_t File::write(const void *buf, size_t buflen)
{
	return File_sysiface::write(this, buf, buflen);
}

DR_EXPORT_MET FileOffset File::seek(FileOffset offset, int how)
{
	return File_sysiface::seek(this, offset, how);
}

DR_EXPORT_MET void File::truncate(FileOffset offset)
{
	File_sysiface::truncate(this, offset);
}

DR_EXPORT_MET void File::lockFull(int type)
{
	File_sysiface::lockFull(this, type);
}

DR_EXPORT_MET void File::unlockFull(int type)
{
	File_sysiface::unlockFull(this, type);
}

DR_EXPORT_MET void File::updateTime(Sint64 time)
{
	File_sysiface::updateTime(this, time);
}

DR_EXPORT_MET File::HandleType File::getType(const String &filename_)
{
	return File_sysiface::getType(filename_);
}

DR_EXPORT_MET void File::unlink(const String &filename_)
{
	File_sysiface::unlink(filename_);
}

DR_EXPORT_MET void File::rename(const String &filename_, const String &tofilename_)
{
	File_sysiface::rename(filename_, tofilename_);
}

DR_EXPORT_MET Blob File::readFile(const String &filename)
{
	ERef<File> fd(new File(filename, M_READ));
	Blob out;
	Blob data;
	while (!(data = fd->read(4096)).isEmpty()) {
		out.append(data);
	}
	return out;
}

DR_EXPORT_MET void File::writeFile(const String &filename, const Blob &content)
{
	ERef<File> fd(new File(filename, M_WRITE|M_CREATE|M_TRUNC));
	for (size_t p = 0; p < content.getSize(); ) {
		p += fd->write(content.toStr()+p, content.getSize()-p);
	}
	fd->flush();
}


DR_IO_NS_END
