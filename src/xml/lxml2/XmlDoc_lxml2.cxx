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

#include <dr/xml/XmlException.hxx>
#include <dr/xml/XmlNode_lxml2.hxx>
#include <dr/xml/XmlElement_lxml2.hxx>

#include <dr/xml/XmlDoc_lxml2.hxx>

DRXML_NS_BEGIN


DR_OBJECT_DEF(DRXML_NS_STR, XmlDoc_lxml2, XmlDoc);
DR_OBJECT_IMPL_SIMPLE(XmlDoc_lxml2);


XmlDoc_lxml2::XmlDoc_lxml2():
	doc(NULL),
	xpath(NULL)
{
}

XmlDoc_lxml2 *XmlDoc_lxml2::fromFile(const String &file)
{
	ERef<XmlDoc_lxml2> this_(new XmlDoc_lxml2());
	if (!(this_->doc = xmlParseFile(file.utf8().toStr()))) {
		xthrownew(XmlException("parse"));
	}
	if (!(this_->xpath = xmlXPathNewContext(this_->doc))) {
		xthrownew(XmlException("xpath"));
	}
	return this_.getAndNull();
}

XmlDoc_lxml2 *XmlDoc_lxml2::fromString(const String &str)
{
	ERef<XmlDoc_lxml2> this_(new XmlDoc_lxml2());
	if (!(this_->doc = xmlParseDoc((const xmlChar *)str.utf8().toStr()))) {
		xthrownew(XmlException("parse"));
	}
	if (!(this_->xpath = xmlXPathNewContext(this_->doc))) {
		xthrownew(XmlException("xpath"));
	}
	return this_.getAndNull();
}

XmlDoc_lxml2 *XmlDoc_lxml2::fromBlob(const Blob &str)
{
	ERef<XmlDoc_lxml2> this_(new XmlDoc_lxml2());
	if (!(this_->doc = xmlParseDoc((const xmlChar *)str.toStr()))) {
		xthrownew(XmlException("parse"));
	}
	if (!(this_->xpath = xmlXPathNewContext(this_->doc))) {
		xthrownew(XmlException("xpath"));
	}
	return this_.getAndNull();
}

size_t XmlDoc_lxml2::multiDocFromFile(RArray<XmlDoc> *docs, const String &path)
{
	Blob buffer;
	ERef<dr::io::Handle> file(new dr::io::File(path, dr::io::File::M_READ));
	ssize_t basep = 1;
	for (;;) {
		goto check_xml;
		for (;;) {
			if (file->read(&buffer, 4096, buffer.getSize()) <= 0) {
				basep = buffer.getSize();
				break;
			}
			const char *n;
			const char *p;
check_xml:
			const char *b = buffer.toStr();
			const char *e = b+buffer.getSize();
			if ((n = strstr(b+basep, "<?xml")) == NULL) {
				continue;
			}
			if ((p = n+5) >= e)
				continue;
			if (!isspace(*p)) {
				basep = p-b;
				continue;
			}
			do {
				if (++p >= e)
					goto more_read;
			} while (isspace(*p));
			for (;;) {
				if (p >= e)
					goto more_read;
				if (!isalnum(*p))
					break;
				p++;
			}
			if (p+1 >= e)
				goto more_read;
			if (p[0] != '=' || p[1] != '\"') {
				basep = p-b;
			}
			basep = n-b;
			break;
more_read:
			;
		}
		if (basep == 0)
			break;
		docs->appendNoref(fromBlob(buffer.removeLeftGet(basep)));
		basep = 1;
	}
	return docs->count();
}

XmlDoc_lxml2::~XmlDoc_lxml2()
{
	if (xpath)
		xmlXPathFreeContext(xpath);
	if (doc)
		xmlFreeDoc(doc);
}

XmlNode *XmlDoc_lxml2::findPathNode(const String &path)
{
	xmlXPathObjectPtr pobj;
	if (!(pobj = xmlXPathEval((xmlChar *)path.utf8().toStr(), xpath))) {
		xthrownew(XmlException(String("xpath: ")+path));
	}
	if (pobj->type != XPATH_NODESET) {
		xmlXPathFreeObject(pobj);
		xthrownew(XmlException(String("xpath not nodeset: ")+path));
	}
	if (pobj->nodesetval->nodeNr == 0) {
		xmlXPathFreeObject(pobj);
		xthrownew(XmlException(String("xpath empty: ")+path));
	}
	xmlNodePtr n = pobj->nodesetval->nodeTab[0];
	xmlXPathFreeObject(pobj);
	return new XmlNode_lxml2(this, n);
}

XmlElement *XmlDoc_lxml2::findPathElement(const String &path)
{
	xmlXPathObjectPtr pobj;
	if (!(pobj = xmlXPathEval((xmlChar *)path.utf8().toStr(), xpath))) {
		xthrownew(XmlException(String("xpath: ")+path));
	}
	if (pobj->type != XPATH_NODESET) {
		xmlXPathFreeObject(pobj);
		xthrownew(XmlException(String("xpath not nodeset: ")+path));
	}
	if (pobj->nodesetval->nodeNr == 0) {
		xmlXPathFreeObject(pobj);
		xthrownew(XmlException(String("xpath empty: ")+path));
	}
	xmlNodePtr n = pobj->nodesetval->nodeTab[0];
	xmlXPathFreeObject(pobj);
	if (n->type != XML_ELEMENT_NODE) {
		xthrownew(XmlException(String("xpath not element: ")+path));
	}
	return new XmlElement_lxml2(this, (xmlElementPtr)n);
}

size_t XmlDoc_lxml2::execPathElements(const String &path, const Eslot1<void, XmlElement *> &exec)
{
	size_t i;
	xmlXPathObjectPtr pobj;
	if (!(pobj = xmlXPathEval((xmlChar *)path.utf8().toStr(), xpath))) {
		xthrownew(XmlException(String("xpath: ")+path));
	}
	if (pobj->type != XPATH_NODESET) {
		xmlXPathFreeObject(pobj);
		xthrownew(XmlException(String("xpath not nodeset: ")+path));
	}
	for (i = 0; i < (size_t)pobj->nodesetval->nodeNr; i++) {
		xtry {
			xmlNodePtr n = pobj->nodesetval->nodeTab[i];
			if (n->type != XML_ELEMENT_NODE)
				continue;
			ERef<XmlElement_lxml2> el(new XmlElement_lxml2(this, (xmlElementPtr)n));
			exec(el);
		}
		xcatchany {
			xmlXPathFreeObject(pobj);
			xrethrowany;
		}
		xend;
	}
	xmlXPathFreeObject(pobj);
	return i;
}


DRXML_NS_END
