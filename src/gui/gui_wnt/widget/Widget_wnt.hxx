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

#ifndef dr__gui__wnt__Widget_wnt__hxx__
# define dr__gui__wnt__Widget_wnt__hxx__

#include <dr/gui/dev/Widget_impl.hxx>
#include <dr/gui/wnt/Gui_wnt.hxx>

DR_GUI_NS_BEGIN


class Style;

class Widget_wnt: public Widget_impl
{
public:
	DR_RINLINE			Widget_wnt(Gui_wnt *impl_gui, Widget *owner, HWND impl_window);
	virtual				~Widget_wnt();

public:
	virtual void			changeName(const String &name);
	virtual void			redraw(const Rect &rect);
	virtual Paint *			paint(Visual *visual);
	virtual void			show(bool show);
	virtual void			setFocus();

public:
	DR_RINLINE bool			isDummy();

protected:
	DR_RINLINE Gui_wnt *		accGuiImpl()				{ return (Gui_wnt *)gui_impl; }

public:
	HWND				impl_window;
	bool				mouse_is_in;
};


DR_RINLINE Widget_wnt::Widget_wnt(Gui_wnt *impl_gui_, Widget *owner_, HWND impl_window_):
	Widget_impl(impl_gui_, owner_),
	impl_window(impl_window_),
	mouse_is_in(false)
{
}

DR_RINLINE bool Widget_wnt::isDummy()
{
	return impl_window == NULL;
}


DR_GUI_NS_END

#endif
