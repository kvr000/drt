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
#include <unistd.h>
#include <signal.h>

#include <dr/Array.hxx>
#include <dr/List.hxx>
#include <dr/Hash.hxx>

#include <dr/testenv/testenv.hxx>
#include <dr/testenv/TestObject.hxx>

DR_NS_USE
DR_TESTENV_NS_USE

#define TEST_ARRAY
#define TEST_LIST
#define TEST_HASH

#ifdef TEST_ARRAY
TESTNS(array);
void test()
{
	{
		RArray<TestObject> ar;

		ar.appendNoref(new TestObject(4));
		ar.appendDoref(tref(new TestObject(9)));
		ar.appendDoref(tref(new TestObject(7)));
		ar.shrinkEnd(2);
	}
	CHECK(TestObject::countLiving() == 0);
}
TESTNSE(array);
#endif

#ifdef TEST_LIST
TESTNS(list);
void test()
{
	{
		RList<TestObject> list;
		list.accAppendingNew()->setNoref(new TestObject(3));
		list.append(tref(new TestObject(5)));
	}
	CHECK(TestObject::countLiving() == 0);
}
TESTNSE(list);
#endif

#ifdef TEST_HASH
TESTNS(hash);
void test()
{
	{
		RHash<String, TestObject> hash;
		hash["bla"].setNoref(new TestObject(3));
		hash["sehy"].setDoref(tref(new TestObject(4)));
	}
	CHECK(TestObject::countLiving() == 0);
}
TESTNSE(hash);
#endif

int main()
{
	test_init();
#ifdef TEST_ARRAY
	TESTRUN(array);
#endif
#ifdef TEST_LIST
	TESTRUN(list);
#endif
#ifdef TEST_HASH
	TESTRUN(hash);
#endif
	return 0;
}
