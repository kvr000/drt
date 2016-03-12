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

#ifndef dr__gui__Paint__hxx__
# define dr__gui__Paint__hxx__

#include <dr/Object.hxx>
#include <dr/String.hxx>

#include <dr/gui/struct.hxx>

DR_GUI_NS_BEGIN


class Gui_impl;
class Font;
class Font_impl;

class DR_GUI_PUB Paint: public Object
{
	DR_OBJECT_DECL_SIMPLE(Paint, Object);

public:
	Rect				clip;

protected:
	Gui_impl *			gui;

protected:
	DR_RINLINE			Paint(Gui_impl *gui_)		: clip(None()), gui(gui_) {}
	DR_RINLINE			Paint(Gui_impl *gui_, const Rect &clip_): clip(clip_), gui(gui_) {}
	DR_RINLINE			~Paint()				{}

public:
	virtual void			setForeground(GuiColor color) = 0;
	virtual void			setBackground(GuiColor color) = 0;

	virtual void			setLineStyle(int style) = 0;

	virtual void			setFont(Font *font) = 0;
	virtual void			setFont(Font_impl *font) = 0;

	virtual Font_impl *		getFont() = 0;
	virtual int			getFontAscent() = 0;
	virtual int			getFontDescent() = 0;
	virtual int			getFontHeight() = 0;
	virtual void			getStringSize(Size *size, const String &str) = 0;

public:
	virtual void			fillRectangle(const Rect &rect) = 0;
	virtual void			drawRectangle(const Rect &rect) = 0;
	virtual void			fillClip() = 0;

	virtual void			drawLine(const Point &p0, const Point &p1) = 0;
	virtual void			drawLines(const Point *plist, int count) = 0;

	virtual void			drawString(const Point &pos, const String &str) = 0;
};


DR_GUI_NS_END

#endif
