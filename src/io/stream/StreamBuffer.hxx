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

#ifndef dr__io__StreamBuffer__hxx__
# define dr__io__StreamBuffer__hxx__

#include <dr/Ref.hxx>

#include <dr/Time.hxx>

#include <dr/io/Handle.hxx>

DR_IO_NS_BEGIN


class DR_IO_PUB StreamBuffer: public Object
{
	DR_OBJECT_DECL(Stream, Object);

public:
	/* constructor */		StreamBuffer(Handle *handle);

public:
	virtual				~StreamBuffer();

public:
	virtual void			reinit(Handle *handle);

public:
	DR_RINLINE Handle *		accHandle()				{ return *handle; }
	DR_RINLINE Blob *		ptrCache()				{ return &cache; }
	DR_RINLINE void			shrinkCache(size_t size)		{ cache.removeLeft(size); }

public:
	virtual void			setTimeLimit(SysTime abs_time)		{ time_limit = abs_time; }

public:
	virtual Blob			readLine();
	/**
	 * reads until specified character is found
	 *
	 * @param search
	 * 	character to be found
	 * @return position before the character
	 * @throw EndOfDataExcept 	if the character is not found
	 */
	virtual ssize_t			readUntilChar(char search);
	virtual ssize_t			read(Blob *data, size_t maxsize);

	/**
	 * extends the data by reading up to add_size characters
	 *
	 * @param data
	 * 	the buffer to store the data
	 * @param add_size
	 * 	recommended number of characters to add
	 * @return
	 * 	number of characters added/read (always positive)
	 */
	virtual ssize_t			readExtendAdd(Blob *data, size_t add_size);
	virtual ssize_t			readFull(Blob *data, size_t fullsize);

	virtual ssize_t			write(const Blob &data);
	virtual ssize_t			writeFull(const Blob &data);

	/**
	 * extend cache to to_size
	 *
	 * @param to_size		requested new size of cache
	 * @return pointer to cache
	 */
	virtual Blob *			extendCache(size_t to_size);

	/**
	 * extend cache by add_size characters
	 *
	 * @param add_size		number of characters to extend the cache
	 * @return pointer to cache
	 */
	virtual Blob *			extendCacheAdd(size_t add_size);

	/**
	 * extend cache by add_size
	 *
	 * @param add_size
	 * 	recommended number of characters to extend
	 * @return new size of the cache
	 */
	virtual size_t			tryExtendCache(size_t add_size);

	/**
	 * extend cache to to_size
	 *
	 * @param to_size		requested new size of cache
	 * @return added characters to the cache
	 */
	virtual size_t			tryExtendCacheAdd(size_t to_size);

	/**
	 * extend cache until certain character is read
	 *
	 * @param search
	 * 	the character to be found
	 * @return
	 * 	-1 if the character is not found
	 *	otherwise position of the character
	 */
	virtual ssize_t			tryReadUntilChar(char search);

	/**
	 * reads one line from the source
	 *
	 * @return the line
	 * @return Blob(null) if there is no more lines
	 */
	virtual Blob			tryReadLine();

	/**
	 * set cache suggested size
	 *
	 * @param suggested_size
	 * 	number of characters to read at once
	 */
	virtual void			setCacheSize(size_t suggested_size);

protected:
	Ref<Handle>			handle;
	Blob				cache;
	size_t				cache_suggest;
	SysTime				time_limit;
};


DR_IO_NS_END

#endif
