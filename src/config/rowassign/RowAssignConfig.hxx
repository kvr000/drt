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

#ifndef dr__config__RowAssignConfig__hxx__
# define dr__config__RowAssignConfig__hxx__

#include <dr/config/def_config.hxx>
#include <dr/io/def_io.hxx>
#include <dr/Hash.hxx>

#include <dr/Object.hxx>

DR_IO_NS_BEGIN
class Handle;
class File;
class StreamBuffer;
DR_IO_NS_END

DR_CONFIG_NS_BEGIN

DR_IO_NS_USE;


class RowAssignConfig: public Object
{
	DR_OBJECT_DECL(RowAssignConfig, Object);

public:
	/* constructor */		RowAssignConfig(const String &file);
	/* constructor */		RowAssignConfig(Handle *handle);
	/* constructor */		RowAssignConfig(StreamBuffer *stream);

protected:
	virtual				~RowAssignConfig();

public:
	virtual String			getValue(const String &key);
	virtual String			getValue(const String &key, const String &default_value);

protected:
	void				initialize(StreamBuffer *stream);

protected:
	Hash<String, String>		content;
};


DR_CONFIG_NS_END

#endif
