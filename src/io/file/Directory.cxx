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
#include <dr/io/Directory.hxx>

#include <dr/io/os/File_sysiface.os.hxx>

DR_IO_NS_BEGIN


DR_OBJECT_DEF(DR_IO_NS_STR, Directory, Handle);
DR_OBJECT_IMPL_SIMPLE(Directory);

DR_EXPORT_DTS const String Directory::rmdir_string(Const::string("rmdir"));
DR_EXPORT_DTS const String Directory::mkdir_string(Const::string("mkdir"));


Directory::Directory(const String &dirname_):
	dirname(dirname_),
	os_handle(File_sysiface::getDirHandleInvalid())
{
	os_handle = File_sysiface::openDir(this, dirname);
}

DR_EXPORT_MET Directory::~Directory()
{
	File_sysiface::closeDir(this);
}

DR_EXPORT_MET bool Directory::getNext(String *entry)
{
	return File_sysiface::nextDirEntry(this, entry);
}

DR_EXPORT_MET void Directory::rmdir(const String &dirname_)
{
	File_sysiface::rmdir(dirname_);
}

DR_EXPORT_MET void Directory::mkdir(const String &dirname_)
{
	File_sysiface::mkdir(dirname_);
}

DR_EXPORT_MET Handle::HandleType Directory::getEntryType(const String &entryname)
{
	return File::getType(dirname+"/"+entryname);
}

DR_IO_NS_END
