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

/*drt
 * include: 	dr/String.hxx
 * include:	dr/Hash.hxx
 * include:	dr/io/Handle.hxx
 * include:	dr/io/StreamBuffer.hxx
 * include:	dr/config/def_config.hxx
 *
 * ns:		dr::config
 */

#include <dr/x_kw.hxx>
#include <dr/Const.hxx>

#include <dr/io/File.hxx>
#include <dr/io/StreamBuffer.hxx>

#include <dr/config/NoValueException.hxx>

#include <dr/config/RowAssignConfig.hxx>
#include "_gen/RowAssignConfig-all.hxx"

DR_CONFIG_NS_BEGIN


/*drt
 * class:	RowAssignConfig
 * ancestor:	dr::Object
 *
 * at:	THash<String, String>		values;
 *
 * doc:{
 * 	Access to ini files
 * }doc
 */

DR_MET(protected)
RowAssignConfig::RowAssignConfig(const Null &)
{
}

DR_MET(public)
RowAssignConfig::RowAssignConfig(const String &filename)
{
	initialize(tref(new dr::io::StreamBuffer(tref(new dr::io::File(filename, dr::io::File::M_READ)))));
}

DR_MET(public)
RowAssignConfig::RowAssignConfig(dr::io::Handle *file)
{
	initialize(tref(new dr::io::StreamBuffer(file)));
}

DR_MET(public)
RowAssignConfig::RowAssignConfig(dr::io::StreamBuffer *sb)
{
	initialize(sb);
}

DR_MET(protected virtual)
RowAssignConfig::~RowAssignConfig()
{
}

DR_MET(protected)
void RowAssignConfig::initialize(dr::io::StreamBuffer *stream)
{
	Blob line;
	while (!(line = stream->tryReadLine()).isNull()) {
		Blob key;
		Blob value;
		ssize_t p;
		if ((p = line.find('#')) >= 0)
			line.removeMid(p);
		line.trimSpaces();
		if (line.getSize() == 0)
			continue;
		if ((p = line.find('=')) < 0)
			continue;
		key = line.left(p); key.trimSpaces();
		value = line.mid(p+1); value.trimSpaces();
		values[String(key.toStr(), key.getSize())] = String(value.toStr(), value.getSize());
	}
}

DR_MET(public virtual)
String RowAssignConfig::getValue(const String &key)
{
	String val = getValue(key, Null());
	if (val.isNull())
		xthrownew(NoValueException(this, key));
	return val;
}

DR_MET(public virtual)
String RowAssignConfig::getValue(const String &key, const String &default_val)
{
	if (String *v = values.accValue(key))
		return *v;
	return default_val;
}


DR_CONFIG_NS_END
