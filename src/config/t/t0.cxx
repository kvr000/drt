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

#include <dr/tenv/tenv.hxx>

DR_CONFIG_NS_USE;
DR_TENV_NS_USE


#define TEST_ROWASSIGN
#define TEST_INI

#ifdef TEST_ROWASSIGN
TENV_NS(rowassign);
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
TENV_NSE(rowassign);
#endif

#ifdef TEST_INI
TENV_NS(ini);
void test()
{
	xtry {
		ERef<IniConfig> ini(new IniConfig("testconf.ini"));
		TENV_CHECK(ini->getValue("", "invalid", Null()) == String(Null()));
		TENV_CHECK(ini->getValue("", "gl0") == "glob0");
		TENV_CHECK(ini->getValue("main/sub", "gl1") == "glob1");
		TENV_CHECK(ini->getValue("sub", "s1") == "sub");
		TENV_CHECK(ini->getValue("main/sub", "s1") == "main");
		TENV_CHECK(ini->getValue("main/sub", "s0") == "sub");
		TENV_CHECK(tref(ini->getSection("main/sub"))->getValue("s0") == "sub");
	}
	xcatch (Exception, ex) {
		printf("caught exception: %s\n", ex->stringify().utf8().toStr());
		TENV_CHECK(0);
	}
	xend;
}
TENV_NSE(ini);
#endif

DR_TENV_MAIN()
{
	tenv_init();
#ifdef TEST_ROWASSIGN
	TENV_RUN(rowassign);
#endif
#ifdef TEST_INI
	TENV_RUN(ini);
#endif
	return 0;
}
