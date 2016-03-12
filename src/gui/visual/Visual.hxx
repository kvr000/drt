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

#ifndef dr__gui__Visual__hxx__
# define dr__gui__Visual__hxx__

#include <dr/Object.hxx>

#include <dr/gui/struct.hxx>

DR_GUI_NS_BEGIN


class Gui_impl;

class DR_GUI_PUB Visual: public Object
{
	DR_OBJECT_DECL_SIMPLE(Visual, Object);

public:
	enum VisualType
	{
		VT_Mono,
		VT_Gray,
		VT_Fixed,
		VT_Index,
		VT_True,

		VT_GL			= 0x10,
	};

	VisualType			type;

	int				depth;

	/* masks */
	GuiPixel			r_mask;
	GuiPixel			g_mask;
	GuiPixel			b_mask;

	/* offsets */
	int				r_offs;
	int				g_offs;
	int				b_offs;

	/* depth */
	int				r_size;
	int				g_size;
	int				b_size;

	/* 8-size, for internal use */
	int				r_m8size;
	int				g_m8size;
	int				b_m8size;

	/* rgb_offset+r_m8size, for internal use */
	int				r_cmove;
	int				g_cmove;
	int				b_cmove;

protected:
	DR_RINLINE			Visual(Gui_impl *gui_)			: gui(gui_) {}
	DR_RINLINE			~Visual()				{}

public:
	DR_RINLINE GuiPixel		mapRGB(unsigned r, unsigned g, unsigned b);
	DR_RINLINE GuiPixel		mapColor(GuiColor color);

protected:
	DR_GUI_MET void			setup_truecolor();

protected:
	GuiPixel			(*p_mapColor)(Visual *visual, GuiColor color);

protected:
	Gui_impl *			gui;

protected:
	DR_GUI_MTS static GuiPixel	mapColor_Mono(Visual *visual, GuiColor color);
	DR_GUI_MTS static GuiPixel	mapColor_Gray(Visual *visual, GuiColor color);
	DR_GUI_MTS static GuiPixel	mapColor_Index(Visual *visual, GuiColor color);
	DR_GUI_MTS static GuiPixel	mapColor_True(Visual *visual, GuiColor color);
	DR_GUI_MTS static GuiPixel	mapColor_True24RGB(Visual *visual, GuiColor color);
	DR_GUI_MTS static GuiPixel	mapColor_True24BGR(Visual *visual, GuiColor color);
};


DR_RINLINE GuiPixel Visual::mapRGB(unsigned r, unsigned g, unsigned b)
{
	return p_mapColor(this, drRGB(r, g, b));
}

DR_RINLINE GuiPixel Visual::mapColor(GuiColor color)
{
	return p_mapColor(this, color);
}


DR_GUI_NS_END

#endif
