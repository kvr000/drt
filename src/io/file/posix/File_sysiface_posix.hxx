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

#ifndef dr__io__File_sysiface_posix__hxx__
# define dr__io__File_sysiface_posix__hxx__

#include <errno.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>
#include <sys/file.h>
#include <sys/time.h>

#include <dr/io/File.hxx>
#include <dr/io/dev/File_sysiface.hxx>
#include <dr/Time.hxx>

#include <dr/io/FileFailedException.hxx>

#include <dr/x_kw.hxx>
#include <dr/pe_error.hxx>

DR_IO_NS_BEGIN

/**
 * File POSIX system interface
 */
class DR_IO_PUB File_sysiface_posix: File_sysiface_base
{
public:
	static void			throwSysException(File *handle, const String &operation);
	static void			throwFileStaticSysException(const String &filename, const String &operation);

	static int			getOsHandle(File *handle);
	static bool			isHandleInvalid(File *handle);
	static void			setHandleInvalid(File *handle);
	static void *			getOsHandleInvalid()			{ return (void *)-1L; }

	static int			getDirOsHandle(Directory *handle);
	static bool			isDirHandleInvalid(Directory *handle);
	static void			setDirHandleInvalid(Directory *handle);
	static void *			getDirHandleInvalid()			{ return (void *)-1L; }

	static void *			open(const String &file, Handle::Mode mode);
	static void			close(File *handle);

	static ssize_t			read(File *handle, void *buf, size_t buflen);
	static ssize_t			write(File *handle, const void *buf, size_t buflen);
	static FileOffset		seek(File *handle, FileOffset offset, int how);
	static void			truncate(File *handle, FileOffset offset);

	static void			lockFull(File *handle, int type);
	static void			unlockFull(File *handle, int type);

	static void			updateTime(File *handle, SysTime time);

	static File::HandleType		getType(const String &file);
	static void			unlink(const String &file);
	static void			rename(const String &file, const String &tofile);


	static void *			openDir(Directory *handle, const String &path);
	static void			closeDir(Directory *handle);

	static bool			nextDirEntry(Directory *handle, String *entry);
	static void			rmdir(const String &dirname);
	static void			mkdir(const String &dirname);
};

#ifndef O_LARGEFILE
# define O_LARGEFILE 0
#endif

DR_RINLINE int File_sysiface_posix::getOsHandle(File *handle)
{
	return (int)(SintPtr)b_getOsHandle(handle);
}

DR_RINLINE bool File_sysiface_posix::isHandleInvalid(File *handle)
{
	return getOsHandle(handle) < 0;
}

DR_RINLINE void File_sysiface_posix::setHandleInvalid(File *handle)
{
	b_setHandle(handle, (void *)(SintPtr)-1);
}

DR_RINLINE void *File_sysiface_posix::open(const String &file, Handle::Mode mode)
{
	int fd;
	int oflags;

	switch (mode&(Handle::M_READ|Handle::M_WRITE)) {
	case 0:
		return (void *)(SintPtr)-1;

	case Handle::M_READ:
		oflags = O_RDONLY;
		break;

	case Handle::M_WRITE:
		oflags = O_WRONLY;
		break;

	case (int)Handle::M_READ|(int)Handle::M_WRITE:
		oflags = O_RDWR;
		break;

	default: /* to make compiler happy */
		DR_AssertInvalid();
		return (void *)(SintPtr)-1;
	}

	if ((mode &= ~(Handle::M_READ|Handle::M_WRITE)) == 0)
		goto go;
	if (mode&Handle::M_CREATE) {
		oflags |= O_CREAT;
		if ((mode &= ~Handle::M_CREATE) == 0)
			goto go;
	}
	if (mode&Handle::M_TRUNC) {
		oflags |= O_TRUNC;
		if ((mode &= ~Handle::M_TRUNC) == 0)
			goto go;
	}
	if (mode&Handle::M_APPEND) {
		oflags |= O_APPEND;
		if ((mode &= ~Handle::M_APPEND) == 0)
			goto go;
	}

go:
	if ((fd = ::open(file.utf8().toStr(), oflags|O_LARGEFILE, 0666)) < 0)
		throwFileStaticSysException(file, File::open_string);
	return (void *)(SintPtr)fd;
}

DR_RINLINE void File_sysiface_posix::close(File *handle)
{
	::close(getOsHandle(handle));
}

DR_RINLINE ssize_t File_sysiface_posix::read(File *handle, void *buf, size_t buflen)
{
	ssize_t br;
	if ((br = ::read(getOsHandle(handle), buf, buflen)) < 0)
		throwSysException(handle, Handle::read_string);
	return br;
}

DR_RINLINE ssize_t File_sysiface_posix::write(File *handle, const void *buf, size_t buflen)
{
	ssize_t br;
	if ((br = ::write(getOsHandle(handle), buf, buflen)) < 0)
		throwSysException(handle, Handle::write_string);
	return br;
}

DR_RINLINE FileOffset File_sysiface_posix::seek(File *handle, FileOffset offset, int how)
{
	ssize_t br;
	if ((br = ::lseek(getOsHandle(handle), offset, how)) < 0)
		throwSysException(handle, Handle::seek_string);
	return br;
}

DR_RINLINE void File_sysiface_posix::truncate(File *handle, FileOffset offset)
{
	if (::ftruncate(getOsHandle(handle), offset) < 0)
		throwSysException(handle, Handle::truncate_string);
}

DR_RINLINE void File_sysiface_posix::lockFull(File *handle, int type)
{
	if (::flock(getOsHandle(handle), type) < 0)
		throwSysException(handle, Handle::lock_string);
}

DR_RINLINE void File_sysiface_posix::unlockFull(File *handle, int type)
{
	if (::flock(getOsHandle(handle), LOCK_UN) < 0)
		throwSysException(handle, Handle::lock_string);
}

DR_RINLINE void File_sysiface_posix::updateTime(File *handle, SysTime time)
{
	struct timeval tv[2];
	if (time != Time::INVAL_TIME) {
		tv[0].tv_sec = Time::toSeconds(time);
		tv[0].tv_usec = Time::fractUsecs(time);
		tv[1].tv_sec = Time::toSeconds(time);
		tv[1].tv_usec = Time::fractUsecs(time);
	}
	if (::futimes(getOsHandle(handle), time == Time::INVAL_TIME ? NULL : tv) < 0)
		throwSysException(handle, Handle::lock_string);
}

DR_RINLINE File::HandleType File_sysiface_posix::getType(const String &filename)
{
	struct stat sb;
	if (::stat(filename.utf8().toStr(), &sb) < 0) {
		throwFileStaticSysException(filename, File::stat_string);
	}
	return (File::HandleType)(sb.st_mode>>12);
}

DR_RINLINE void File_sysiface_posix::unlink(const String &filename)
{
	if (::unlink(filename.utf8().toStr()) < 0)
		throwFileStaticSysException(filename, File::unlink_string);
}

DR_RINLINE void File_sysiface_posix::rename(const String &filename, const String &tofilename)
{
	if (::rename(filename.utf8().toStr(), tofilename.utf8().toStr()) < 0)
		throwFileStaticSysException(filename, File::rename_string);
}

DR_RINLINE int File_sysiface_posix::getDirOsHandle(Directory *handle)
{
	return (int)(SintPtr)b_getDirOsHandle(handle);
}

DR_RINLINE bool File_sysiface_posix::isDirHandleInvalid(Directory *handle)
{
	return getDirOsHandle(handle) < 0;
}

DR_RINLINE void File_sysiface_posix::setDirHandleInvalid(Directory *handle)
{
	b_setDirHandle(handle, (void *)(SintPtr)-1);
}

DR_RINLINE void *File_sysiface_posix::openDir(Directory *handle, const String &pathname)
{
#ifdef O_DIRECTORY
	int fd = ::open(pathname.utf8().toStr(), O_RDONLY|O_DIRECTORY|O_LARGEFILE);
	if (fd < 0)
		throwFileStaticSysException(pathname, File::open_string);
#else
	int fd = ::open(pathname.utf8().toStr(), O_RDONLY|O_LARGEFILE);
	if (fd < 0)
		throwFileStaticSysException(pathname, File::open_string);
	struct stat sb;
	if (fstat(fd, &sb) < 0) {
		::close(fd);
		throwFileStaticSysException(pathname, File::stat_string);
	}
	if (!S_ISDIR(sb.st_mode)) {
		::close(fd);
		errno = ENOTDIR;
		throwFileStaticSysException(pathname, File::open_string);
	}
#endif
	handle->cache_pos = 0;
	handle->sys_pos = 0;
	return (void *)(SintPtr)fd;
}

DR_RINLINE void File_sysiface_posix::closeDir(Directory *handle)
{
	::close(getDirOsHandle(handle));
}

DR_RINLINE void File_sysiface_posix::rmdir(const String &dirname)
{
	if (::rmdir(dirname.utf8().toStr()) < 0)
		throwFileStaticSysException(dirname, Directory::rmdir_string);
}

DR_RINLINE void File_sysiface_posix::mkdir(const String &dirname)
{
	if (::mkdir(dirname.utf8().toStr(), 0777) < 0)
		throwFileStaticSysException(dirname, Directory::mkdir_string);
}

DR_RINLINE bool File_sysiface_posix::nextDirEntry(Directory *handle, String *entry)
{
	for (;;) {
		if (handle->cache.getSize()-handle->cache_pos >= (size_t)&((dirent *)NULL)->d_name[0]) {
			dirent de;
			memcpy(&de, handle->cache.toStr()+handle->cache_pos, (size_t)&((dirent *)NULL)->d_name[0]);
			if (handle->cache.getSize()-handle->cache_pos >= de.d_reclen) {
				//Fatal::plog("found %s (%d), type %d\n", ((dirent *)(handle->cache.toStr()+handle->cache_pos))->d_name, ((dirent *)(handle->cache.toStr()+handle->cache_pos))->d_type);
				entry->setUtf8(((dirent *)(handle->cache.toStr()+handle->cache_pos))->d_name);
				handle->cache_pos += de.d_reclen;
				return true;
			}
		}
		handle->cache.removeLeft(handle->cache_pos);
		handle->cache_pos = 0;
#ifdef __FBSDID
		ssize_t err = getdirentries(getDirOsHandle(handle), handle->cache.lockKeep(2*PATH_MAX)+handle->cache.getSize(), 2*PATH_MAX-handle->cache.getSize(), (long *)(void *)&handle->sys_pos);
#else
 		ssize_t err = getdirentries(getDirOsHandle(handle), handle->cache.lockKeep(2*PATH_MAX)+handle->cache.getSize(), 2*PATH_MAX-handle->cache.getSize(), (off_t *)(void *)&handle->sys_pos);
#endif
		if (err < 0) {
			throwSysException(NULL, File::read_string);
		}
		else if (err == 0) {
			return false;
		}
		else {
			handle->cache.unlockAppend(err);
		}
	}
}


DR_IO_NS_END

#endif
