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

#ifndef dr__io__Directory__hxx__
# define dr__io__Directory__hxx__

#include <dr/io/def_io.hxx>

#include <dr/Blob.hxx>

#include <dr/io/Handle.hxx>

#include <dr/Object.hxx>

DR_IO_NS_BEGIN


/**
 * Directory handle
 *
 * Handle to real directory entry on filesystem
 */
class DR_IO_PUB Directory: public Object
{
	DR_OBJECT_DECL_SIMPLE(Directory, Object);

public:
	DR_CONSTRUCT			Directory(const String &dirname);

protected:
	virtual				~Directory();

public:
	static void			remove(const String &directory);

public:
	virtual bool			getNext(String *entry);

public:
	virtual Handle::HandleType	getEntryType(const String &entry);

protected:
	String				dirname;

protected:
	void *				os_handle;

public:
	Blob				cache;
	size_t				cache_pos;
	Sint64				sys_pos;

public:
	static const String		rmdir_string;

private:
	friend class File_sysiface_base;
};


DR_IO_NS_END

#endif
