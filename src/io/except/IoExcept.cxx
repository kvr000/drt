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

#include <dr/io/IoExcept.hxx>
#include <dr/io/Handle.hxx>

DR_IO_NS_BEGIN


DR_THROW_DEF(DR_IO_NS_STR, IoExcept, Except);
DR_THROW_IMPL_SIMPLE(IoExcept);

DR_EXPORT_MET IoExcept::IoExcept(Handle *handle_, const String &operation_, int posix_error_, int minor_error_):
	handle(handle_, true),
	operation(operation_),
	posix_error(posix_error_),
	minor_error(minor_error_)
{
}

DR_EXPORT_MET IoExcept::IoExcept(Throwable *previous_, Handle *handle_, const String &operation_, int posix_error_, int minor_error_):
	Except(previous_),
	handle(handle_, true),
	operation(operation_),
	posix_error(posix_error_),
	minor_error(minor_error_)
{
}

DR_EXPORT_MET String IoExcept::stringify() const
{
	String desc("io exception: ");
	if (handle) {
		desc.append(handle->classname()).append("@").appendPointer((Object *)handle).append(": ");
	}
	if (!operation.isNull()) {
		desc.append(operation).append(": ");
	}
#ifndef DR_OS_WCE
	desc.append(strerror(posix_error));
#else
	desc.append("posix error=").appendNumber(posix_error);
#endif
	desc.append(" minor error=").appendNumber(minor_error);
	return desc;
}

DR_EXPORT_MET String IoExcept::getOperation()
{
	return operation;
}

DR_EXPORT_MET Handle *IoExcept::getHandle()
{
	handle->ref();
	return handle;
}

DR_EXPORT_MET int IoExcept::getPosixError()
{
	return posix_error;
}

DR_EXPORT_MET int IoExcept::getMinorError()
{
	return minor_error;
}


DR_IO_NS_END
