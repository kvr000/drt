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

#ifndef dr__xml__XmlDoc_lxml2__hxx__
# define dr__xml__XmlDoc_lxml2__hxx__

#include <libxml/parser.h>
#include <libxml/xmlreader.h>
#include <libxml/xpath.h>

#include <dr/Blob.hxx>
#include <dr/Array.hxx>
#include <dr/Ref.hxx>

#include <dr/xml/def_xml.hxx>

#include <dr/xml/XmlDoc.hxx>

DR_XML_NS_BEGIN


/**
 * Xml Document
 */
class DR_XML_PUB XmlDoc_lxml2: public XmlDoc
{
	DR_OBJECT_DECL_SIMPLE(XmlDoc_lxml2, XmlDoc);

public:
	DR_CONSTRUCT			XmlDoc_lxml2();


	static XmlDoc_lxml2 *		fromFile(const String &file);
	static XmlDoc_lxml2 *		fromString(const String &str);
	static XmlDoc_lxml2 *		fromBlob(const Blob &str);
	static size_t			multiDocFromFile(RArray<XmlDoc> *docs, const String &path);

protected:
	DR_CONSTRUCT			~XmlDoc_lxml2();

public:
	virtual XmlNode *		findPathNode(const String &path);
	virtual XmlElement *		findPathElement(const String &path);
	virtual size_t			execPathElements(const String &path, const Eslot1<void, XmlElement *> &exec);

protected:
	xmlDocPtr			doc;
	xmlXPathContextPtr		xpath;
};


DR_XML_NS_END

#endif
