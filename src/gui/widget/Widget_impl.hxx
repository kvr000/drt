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

#ifndef dr__gui__Widget_impl__hxx__
# define dr__gui__Widget_impl__hxx__

#include <dr/gui/dev/Gui_impl.hxx>
#include <dr/gui/Widget.hxx>

DR_GUI_NS_BEGIN


class Style;

class DR_GUI_PUB Widget_impl: public Base
{
public:
	DR_RINLINE			Widget_impl(Gui_impl *gui_impl_, Widget *owner);
	DR_RINLINE virtual		~Widget_impl()				{ owner->main_impl = NULL; }

	virtual void			changeName(const String &name) = 0;
	virtual void			redraw(const Rect &rect) = 0;
	virtual Paint *			paint(Visual *visual) = 0;
	virtual void			show(bool show) = 0;
	virtual void			setFocus() = 0;

	DR_RINLINE Gui *		accGui()				{ return gui_impl->accGui(); }

public:
	Gui_impl *			gui_impl;
	Widget *			owner;
};


DR_RINLINE Widget_impl::Widget_impl(Gui_impl *gui_impl_, Widget *owner_):
	gui_impl(gui_impl_),
	owner(owner_)
{
	owner->main_impl = this;
}


DR_GUI_NS_END

#endif
