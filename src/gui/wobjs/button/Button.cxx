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

#include <dr/gui/Button.hxx>

DR_GUI_NS_BEGIN


DR_EXPORT_DTS const String Button::style_name(Const::string("style_button"));


DR_EXPORT_MET Button::Button(Widget *parent_):
	Widget(parent_)
{
}

DR_EXPORT_MET Button::Button(Gui *gui_):
	Widget(gui_)
{
}

DR_EXPORT_MET Button::~Button()
{
}

DR_EXPORT_MET String Button::getStyleName()
{
	return style_name;
}

DR_EXPORT_MET void Button::pressed()
{
	Fatal::plog("button %p pressed\n", this);
}


DR_GUI_NS_END
