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
#include <dr/ThreadSimple.hxx>

#include <dr/config/RowAssignConfig.hxx>
#include <dr/config/IniConfig.hxx>

#include <dr/testenv/testenv.hxx>

DR_CONFIG_NS_USE;
DR_TESTENV_NS_USE


#define TEST_ROWASSIGN
#define TEST_INI

#ifdef TEST_ROWASSIGN
TESTNS(rowassign);
void test()
{
	xtry {
		ERef<RowAssignConfig> rac(new RowAssignConfig("testconf.cfg"));
		printf("bla: %s\n", rac->getValue("bla").utf8().toStr());
		printf("kra: %s\n", rac->getValue("kra").utf8().toStr());
		printf("ble: %s\n", rac->getValue("ble").utf8().toStr());
	}
	xcatch (Exception, ex) {
		printf("caught exception: %s\n", ex->stringify().utf8().toStr());
	}
	xend;
}
TESTNSE(rowassign);
#endif

#ifdef TEST_INI
TESTNS(ini);
void test()
{
	xtry {
		ERef<IniConfig> ini(new IniConfig("testconf.ini"));
		CHECK(ini->getValue("", "invalid", Null()) == String(Null()));
		CHECK(ini->getValue("", "gl0") == "glob0");
		CHECK(ini->getValue("main/sub", "gl1") == "glob1");
		CHECK(ini->getValue("sub", "s1") == "sub");
		CHECK(ini->getValue("main/sub", "s1") == "main");
		CHECK(ini->getValue("main/sub", "s0") == "sub");
		CHECK(tref(ini->getSection("main/sub"))->getValue("s0") == "sub");
	}
	xcatch (Exception, ex) {
		printf("caught exception: %s\n", ex->stringify().utf8().toStr());
		CHECK(0);
	}
	xend;
}
TESTNSE(ini);
#endif

DR_TESTENV_MAIN()
{
	test_init();
#ifdef TEST_ROWASSIGN
	TESTRUN(rowassign);
#endif
#ifdef TEST_INI
	TESTRUN(ini);
#endif
	return 0;
}
