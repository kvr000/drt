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

#include <dr/Const.hxx>
#include <dr/Assert.hxx>

#include <dr/gui/Visual.hxx>

DR_GUI_NS_BEGIN


DR_OBJECT_DEF(DR_GUI_NS_STR, Visual, Object);
DR_OBJECT_IMPL_SIMPLE(Visual);

GuiPixel Visual::mapColor_Mono(Visual *visual, GuiColor color)
{
	DR_AssertTodo();
	return 0;
}

GuiPixel Visual::mapColor_Gray(Visual *visual, GuiColor color)
{
	DR_AssertTodo();
	return 0;
}

GuiPixel Visual::mapColor_Index(Visual *visual, GuiColor color)
{
	DR_AssertTodo();
	return 0;
}

GuiPixel Visual::mapColor_True(Visual *visual, GuiColor color)
{
	return ((color>>visual->r_cmove)&visual->r_mask)|((color>>visual->g_cmove)&visual->g_mask)|((color>>visual->b_cmove)&visual->b_mask);
}

GuiPixel Visual::mapColor_True24RGB(Visual *visual, GuiColor color)
{
	return color;
}

GuiPixel Visual::mapColor_True24BGR(Visual *visual, GuiColor color)
{
	return ((color>>16)&0x0000ff)|((color<<16)&0xff0000)|((color&0x00ff00));
}

#if !(defined DR_OS_UNIX) && !(defined ffs)
namespace {
int ffs(unsigned value)
{
	int b;
	if (!value)
		return 0;
	for (b = 1; (value&1) == 0; b++, value >>= 1);
	return b;
}

};
#endif

void Visual::setup_truecolor()
{
	r_offs = ffs(r_mask)-1;
	g_offs = ffs(g_mask)-1;
	b_offs = ffs(b_mask)-1;

	r_size = ffs((r_mask>>r_offs)+1)-1;
	g_size = ffs((g_mask>>g_offs)+1)-1;
	b_size = ffs((b_mask>>b_offs)+1)-1;

	r_m8size = 8-r_size;
	g_m8size = 8-g_size;
	b_m8size = 8-b_size;

	r_cmove = 24-r_offs-r_size;
	g_cmove = 16-g_offs-g_size;
	b_cmove = 8-b_offs-b_size;

	p_mapColor = &mapColor_True;

	if (r_size == 8 && g_size == 8 && b_size == 8) {
		if (r_offs == 0 && g_offs == 8 && b_offs == 16)
			p_mapColor = &mapColor_True24BGR;
		else if (r_offs == 16 && g_offs == 8 && b_offs == 0)
			p_mapColor = &mapColor_True24RGB;
	}
}


DR_GUI_NS_END
