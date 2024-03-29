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
 * include:	dr/xml/XmlSubsystem.hxx
 *
 * ns:		dr::xml
 */


#include <dr/io/File.hxx>

#include <dr/xml/XmlDoc_lxml2.hxx>

#include "_gen/XmlSubsystem_lxml2-all.hxx"

DR_XML_NS_BEGIN


/*drt
 * class:	XmlSubsystem_lxml2
 * ancestor:	XmlSubsystem
 *
 * doc:{
 * 	Xml Subsystem - lxm2 implementation
 * }doc
 */

DR_MET(public virtual)
XmlDoc *XmlSubsystem_lxml2::createDocFromFile(const String &file)
{
	return XmlDoc_lxml2::fromFile(file);
}

DR_MET(public virtual)
XmlDoc *XmlSubsystem_lxml2::createDocFromString(const String &str)
{
	return XmlDoc_lxml2::fromString(str);
}

DR_MET(public virtual)
XmlDoc *XmlSubsystem_lxml2::createDocFromBlob(const Blob &str)
{
	return XmlDoc_lxml2::fromBlob(str);
}

DR_MET(public virtual)
size_t XmlSubsystem_lxml2::createMultiDocFromFile(RArray<XmlDoc> *docs, const String &path)
{
	return XmlDoc_lxml2::multiDocFromFile(docs, path);
}


DR_XML_NS_END
