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

#ifndef dr__io__IoExcept__hxx__
# define dr__io__IoExcept__hxx__

#include <dr/Exception.hxx>

#include <dr/io/def_io.hxx>

DR_IO_NS_BEGIN


class Handle;

/**
 * Io Exception
 *
 * Caused by input/output failure
 */
class DR_IO_PUB IoException: public Exception
{
	DR_THROW_DECL_SIMPLE(IoException, Exception);

public:
	/**
	 * construct IoException:
	 *
	 * @param handle
	 * 	handle that caused the exception
	 * @param operation
	 * 	operation that caused the exception
	 * @param posix_error
	 * 	posix error code
	 */
	DR_CONSTRUCT			IoException(Handle *handle, const String &operation, int posix_error, int minor_error);

	/**
	 * construct IoException:
	 *
	 * @param previous
	 * 	specifies exception that caused this one, the reference is lost
	 * @param handle
	 * 	handle that caused the exception
	 * @param operation
	 * 	operation that caused the exception
	 * @param posix_error
	 * 	posix error code
	 */
	DR_CONSTRUCT			IoException(Throwable *previous, Handle *handle, const String &operation, int posix_error, int minor_error);

public:
	/**
	 * get description
	 *
	 * @return
	 * 	formatted error description
	 */
	virtual String			stringify() const;

	/**
	 * get operation
	 *
	 * @return
	 * 	operation that failed
	 */
	virtual String			getOperation();

	/**
	 * get failure handle
	 *
	 * @return
	 * 	handle on which the error occured
	 */
	virtual Handle *		getHandle();

	/**
	 * get posix error
	 *
	 * @return		 	posix error id
	 */
	virtual int			getPosixError();

	/**
	 * get minor error (native specific)
	 *
	 * @return			minor error id
	 */
	virtual int			getMinorError();

protected:
	Ref<Handle>			handle;
	String				operation;
	int				posix_error;
	int				minor_error;
};


DR_IO_NS_END

#endif
