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
 * include: 	dr/xml/def_xml.hxx
 * include: 	dr/Object.hxx
 *
 * ns:		dr::xml
 */

#include "_gen/XmlElement-all.hxx"

DR_XML_NS_BEGIN


/*drt
 * class:	XmlElement
 * ancestor:	dr::Object
 *
 * doc:{
 * 	Base for all XML XmlElement implementations
 * }doc
 */

DR_MET(public inline)
//XmlElement::XmlElement()
//{}

DR_MET(public inline)
//XmlElement::~XmlElement()
//{}

DR_MET(public abstract)
//String XmlElement::getTexts()

DR_MET(public abstract)
//String XmlElement::getAttribute(const String &attr)

DR_MET(public abstract)
//String XmlElement::checkAttribute(const String &attr)

DR_MET(public abstract)
//XmlElement *XmlElement::checkSubElement(const String &name)


DR_XML_NS_END
