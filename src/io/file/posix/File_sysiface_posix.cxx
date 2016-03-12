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

#include <errno.h>

#include <dr/io/File.hxx>
#include <dr/io/IoException.hxx>
#include <dr/io/NonblockException.hxx>
#include <dr/io/FileNotFoundException.hxx>
#include <dr/io/FileExistsException.hxx>

#include <dr/io/dev/File_sysiface.hxx>

#include <dr/x_kw.hxx>
#include <dr/pe_error.hxx>

#ifdef dr__io__File_sysiface_posix__hxx__


DR_IO_NS_BEGIN


void File_sysiface_posix::throwSysException(File *handle, const String &operation)
{
	switch (errno) {
	case EAGAIN:
#if EAGAIN != EWOULDBLOCK
	case EWOULDBLOCK:
#endif
		xthrownew(NonblockException(handle, operation, DRP_EAGAIN, errno));

	default:
		xthrownew(IoException(handle, operation, errno, errno));
	}
}

void File_sysiface_posix::throwFileStaticSysException(const String &filename, const String &operation)
{
	switch (errno) {
	case EAGAIN:
#if EAGAIN != EWOULDBLOCK
	case EWOULDBLOCK:
#endif
		xthrownew(NonblockException(NULL, operation, DRP_EAGAIN, errno));

	case EEXIST:
		xthrownew(FileExistsException(filename, operation, errno, errno));

	case ENOENT:
		xthrownew(FileNotFoundException(filename, operation, errno, errno));

	default:
		xthrownew(FileFailedException(filename, operation, errno, errno));
	}
}


DR_IO_NS_END

#endif
