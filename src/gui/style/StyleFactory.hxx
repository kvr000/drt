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

#ifndef dr__gui__StyleFactory__hxx__
# define dr__gui__StyleFactory__hxx__

#include <dr/gui/Gui.hxx>
#include <dr/gui/Font.hxx>
#include <dr/gui/struct.hxx>

DR_GUI_NS_BEGIN


class Gui;
class StyleWidget;
class Widget;
class Font;
class Paint;
class Control;


class DR_GUI_PUB StyleFactory: public Object
{
	DR_OBJECT_DECL_SIMPLE(StyleFactory, Object);

public:
	/* constructor */		StyleFactory(Gui *gui);

protected:
	virtual				~StyleFactory();

public:
	virtual Control *		createControl(const String &control_name, Widget *parent) = 0;

public:
	virtual void			drawFocusMark(Paint *painter, Control *control) = 0;

protected:
	Gui *				gui;

public:
	GuiColor			clr_background;
	GuiColor			clr_light_edge;
	GuiColor			clr_dark_edge;
	GuiColor			clr_font;
	GuiColor			clr_disabled;
	GuiColor			clr_focus;

	Ref<Font>			font_msg;
	Ref<Font>			font_typing;
	Ref<Font>			font_boxes;
};


DR_GUI_NS_END

#endif
