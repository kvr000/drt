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

#include <dr/app/ApplicationCache.hxx>

#include <dr/testenv/testenv.hxx>
#include <dr/testenv/TestObject.hxx>

DR_APP_NS_USE;
DR_TESTENV_NS_USE


#define TEST_APPCACHE

#ifdef TEST_APPCACHE
TESTNS(appcache);

static void checkIds(ApplicationCache *ac, int first, int next)
{
	for (int i = 0; i < first; i++) {
		CHECK(tref(ac->findResource(tref(new TestObject(i)))).isNull());
	}
	for (int i = first; i < next; i++) {
		CHECK(!tref(ac->findResource(tref(new TestObject(i)))).isNull());
	}
}

void test()
{
	ERef<ApplicationCache> ac(new ApplicationCache);

	ac->addResource(1, tref(new TestObject(0)), tref(new TestObject(0)));
	ac->addResource(1, tref(new TestObject(1)), tref(new TestObject(1)));
	ac->addResource(2, tref(new TestObject(2)), tref(new TestObject(2)));
	ac->addResource(2, tref(new TestObject(3)), tref(new TestObject(3)));
	ac->addResource(4, tref(new TestObject(4)), tref(new TestObject(4)));

	checkIds(ac, 0, 5);
	CHECK(ac->expire(1) == 1);
	checkIds(ac, 2, 5);
	CHECK(ac->expire(1) == 1);
	checkIds(ac, 2, 5);
	CHECK(ac->expire(2) == 2);
	checkIds(ac, 4, 5);
	CHECK(ac->expire(4) == Time::INVAL_TIME);
	checkIds(ac, 5, 5);
}
TESTNSE(appcache);
#endif

DR_TESTENV_MAIN()
{
	test_init();
#ifdef TEST_APPCACHE
	TESTRUN(appcache);
#endif
	return 0;
}
