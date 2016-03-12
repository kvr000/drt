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

#ifndef dr__gui__wnt__Paint_wnt__hxx__
# define dr__gui__wnt__Paint_wnt__hxx__

#include <dr/gui/Paint.hxx>
#include <dr/gui/wnt/Gui_wnt.hxx>
#include <dr/gui/wnt/Font_wnt.hxx>

DR_GUI_NS_BEGIN


class Widget_wnt;
class Visual_wnt;


class Paint_wnt: public Paint
{
public:
	DR_RINLINE			Paint_wnt(Gui_wnt *gui_, Widget_wnt *widget_, Visual_wnt *visual_);

protected:
	virtual				~Paint_wnt();

public:
	bool				initContext();

public:
	virtual void			setForeground(GuiColor color);
	virtual void			setBackground(GuiColor color);

	virtual void			setLineStyle(int line_style);

	virtual void			setFont(Font *font);
	virtual void			setFont(Font_impl *font);

	virtual Font_impl *		getFont();
	virtual int			getFontAscent();
	virtual int			getFontDescent();
	virtual int			getFontHeight();
	virtual void			getStringSize(Size *size, const String &str);

public:
	virtual void			drawRectangle(const Rect &rect);
	virtual void			fillRectangle(const Rect &rect);
	virtual void			fillClip();

	virtual void			drawLine(const Point &p0, const Point &p1);
	virtual void			drawLines(const Point *plist, int count);

	virtual void			drawString(const Point &pos, const String &str);

public:
	DR_RINLINE static DWORD		colorToWnt(GuiColor color);

public:
	DR_RINLINE Gui_wnt *		getGui()				{ return (Gui_wnt *)gui; }

protected:
public:
	Widget_wnt *			widget;
	Visual_wnt *			visual;
	Ref<Font_wnt>			font;
	HDC				gc;
	PAINTSTRUCT			ps;

	DWORD				foreground_color;
	DWORD				background_color;
	int				line_style;
};


DR_RINLINE Paint_wnt::Paint_wnt(Gui_wnt *gui_, Widget_wnt *widget_, Visual_wnt *visual_):
	Paint(gui_, None()),
	widget(widget_),
	visual(visual_),
	gc(NULL),
	foreground_color(0),
	background_color(0xc0c0c0),
	line_style(0)
{
	font.setDoref(getGui()->default_font);
}

DR_RINLINE DWORD Paint_wnt::colorToWnt(GuiColor color)
{
	return (color>>16)|(color&0x00ff00)|((Uint8)color<<16);
}


DR_GUI_NS_END

#endif
