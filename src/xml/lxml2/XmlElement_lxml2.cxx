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
 * include:	libxml/parser.h
 * include:	libxml/xmlreader.h
 * include:	libxml/xpath.h
 * include:	libxml/tree.h
 * 
 * include:	dr/Ref.hxx
 * include:	dr/xml/XmlElement.hxx
 * include:	dr/xml/XmlDoc_lxml2.hxx
 *
 * ns:		dr::xml
 */

#include <dr/x_kw.hxx>

#include <dr/xml/XmlException.hxx>

#include <dr/xml/XmlElement_lxml2.hxx>
#include "_gen/XmlElement_lxml2-all.hxx"

DR_XML_NS_BEGIN


/*drt
 * class:	XmlElement_lxml2
 * ancestor:	XmlElement
 *
 * at:	Ref<XmlDoc_lxml2>		owner;
 * at:	xmlElementPtr			element;
 */


DR_MET(public)
XmlElement_lxml2::XmlElement_lxml2(XmlDoc_lxml2 *owner_, xmlElementPtr element_):
	owner(owner_, true),
	element(element_)
{
}

DR_MET(protected virtual)
XmlElement_lxml2::~XmlElement_lxml2()
{
}

DR_MET(public virtual)
String XmlElement_lxml2::getTexts()
{
	if (element->children && element->children->type == XML_TEXT_NODE)
		return (const char *)element->children->content;
	return String();
}

DR_MET(public virtual)
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

DR_MET(public virtual)
String XmlElement_lxml2::checkAttribute(const String &name)
{
	BString name8(name.utf8());
	for (xmlAttribute *a = element->attributes; a; a = (xmlAttribute *)a->next) {
		if (xmlStrEqual(a->name, (const xmlChar *)name8.toStr()) && a->children && a->children->type == XML_TEXT_NODE)
			return (const char *)a->children->content;
	}
	return Null();
}

DR_MET(public virtual)
XmlElement *XmlElement_lxml2::checkSubElement(const String &name)
{
	BString name8(name.utf8());
	for (xmlNodePtr child = element->children; child; child = child->next) {
		if (child->type == XML_ELEMENT_NODE && xmlStrEqual(child->name, (const xmlChar *)name8.toStr()))
			return new XmlElement_lxml2(owner, (xmlElementPtr)child);
	}
	return NULL;
}


DR_XML_NS_END
