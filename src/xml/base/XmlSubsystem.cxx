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
 * include: 	dr/Subsystem.hxx
 * include: 	dr/Array.hxx
 *
 * ns:		dr::xml
 *
 * forward:	class XmlDoc;
 */

#include <dr/xml/XmlSubsystem_lxml2.hxx>

#include "_gen/XmlSubsystem-all.hxx"

DR_XML_NS_BEGIN


/*drt
 * class:	XmlSubsystem
 * ancestor:	dr::Subsystem
 *
 * doc:{
 * 	Base for all XML implementations
 * }doc
 */

DR_MET(public inline)
//XmlSubsystem::XmlSubsystem()
//{}

DR_MET(public static)
XmlSubsystem *XmlSubsystem::getImpl(const String &impl)
{
	return new XmlSubsystem_lxml2();
}

DR_MET(public abstract)
//XmlDoc *XmlSubsystem::createDocFromFile(const String &path)

DR_MET(public abstract)
//size_t XmlSubsystem::createMultiDocFromFile(RArray<XmlDoc> *docs, const String &path)

DR_MET(public abstract)
//XmlDoc *XmlSubsystem::createDocFromString(const String &content)

DR_MET(public abstract)
//XmlDoc *XmlSubsystem::createDocFromBlob(const Blob &content)


DR_XML_NS_END
