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

#ifndef dr__io__Handle__hxx__
# define dr__io__Handle__hxx__

#include <dr/Object.hxx>
#include <dr/Blob.hxx>

#include <dr/io/Vfs.hxx>

DR_IO_NS_BEGIN


/**
 * Base for all IO handles
 */
class DR_IO_PUB Handle: public Object
{
	DR_OBJECT_DECL_SIMPLE(Handle, Object);

public:
	enum HandleType {
		HT_Pipe			= 0x1,
		HT_DevChar		= 0x2,
		HT_Directory		= 0x4,
		HT_DevBlock		= 0x6,
		HT_File			= 0x8,
		HT_SymLink		= 0xa,
		HT_Socket		= 0xc,
	};

public:
	enum Mode {
		M_READ			= 1,
		M_WRITE			= 2,
		M_EXCEPT		= 4,

		M_CREATE		= 0x040,
		M_EXCL			= 0x080,
		M_TRUNC			= 0x200,
		M_APPEND		= 0x400,

		M_NOCLOSE		= 0x10000000,
	};

protected:
	DR_CONSTRUCT			Handle(Mode mode_);

public:
	virtual Blob			read(size_t size);
	virtual ssize_t			write(const Blob &str);
	virtual ssize_t			read(void *buf, size_t size);
	virtual ssize_t			read(Blob *buf, size_t size, size_t offs);
	virtual ssize_t			write(const void *buf, size_t size);
	virtual FileOffset		seek(FileOffset off, int how);
	virtual void			flush();
	virtual void			setBlocking(bool blocking);

	virtual Mode			waitData(Mode mode, Sint64 nanosec);

protected:
	virtual void			destroy();

protected:
	Mode				mode;

public:
	static const String		close_string;
	static const String		write_string;
	static const String		read_string;
	static const String		seek_string;
	static const String		truncate_string;
	static const String		wait_string;
	static const String		lock_string;
};


#define DRF_REDIR_HANDLE_HIGH() \
	DR_SUPER_REDIR1(Blob, read, size_t); \
	DR_SUPER_REDIR3(ssize_t, read, Blob *, size_t, size_t); \
	DR_SUPER_REDIR1(ssize_t, write, const Blob &); \


DR_ENUM_OPS(Handle::Mode);


DR_RINLINE Handle::Handle(Mode mode_):
	mode(mode_)
{
}

DR_RINLINE void Handle::flush()
{
}

DR_RINLINE void Handle::destroy()
{
	flush();
	Super::destroy();
}


DR_IO_NS_END

#endif
