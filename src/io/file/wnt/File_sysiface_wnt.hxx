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

#include <dr/io/dev/File_sysiface.hxx>

#ifndef dr__io__File_sysiface_wnt__hxx__
# define dr__io__File_sysiface_wnt__hxx__

#include <winsock2.h>
#include <windows.h>

#include <dr/x_kw.hxx>

#include <dr/io/File.hxx>

#include <dr/UnsupportedExcept.hxx>
#include <dr/OverflowExcept.hxx>
#include <dr/io/FileFailedExcept.hxx>

DR_IO_NS_BEGIN


/**
 * File POSIX system interface
 */
class DR_IO_PUB File_sysiface_wnt: File_sysiface_base
{
public:
	static void			throwSysException(File *handle, const String &operation, int err);
	static void			throwFileStaticSysException(const String &filename, const String &operation, int err);

	static void *			getOsHandle(File *handle);
	static bool			isHandleInvalid(File *handle);
	static void			setHandleInvalid(File *handle);
	static void *			getOsHandleInvalid()			{ return (void *)INVALID_HANDLE_VALUE; }

	static HANDLE			getDirOsHandle(Directory *handle);
	static bool			isDirHandleInvalid(Directory *handle);
	static void			setDirHandleInvalid(Directory *handle);
	static void *			getDirHandleInvalid()			{ return (void *)INVALID_HANDLE_VALUE; }

	static void *			open(const String &file, Handle::Mode mode);
	static void			close(File *handle);

	static ssize_t			read(File *handle, void *buf, size_t buflen);
	static ssize_t			write(File *handle, const void *buf, size_t buflen);
	static FileOffset		seek(File *handle, FileOffset offset, int how);
	static void			truncate(File *handle, FileOffset offset);

	static void			lockFull(File *handle, int type);
	static void			unlockFull(File *handle, int type);

	static void			updateTime(File *handle, Sint64 time);

	static File::HandleType		getType(const String &file);
	static void			unlink(const String &file);
	static void			rename(const String &file, const String &tofile);


	static void *			openDir(Directory *handle, const String &path);
	static void			closeDir(Directory *handle);

	static bool			nextDirEntry(Directory *handle, String *entry);
	static void			rmdir(const String &entry);


	static int			mapWntErrorToPosix(int error);
};


DR_RINLINE void *File_sysiface_wnt::getOsHandle(File *handle)
{
	return b_getOsHandle(handle);
}

DR_RINLINE bool File_sysiface_wnt::isHandleInvalid(File *handle)
{
	return getOsHandle(handle) == (void *)INVALID_HANDLE_VALUE;
}

DR_RINLINE void File_sysiface_wnt::setHandleInvalid(File *handle)
{
	b_setHandle(handle, (void *)INVALID_HANDLE_VALUE);
}

DR_RINLINE void *File_sysiface_wnt::open(const String &file, Handle::Mode mode)
{
	void *		fd;
	int		aflags;
	int		oflags;

	switch (mode&(Handle::M_READ|Handle::M_WRITE)) {
	case 0:
		return (void *)INVALID_HANDLE_VALUE;

	case Handle::M_READ:
		aflags = GENERIC_READ;
		break;

	case Handle::M_WRITE:
		aflags = GENERIC_WRITE;
		break;

	case (int)Handle::M_READ|(int)Handle::M_WRITE:
		aflags = GENERIC_READ|GENERIC_WRITE;
		break;

	default: /* to make compiler happy */
		DR_AssertInvalid();
		return INVALID_HANDLE_VALUE;
	}

	switch (mode&(Handle::M_CREATE|Handle::M_EXCL|Handle::M_TRUNC)) {
	case 0:
		oflags = OPEN_EXISTING;
		break;
	case Handle::M_TRUNC:
		oflags = TRUNCATE_EXISTING;
		break;
	case Handle::M_EXCL:
	case (int)Handle::M_EXCL|Handle::M_TRUNC:
		DR_THROWNEW(UnsupportedExcept(NULL, File::comp_name, "open_mode", "M_EXCL|M_TRUNC"));
		return (void *)INVALID_HANDLE_VALUE;
	case Handle::M_CREATE:
		oflags = OPEN_ALWAYS;
		break;
	case (int)Handle::M_CREATE|Handle::M_TRUNC:
		oflags = CREATE_ALWAYS;
		break;
	case (int)Handle::M_CREATE|Handle::M_EXCL:
	case (int)Handle::M_CREATE|Handle::M_EXCL|Handle::M_TRUNC:
		oflags = CREATE_NEW;
		break;
	default: /* to make compiler happy */
		DR_AssertInvalid();
		return INVALID_HANDLE_VALUE;
	}

	if ((fd = CreateFileW(file.wide().toStr(), aflags, 0, NULL, oflags, 0, NULL)) == INVALID_HANDLE_VALUE) {
		throwFileStaticSysException(file, File::open_string, GetLastError());
	}
	return (void *)fd;
}

DR_RINLINE void File_sysiface_wnt::close(File *handle)
{
	CloseHandle(getOsHandle(handle));
}

DR_RINLINE ssize_t File_sysiface_wnt::read(File *handle, void *buf, size_t buflen)
{
	DWORD br = buflen > 0x7ffffff0 ? 0x7ffffff0 : buflen;
	if (!ReadFile(getOsHandle(handle), buf, br, &br, NULL))
		throwSysException(handle, Handle::read_string, GetLastError());
	return br;
}

DR_RINLINE ssize_t File_sysiface_wnt::write(File *handle, const void *buf, size_t buflen)
{
	DWORD br = buflen > 0x7ffffff0 ? 0x7ffffff0 : buflen;
	if (!WriteFile(getOsHandle(handle), buf, br, &br, NULL))
		throwSysException(handle, Handle::write_string, GetLastError());
	return br;
}

DR_RINLINE FileOffset File_sysiface_wnt::seek(File *handle, FileOffset offset, int how)
{
#ifdef DR_OS_WCE
	LONG ofl = offset;
	LONG ofh = offset;
	if ((ofl = SetFilePointer(getOsHandle(handle), ofl, &ofh, how)) == 0xffffffff) {
		if (int err = GetLastError())
			throwSysException(handle, Handle::seek_string, err);
	}
	offset = ((FileOffset)ofh<<32)|ofl;
#else
	Sint64 of = offset;
	if (!SetFilePointerEx(getOsHandle(handle), *(LARGE_INTEGER *)&of, (LARGE_INTEGER *)&of, how))
		throwSysException(handle, Handle::seek_string, GetLastError());
	offset = of;
#endif
	return offset;
}

DR_RINLINE void File_sysiface_wnt::truncate(File *handle, FileOffset offset)
{
	FileOffset old_pos = seek(handle, offset, 1);
	if (::SetEndOfFile(getOsHandle(handle)) < 0)
		throwSysException(handle, Handle::truncate_string, GetLastError());
	seek(handle, old_pos, 0);
}

DR_RINLINE void File_sysiface_wnt::lockFull(File *handle, int type)
{
	OVERLAPPED linfo;
	linfo.Offset = 0;
	linfo.OffsetHigh = 0;
	linfo.hEvent = NULL;
	if (!LockFileEx(getOsHandle(handle), ((type&2) ? LOCKFILE_EXCLUSIVE_LOCK : 0), 0, 0xffffffff, 0x7fffffff, &linfo))
		throwSysException(handle, Handle::lock_string, GetLastError());
}

DR_RINLINE void File_sysiface_wnt::unlockFull(File *handle, int type)
{
	OVERLAPPED linfo;
	linfo.Offset = 0;
	linfo.OffsetHigh = 0;
	linfo.hEvent = NULL;
	if (!UnlockFileEx(getOsHandle(handle), 0, 0xffffffff, 0x7fffffff, &linfo))
		throwSysException(handle, Handle::lock_string, GetLastError());
}

DR_RINLINE void File_sysiface_wnt::updateTime(File *handle, Sint64 time)
{
	time = (Time::toSeconds(time)+11644473600)*10000000;
	FILETIME ftime;
	ftime.dwLowDateTime = (DWORD)time;
	ftime.dwHighDateTime = (DWORD)(time>>32);
	if (!SetFileTime(getOsHandle(handle), NULL, &ftime, &ftime))
		throwSysException(handle, Handle::lock_string, GetLastError());
}

DR_RINLINE File::HandleType File_sysiface_wnt::getType(const String &filename)
{
	DWORD attrs;
	if ((attrs = GetFileAttributesW(filename.wide().toStr())) == INVALID_FILE_ATTRIBUTES) {
		throwFileStaticSysException(filename, File::stat_string, GetLastError());
	}
	if (attrs&FILE_ATTRIBUTE_DIRECTORY)
		return File::HT_Directory;
	else if (attrs&FILE_ATTRIBUTE_DEVICE)
		return File::HT_DevChar;
	else
		return File::HT_File;
}

DR_RINLINE void File_sysiface_wnt::rename(const String &filename, const String &tofilename)
{
	if (!::MoveFileW(filename.wide().toStr(), tofilename.wide().toStr()))
		throwFileStaticSysException(filename, File::rename_string, GetLastError());
}

DR_RINLINE void File_sysiface_wnt::unlink(const String &filename)
{
	if (!DeleteFileW(filename.wide().toStr())) {
		throwFileStaticSysException(filename, File::unlink_string, GetLastError());
	}
}


DR_RINLINE HANDLE File_sysiface_wnt::getDirOsHandle(Directory *handle)
{
	return (HANDLE)(SintPtr)b_getDirOsHandle(handle);
}

DR_RINLINE bool File_sysiface_wnt::isDirHandleInvalid(Directory *handle)
{
	return getDirOsHandle(handle) != INVALID_HANDLE_VALUE;
}

DR_RINLINE void File_sysiface_wnt::setDirHandleInvalid(Directory *handle)
{
	b_setDirHandle(handle, (void *)(SintPtr)INVALID_HANDLE_VALUE);
}

DR_RINLINE void *File_sysiface_wnt::openDir(Directory *handle, const String &pathname)
{
	HANDLE fd;
	*(WIN32_FIND_DATAW **)(void *)&handle->sys_pos = (WIN32_FIND_DATAW *)Alloc::allocS(sizeof(WIN32_FIND_DATAW));
	if ((fd = FindFirstFileW((pathname.wide()+L"\\*").toStr(), (WIN32_FIND_DATAW *)(void *)(SintPtr)handle->sys_pos)) == INVALID_HANDLE_VALUE) {
		Alloc::freeS((void *)(SintPtr)handle->sys_pos, sizeof(WIN32_FIND_DATAW));
		throwFileStaticSysException(pathname, File::open_string, GetLastError());
	}
	handle->cache_pos = 1;
	return (void *)(SintPtr)fd;
}

DR_RINLINE void File_sysiface_wnt::closeDir(Directory *handle)
{
	Alloc::freeS((void *)(SintPtr)handle->sys_pos, sizeof(WIN32_FIND_DATAW));
	FindClose(getDirOsHandle(handle));
}

DR_RINLINE void File_sysiface_wnt::rmdir(const String &filename)
{
	if (!::RemoveDirectoryW(filename.wide().toStr()))
		throwFileStaticSysException(filename, Directory::rmdir_string, GetLastError());
}

DR_RINLINE bool File_sysiface_wnt::nextDirEntry(Directory *handle, String *entry)
{
	if (handle->cache_pos) {
		entry->setWide(((WIN32_FIND_DATAW *)(void *)(SintPtr)handle->sys_pos)->cFileName);
		handle->cache_pos = 0;
		return true;
	}
	if (FindNextFileW(getDirOsHandle(handle), ((WIN32_FIND_DATAW *)(void *)(SintPtr)handle->sys_pos))) {
		entry->setWide(((WIN32_FIND_DATAW *)(void *)(SintPtr)handle->sys_pos)->cFileName);
		return true;
	}
	return false;
}

DR_IO_NS_END

#endif
