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

#include <dr/x_kw.hxx>
#include <dr/Const.hxx>

#include <dr/io/File.hxx>
#include <dr/io/StreamBuffer.hxx>

#include <dr/config/NoValueExcept.hxx>

#include <dr/config/RowAssignConfig.hxx>

DR_CONFIG_NS_BEGIN


DR_OBJECT_DEF(DR_CONFIG_NS_STR, RowAssignConfig, Object);
DR_OBJECT_IMPL_SIMPLE(RowAssignConfig);


RowAssignConfig::RowAssignConfig(const String &filename)
{
	initialize(tref(new StreamBuffer(tref(new File(filename, File::M_READ)))));
}

RowAssignConfig::RowAssignConfig(Handle *file)
{
	initialize(tref(new StreamBuffer(file)));
}

RowAssignConfig::RowAssignConfig(StreamBuffer *sb)
{
	initialize(sb);
}

RowAssignConfig::~RowAssignConfig()
{
}

void RowAssignConfig::initialize(StreamBuffer *stream)
{
	Blob line;
	while (!(line = stream->tryReadLine()).isNull()) {
		Blob key;
		Blob value;
		ssize_t p;
		if ((p = line.find('#')) >= 0)
			line.removeMid(p);
		line.stripSpaces();
		if (line.getSize() == 0)
			continue;
		if ((p = line.find('=')) < 0)
			continue;
		key = line.left(p); key.stripSpaces();
		value = line.mid(p+1); value.stripSpaces();
		content[String(key.toStr(), key.getSize())] = String(value.toStr(), value.getSize());
	}
}

String RowAssignConfig::getValue(const String &key)
{
	if (Hash<String, String>::kvpair *p = content.find(key))
		return p->v;
	xthrownew(NoValueExcept(this, key));
	return Null();
}

String RowAssignConfig::getValue(const String &key, const String &default_val)
{
	if (Hash<String, String>::kvpair *p = content.find(key))
		return p->v;
	return default_val;
}


DR_CONFIG_NS_END
