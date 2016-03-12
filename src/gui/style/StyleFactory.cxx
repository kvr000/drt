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

#include <dr/gui/Widget.hxx>
#include <dr/gui/Font.hxx>

#include <dr/gui/StyleFactory.hxx>

DR_GUI_NS_BEGIN


DR_OBJECT_DEF(DR_GUI_NS_STR, StyleFactory, Object);
DR_OBJECT_IMPL_SIMPLE(StyleFactory);


DR_EXPORT_MET StyleFactory::StyleFactory(Gui *gui_):
	gui(gui_)
{
	clr_background = drRGB(144, 144, 160);
	clr_light_edge = drRGB(224, 224, 240);
	clr_dark_edge = drRGB(64, 64, 96);
	clr_font = drRGB(0, 0, 0);
	clr_disabled = drRGB(64, 64, 64);
	clr_focus = drRGB(64, 64, 64);
}

DR_EXPORT_MET StyleFactory::~StyleFactory()
{
}


DR_GUI_NS_END
