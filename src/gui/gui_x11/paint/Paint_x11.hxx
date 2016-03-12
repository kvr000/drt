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

#ifndef dr__gui__x11__Paint_x11__hxx__
# define dr__gui__x11__Paint_x11__hxx__

#include <X11/Xlib.h>

#include <dr/gui/x11/X11wrap/redef_x11.hxx>

#include <dr/gui/Paint.hxx>
#include <dr/gui/x11/Gui_x11.hxx>
#include <dr/gui/x11/Visual_x11.hxx>
#include <dr/gui/x11/Font_x11.hxx>

DR_GUI_NS_BEGIN


class Widget_x11;
class Visual_x11;


class Paint_x11: public Paint
{
public:
	Widget_x11 *			widget;
	Ref<Visual_x11>			visual;
	Ref<Font_x11>			font;
	GC				gc;

public:
	DR_RINLINE			Paint_x11(Gui_x11 *gui_, const Rect &clip_, Widget_x11 *widget_, Visual_x11 *visual_, GC gc_);

protected:
	virtual				~Paint_x11();

public:
	virtual void			setForeground(GuiColor color);
	virtual void			setBackground(GuiColor color);

	virtual void			setLineStyle(int style);

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
	DR_RINLINE Gui_x11 *		accGui()				{ return (Gui_x11 *)gui; }
	DR_RINLINE Display *		accDisplay()				{ return ((Gui_x11 *)gui)->display; }
};


DR_RINLINE Paint_x11::Paint_x11(Gui_x11 *gui_, const Rect &clip_, Widget_x11 *widget_, Visual_x11 *visual_, GC gc_):
	Paint(gui_, clip_),
	widget(widget_),
	visual(visual_, true),
	font(accGui()->def_font, true),
	gc(gc_)
{
}


DR_GUI_NS_END

#endif
