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
#include <dr/io/FileFailedException.hxx>

DR_IO_NS_BEGIN


DR_THROW_DEF(DR_IO_NS_STR, FileFailedException, IoException);
DR_THROW_IMPL_SIMPLE(FileFailedException);

DR_EXPORT_MET FileFailedException::FileFailedException(const String &filename_, const String &operation_, int posix_error_, int minor_error_):
	IoException(NULL, operation_, posix_error_, minor_error_),
	filename(filename_)
{
}

DR_EXPORT_MET FileFailedException::FileFailedException(Throwable *previous_, const String &filename_, const String &operation_, int posix_error_, int minor_error_):
	IoException(previous_, NULL, operation_, posix_error_, minor_error_),
	filename(filename_)
{
}

DR_EXPORT_MET String FileFailedException::stringify() const
{
	String desc(operation);
	desc.append(" failed: file=").append(filename).append(": ")
#ifndef DR_OS_WCE
		.append(strerror(posix_error))
#else
		.append("posix error: ").appendNumber(posix_error);
#endif
		;
	return desc;
}


DR_IO_NS_END
