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

#ifndef dr__xml__XmlNode_lxml2__hxx__
# define dr__xml__XmlNode_lxml2__hxx__

#include <libxml/parser.h>
#include <libxml/xmlreader.h>
#include <libxml/xpath.h>
#include <libxml/tree.h>

#include <dr/Ref.hxx>

#include <dr/xml/def_xml.hxx>

#include <dr/xml/XmlDoc_lxml2.hxx>

#include <dr/xml/XmlNode.hxx>

DRXML_NS_BEGIN


/**
 * Base for all IO handles
 */
class DR_XML_PUB XmlNode_lxml2: public XmlNode
{
	DR_OBJECT_DECL_SIMPLE(XmlNode_lxml2, XmlNode);

public:
	DR_CONSTRUCT			XmlNode_lxml2(XmlDoc_lxml2 *owner, xmlNodePtr node);

protected:
	virtual				~XmlNode_lxml2();

protected:
	Ref<XmlDoc_lxml2>		owner;
	xmlNodePtr			node;
};


DRXML_NS_END

#endif
