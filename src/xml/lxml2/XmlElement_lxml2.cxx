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

#include <dr/xml/XmlException.hxx>

#include <dr/xml/XmlElement_lxml2.hxx>

DRXML_NS_BEGIN


DR_OBJECT_DEF(DRXML_NS_STR, XmlElement_lxml2, XmlElement);
DR_OBJECT_IMPL_SIMPLE(XmlElement_lxml2);


XmlElement_lxml2::XmlElement_lxml2(XmlDoc_lxml2 *owner_, xmlElementPtr element_):
	owner(owner_, true),
	element(element_)
{
}

XmlElement_lxml2::~XmlElement_lxml2()
{
}

String XmlElement_lxml2::getTexts()
{
	if (element->children && element->children->type == XML_TEXT_NODE)
		return (const char *)element->children->content;
	return String();
}

String XmlElement_lxml2::getAttribute(const String &name)
{
	BString name8(name.utf8());
	for (xmlAttribute *a = element->attributes; a; a = (xmlAttribute *)a->next) {
		if (xmlStrEqual(a->name, (const xmlChar *)name8.toStr()) && a->children && a->children->type == XML_TEXT_NODE)
			return (const char *)a->children->content;
	}
	xthrownew(XmlException(String("attribute not found: ")+name));
	return String();
}


DRXML_NS_END
