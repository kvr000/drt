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

#include <dr/gui/wnt/Font_wnt.hxx>

DR_GUI_NS_BEGIN


Font_wnt::Font_wnt(Gui_wnt *gui_, HFONT font_handle_):
	gui(gui_),
	font_handle(font_handle_)
{
	/* TODO */
	ascent = 8; //font_str->ascent;
	descent = 8; //font_str->descent;
	direction = 0; //font_str->direction;
}

Font_wnt::~Font_wnt()
{
	DeleteObject(font_handle);
}


Font_impl *Gui_wnt::createFont(Font *base)
{
	/* TODO ;) */
	return NULL;
}

Font_impl *Gui_wnt::defaultFont(Font *base)
{
	return default_font.getDoref();
}


DR_GUI_NS_END
