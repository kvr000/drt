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

#include <dr/testenv/testenv.hxx>
#include <dr/Assert.hxx>
#include <dr/MethodConv.hxx>

DR_NS_USE
DR_TESTENV_NS_USE


class C0;
class C1;
class C2;
class C3;
class C4;
class C5;
class C6;
class C7;

class O
{
public:
	C7 *				pmeth0(void)				{ return NULL; }
	virtual C7 *			vmeth0(void)				{ return NULL; }
	C6 *				pmeth1(C0*)				{ return NULL; }
	virtual C6 *			vmeth1(C0*)				{ return NULL; }
	C5 *				pmeth2(C0*,C1*)				{ return NULL; }
	virtual C5 *			vmeth2(C0*,C1*)				{ return NULL; }
	C4 *				pmeth3(C0*,C1*,C2*)			{ return NULL; }
	virtual C4 *			vmeth3(C0*,C1*,C2*)			{ return NULL; }
	C3 *				pmeth4(C0*,C1*,C2*,C3*)			{ return NULL; }
	virtual C3 *			vmeth4(C0*,C1*,C2*,C3*)			{ return NULL; }
	C2 *				pmeth5(C0*,C1*,C2*,C3*,C4*)		{ return NULL; }
	virtual C2 *			vmeth5(C0*,C1*,C2*,C3*,C4*)		{ return NULL; }
	C1 *				pmeth6(C0*,C1*,C2*,C3*,C4*,C5*)		{ return NULL; }
	virtual C1 *			vmeth6(C0*,C1*,C2*,C3*,C4*,C5*)		{ return NULL; }
	C0 *				pmeth7(C0*,C1*,C2*,C3*,C4*,C5*,C6*)	{ return NULL; }
	virtual C0 *			vmeth7(C0*,C1*,C2*,C3*,C4*,C5*,C6*)	{ return NULL; }
	C1 *				pmeth8(C0*,C1*,C2*,C3*,C4*,C5*,C6*,C7*)	{ return NULL; }
	virtual C1 *			vmeth8(C0*,C1*,C2*,C3*,C4*,C5*,C6*,C7*)	{ return NULL; }

public:
	virtual				~O()					{}
};


int main(void)
{
	test_init();
	DR_Assert(MethodConv::funcToMptr(MethodConv::methodToFptr(&O::pmeth0)) == &O::pmeth0);
	DR_Assert(MethodConv::funcToMptr(MethodConv::methodToFptr(&O::vmeth0)) == &O::vmeth0);
	DR_Assert(MethodConv::funcToMptr(MethodConv::methodToFptr(&O::pmeth1)) == &O::pmeth1);
	DR_Assert(MethodConv::funcToMptr(MethodConv::methodToFptr(&O::vmeth1)) == &O::vmeth1);
	DR_Assert(MethodConv::funcToMptr(MethodConv::methodToFptr(&O::pmeth2)) == &O::pmeth2);
	DR_Assert(MethodConv::funcToMptr(MethodConv::methodToFptr(&O::vmeth2)) == &O::vmeth2);
	DR_Assert(MethodConv::funcToMptr(MethodConv::methodToFptr(&O::pmeth3)) == &O::pmeth3);
	DR_Assert(MethodConv::funcToMptr(MethodConv::methodToFptr(&O::vmeth3)) == &O::vmeth3);
	DR_Assert(MethodConv::funcToMptr(MethodConv::methodToFptr(&O::pmeth4)) == &O::pmeth4);
	DR_Assert(MethodConv::funcToMptr(MethodConv::methodToFptr(&O::vmeth4)) == &O::vmeth4);
	DR_Assert(MethodConv::funcToMptr(MethodConv::methodToFptr(&O::pmeth5)) == &O::pmeth5);
	DR_Assert(MethodConv::funcToMptr(MethodConv::methodToFptr(&O::vmeth5)) == &O::vmeth5);
	DR_Assert(MethodConv::funcToMptr(MethodConv::methodToFptr(&O::pmeth6)) == &O::pmeth6);
	DR_Assert(MethodConv::funcToMptr(MethodConv::methodToFptr(&O::vmeth6)) == &O::vmeth6);
	DR_Assert(MethodConv::funcToMptr(MethodConv::methodToFptr(&O::pmeth7)) == &O::pmeth7);
	DR_Assert(MethodConv::funcToMptr(MethodConv::methodToFptr(&O::vmeth7)) == &O::vmeth7);
	DR_Assert(MethodConv::funcToMptr(MethodConv::methodToFptr(&O::pmeth8)) == &O::pmeth8);
	DR_Assert(MethodConv::funcToMptr(MethodConv::methodToFptr(&O::vmeth8)) == &O::vmeth8);
	Fatal::plog("p: %p\n", MethodConv::methodToFptr(&O::pmeth0));
	Fatal::plog("v: %p\n", MethodConv::methodToFptr(&O::vmeth0));
	Fatal::plog("p: %p\n", MethodConv::methodToFptr(&O::pmeth1));
	Fatal::plog("v: %p\n", MethodConv::methodToFptr(&O::vmeth1));
	return 0;
}
