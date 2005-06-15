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

#include <stdio.h>
#include <string.h>

#include <dr/x_kw.hxx>
#include <dr/pe_error.hxx>

#include <dr/Ref.hxx>
#include <dr/Exception.hxx>

#include <dr/io/File.hxx>

#include <dr/xml/XmlDoc.hxx>
#include <dr/xml/XmlElement.hxx>
#include <dr/xml/XmlSubsystem.hxx>

#include <dr/tenv/tenv.hxx>

DR_XML_NS_USE;
DR_TENV_NS_USE


#define TEST_XML
#define TEST_MULTIXML

#ifdef TEST_XML
TENV_NS(xml);

static void handleEla(XmlElement *el)
{
	Fatal::plog("handling Ela: %s=%s\n", el->getAttribute("n").utf8().toStr(), el->getAttribute("v").utf8().toStr());
}

void test()
{
	xtry {
		ERef<XmlSubsystem> xmlss(XmlSubsystem::getImpl(Null()));
		ERef<XmlDoc> doc(xmlss->createDocFromString(
					"<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>"
					"<Data>"
					"	<Elc>Cont0</Elc>"
					"	<Ela n=\"xx\" v=\"0\"></Ela>"
					"	<Ela n=\"yy\" v=\"1\"></Ela>"
					"	<Ela n=\"zz\" v=\"3\"></Ela>"
					"</Data>"
					));
		ERef<XmlElement> elr(doc->findPathElement("/Data"));
		ERef<XmlElement> elc(doc->findPathElement("/Data/Elc"));
		Fatal::plog("Found Elc: %s\n", elc->getTexts().utf8().toStr());
		ERef<XmlElement> ela(doc->findPathElement("/Data/Ela[@n=\"zz\"]"));
		Fatal::plog("Found Ela[n=zz]: %s\n", ela->getAttribute("v").utf8().toStr());
		doc->execPathElements("/Data/Ela", &handleEla);
	}
	xcatch (Exception, ex) {
		Fatal::plog("caught exception: %s\n", ex->stringify().utf8().toStr());
	}
	xend;
}
TENV_NSE(xml);
#endif


#ifdef TEST_MULTIXML
TENV_NS(multixml);

void test()
{
	xtry {
		ERef<dr::io::File> tf(new dr::io::File("testfile.xml", dr::io::File::M_WRITE|dr::io::File::M_CREATE|dr::io::File::M_TRUNC));
		tf->write(Blob(
					"<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>"
					"<Data>"
					"	<El>Cont0</El>"
					"</Data>"
					"<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>"
					"<Data>"
					"	<El>Cont1</El>"
					"</Data>"
			      ));
		tf.setNull();
		ERef<XmlSubsystem> xmlss(XmlSubsystem::getImpl(Null()));
		RArray<XmlDoc> docs;
		TENV_CHECK(xmlss->createMultiDocFromFile(&docs, "testfile.xml") == 2);
		TENV_CHECK(tref(docs[0]->findPathElement("/Data/El"))->getTexts() == "Cont0");
		TENV_CHECK(tref(docs[1]->findPathElement("/Data/El"))->getTexts() == "Cont1");
		dr::io::File::unlink("testfile.xml");
	}
	xcatch (Exception, ex) {
		Fatal::plog("caught exception: %s\n", ex->stringify().utf8().toStr());
	}
	xend;
}
TENV_NSE(multixml);
#endif

DR_TENV_MAIN()
{
#ifdef TEST_XML
	TENV_RUN(xml);
#endif
#ifdef TEST_MULTIXML
	TENV_RUN(multixml);
#endif
	return 0;
}
