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

#include <dr/Ref.hxx>
#include <dr/Variant.hxx>

#include <dr/testenv/testenv.hxx>

DR_NS_USE
DR_TESTENV_NS_USE

#define TEST_VARIANT
#define TEST_EQ

#ifdef TEST_VARIANT
TESTNS(variant);
void test()
{
	ERef<Variant> var0(new Variant(false));
}
TESTNSE(variant);
#endif


#ifdef TEST_EQ
TESTNS(eq);
void test()
{
	CHECK(tref(new Variant(BString(Null())))->eq(tref(new Variant(BString(Null())))));
	CHECK(tref(new Variant(Null()))->eq(tref(new Variant(BString(Null())))));
	CHECK(tref(new Variant(BString(Null())))->eq(tref(new Variant(Null()))));
	CHECK(!tref(new Variant(Null()))->eq(tref(new Variant(BString("0")))));
	CHECK(!tref(new Variant(BString(Null())))->eq(tref(new Variant(BString("0")))));
	CHECK(!tref(new Variant(BString("")))->eq(tref(new Variant(BString("0")))));
	CHECK(tref(new Variant((Sint64)0))->eq(tref(new Variant(BString("0")))));
	CHECK(!tref(new Variant((Sint64)0))->eq(tref(new Variant(BString("1")))));
	CHECK(!tref(new Variant((Sint64)0))->eq(tref(new Variant(BString("")))));
	CHECK(!tref(new Variant(BString("")))->eq(tref(new Variant((Sint64)0))));
}
TESTNSE(eq);
#endif


int main()
{
	test_init();
#ifdef TEST_VARIANT
	TESTRUN(variant);
#endif
#ifdef TEST_EQ
	TESTRUN(eq);
#endif
	return 0;
}
