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

#ifndef dr__gui__x11__Widget_x11__hxx__
# define dr__gui__x11__Widget_x11__hxx__

#include <dr/gui/dev/Widget_impl.hxx>
#include <dr/gui/x11/Gui_x11.hxx>

DR_GUI_NS_BEGIN


class Style;

class Widget_x11: public Widget_impl
{
public:
	DR_RINLINE			Widget_x11(Gui_x11 *impl_gui, Widget *owner, X11_Window impl_window);
	virtual				~Widget_x11();

public:
	virtual void			changeName(const String &name);
	virtual void			redraw(const Rect &rect);
	virtual Paint *			paint(Visual *visual);
	virtual void			show(bool show);
	virtual void			setFocus();

public:
	void				addExpose(const Rect &rect);

public:
	DR_RINLINE bool			isDummy();

protected:
	DR_RINLINE Gui_x11 *		accGuiImpl()				{ return (Gui_x11 *)gui_impl; }
	DR_RINLINE Display *		accDisplay()				{ return ((Gui_x11 *)gui_impl)->display; }

public:
	X11_Window			impl_window;

	Widget_x11			*expose_prev, *expose_next;
	Rect				expose_rect;
};


DR_RINLINE Widget_x11::Widget_x11(Gui_x11 *impl_gui_, Widget *owner_, Window impl_window_):
	Widget_impl(impl_gui_, owner_),
	impl_window(impl_window_),
	expose_rect(None())
{
	expose_prev = (Widget_x11 *)4;
	expose_next = (Widget_x11 *)4;
}

DR_RINLINE bool Widget_x11::isDummy()
{
	return impl_window == X11_None;
}


DR_GUI_NS_END

#endif
