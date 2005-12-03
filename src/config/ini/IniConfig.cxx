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
 * include:	dr/config/RowAssignConfig.hxx
 * ns:		dr::config
 */

#include <dr/x_kw.hxx>
#include <dr/Const.hxx>

#include <dr/io/File.hxx>
#include <dr/io/StreamBuffer.hxx>

#include <dr/config/NoValueException.hxx>

#include <dr/config/IniConfig.hxx>
#include "_gen/IniConfig-all.hxx"

DR_CONFIG_NS_BEGIN


/*drt
 * class:	IniConfig
 * ancestor:	dr::Object
 *
 * at:	RHash<String, THash<String, String> > values;
 *
 * doc:{
 * 	Access to ini files
 * }doc
 */

DR_MET(public)
IniConfig::IniConfig(const String &filename)
{
	initialize(tref(new dr::io::StreamBuffer(tref(new dr::io::File(filename, dr::io::File::M_READ)))));
}

DR_MET(public)
IniConfig::IniConfig(dr::io::Handle *file)
{
	initialize(tref(new dr::io::StreamBuffer(file)));
}

DR_MET(public)
IniConfig::IniConfig(dr::io::StreamBuffer *sb)
{
	initialize(sb);
}

DR_MET(protected virtual)
IniConfig::~IniConfig()
{
}

DR_MET(protected virtual)
void IniConfig::initialize(dr::io::StreamBuffer *stream)
{
	THash<String, String> *section = values[String()].setNoref(new THash<String, String>);
	Blob line;
	while (!(line = stream->tryReadLine()).isNull()) {
		Blob key;
		Blob value;
		ssize_t p;
		if ((p = line.findChar('#')) >= 0)
			line.removeMid(p);
		line.trimSpaces();
		if (line.getSize() == 0)
			continue;
		if (line[0] == '[') {
			ssize_t e = line.findChar(']');
			if (e >= 0) {
				line = line.mid(1, e-1);
				if ((section = values[String(line)]) == NULL)
					section = values[String(line)].setNoref(new THash<String, String>);
				continue;
			}
		}
		if ((p = line.findChar('=')) < 0)
			continue;
		key = line.left(p); key.trimSpaces();
		value = line.mid(p+1); value.trimSpaces();
		(*section)[String(key.toStr(), key.getSize())] = String(value.toStr(), value.getSize());
	}
}

DR_MET(public virtual)
String IniConfig::getValue(const String &section, const String &key)
{
	String val;
	if ((val = getValue(section, key, dr::Null())).isNull())
		xthrownew(NoValueException(this, String("[")+section+"]/"+key));
	return val;
}

DR_MET(public virtual)
String IniConfig::getValue(const String &section, const String &key, const String &default_val)
{
	for (String partial_sec = section;;) {
		if (Ref<THash<String, String> > *s = values.accValue(partial_sec)) {
			if (String *v = (*s)->accValue(key))
				return *v;
		}
		if (partial_sec.isEmpty())
			return default_val;
		ssize_t p;
		if ((p = partial_sec.find(String("/"))) < 0)
			partial_sec.setEmpty();
		else
			partial_sec = partial_sec.mid(p+1);
	}
}

DR_MET(public virtual)
RowAssignConfig *IniConfig::getSection(const String &section)
{
	return new IniConfig::RowAssignAccess(this, section);
}


/*drt
 * class:	IniConfig::RowAssignAccess
 * ancestor:	RowAssignConfig
 *
 * at:	Ref<IniConfig>			owner;
 * at:	String				section;
 *
 * doc:{
 * 	Access to ini files
 * }doc
 */

DR_MET(public)
IniConfig::RowAssignAccess::RowAssignAccess(IniConfig *owner_, const String &section_):
	RowAssignConfig(Null()),
	owner(owner_, true),
	section(section_)
{
}

DR_MET(public virtual)
String IniConfig::RowAssignAccess::getValue(const String &key)
{
	return owner->getValue(section, key);
}

DR_MET(public virtual)
String IniConfig::RowAssignAccess::getValue(const String &key, const String &default_value)
{
	return owner->getValue(section, key, default_value);
}


DR_CONFIG_NS_END
