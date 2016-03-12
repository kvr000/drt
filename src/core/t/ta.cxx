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

#include <dr/Alloc.hxx>
#include <dr/Shared.hxx>

#include <dr/tenv/tenv.hxx>

DR_NS_USE


Shared *alfunc(void)
{
	return new Shared;
}

int main(void)
{
	void *mems[2048];
	unsigned i;
	Shared *s;

	s = alfunc();

	for (i = 0; i < sizeof(mems)/sizeof(mems[0]); i++)
		mems[i] = Alloc::allocS(i);
	for (i = 0; i < sizeof(mems)/sizeof(mems[0]); i++)
		Alloc::freeS(mems[i], i);

	for (i = 0; i < sizeof(mems)/sizeof(mems[0]); i++)
		mems[i] = Alloc::allocS(i);
	for (i = sizeof(mems)/sizeof(mems[0]); i-- > 0; )
		Alloc::freeS(mems[i], i);

	s->unref();

	return 0;
}
