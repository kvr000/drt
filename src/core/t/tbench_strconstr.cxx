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

#include <dr/Const.hxx>
#include <dr/String.hxx>

#include <dr/tenv/tenv.hxx>

DR_NS_USE
DR_TENV_NS_USE


TENV_NS(strconstr);

static Sint64 i = 0;
void run_strconstr()
{
	BString(BString::createNumber(i++));
}

void test()
{
	printf("strconstr: %g\n", tenv_bench(&run_strconstr, 2000000)/1000000.0);
}

TENV_NSE(strconstr);

int main()
{
	tenv_init();
	TENV_RUN(strconstr);
	return 0;
}
