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

#ifndef dr__xml__XmlSubsystem_lxml2__hxx__
# define dr__xml__XmlSubsystem_lxml2__hxx__

#include <dr/emits.hxx>

#include <dr/xml/def_xml.hxx>

#include <dr/xml/XmlSubsystem.hxx>

DRXML_NS_BEGIN


/**
 * Base for all XML implementations
 */
class DR_XML_PUB XmlSubsystem_lxml2: public XmlSubsystem
{
	DR_OBJECT_DECL_SIMPLE(XmlSubsystem_lxml2, XmlSubsystem);

public:
	DR_CONSTRUCT			XmlSubsystem_lxml2()			{}

public:
	virtual XmlDoc *		createDocFromFile(const String &path);
	virtual size_t			createMultiDocFromFile(RArray<XmlDoc> *docs, const String &path);
	virtual XmlDoc *		createDocFromString(const String &str);
	virtual XmlDoc *		createDocFromBlob(const Blob &content);
};


DRXML_NS_END

#endif
