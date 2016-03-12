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

#ifndef dr__io__File__hxx__
# define dr__io__File__hxx__

#include <dr/io/Handle.hxx>

DR_IO_NS_BEGIN


/**
 * File handle
 *
 * Handle to real file on filesystem
 */
class DR_IO_PUB File: public Handle
{
	DR_OBJECT_DECL_SIMPLE(File, Handle);

public:
	enum LockType
	{
		LT_Shared			= 1,
		LT_Exclusive			= 2,
	};

public:
	DR_CONSTRUCT			File(const String &filename, Mode mode);

	static File *			createFromOsHandle(void *os_handle, Mode mode);

	DRF_REDIR_HANDLE_HIGH();

public:
	virtual ssize_t			read(void *buf, size_t size);
	virtual ssize_t			write(const void *buf, size_t size);
	virtual FileOffset		seek(FileOffset off, int how);
	virtual void			truncate(FileOffset off);
	virtual void			lockFull(int type);
	virtual void			unlockFull(int type);
	virtual void			updateTime(Sint64 time);
	virtual void			close();

protected:
	virtual				~File();

protected:
	DR_CONSTRUCT			File(Mode mode);

protected:
	String				filename;

protected:
	void *				os_handle;

public:
	static void			unlink(const String &filename);
	static void			rename(const String &filename, const String &tofilename_);
	static HandleType		getType(const String &filename);

	static Blob			readFile(const String &filename);
	static void			writeFile(const String &filename, const Blob &content);

public:
	static const String		unlink_string;
	static const String		rename_string;
	static const String		open_string;
	static const String		stat_string;

private:
	friend class File_sysiface_base;
};


DR_IO_NS_END

#endif
