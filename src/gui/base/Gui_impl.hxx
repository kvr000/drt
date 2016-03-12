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

#ifndef dr__gui__Gui_impl__hxx__
# define dr__gui__Gui_impl__hxx__

#include <dr/gui/Gui.hxx>
#include <dr/gui/struct.hxx>

DR_GUI_NS_BEGIN


class Widget_impl;
class Font_impl;
class Visual;
class StyleWidget;

class Font;


class DR_GUI_PUB Gui_impl: public Object
{
	DR_OBJECT_DECL_SIMPLE(Gui_impl, Object);

public:
	/** opens GUI */
	virtual void			open(Gui *gui) = 0;

	/** flushes gui */
	virtual void			flush();

	/** create widget data */
	struct widget_init_data
	{
		Rect				geometry;
		Widget_impl *			parent;
		Visual *			visual;
		/* constructor */		widget_init_data()			: geometry(None()) {}
	};
	/** create widget */
	virtual void			createWidget(Widget *owner, widget_init_data *data) = 0;
	virtual void			createWidgetDummy(Widget *owner, widget_init_data *data) = 0;

	/** create font */
	virtual Font_impl *		createFont(Font *base) = 0;

	/** get most similar default font */
	virtual Font_impl *		defaultFont(Font *base) = 0;

	DR_RINLINE Gui *		accGui()				{ return gui; }

	/* constructor */		Gui_impl();
	virtual				~Gui_impl();

protected:
	Gui *				gui;

public:
	friend class Gui;
};


DR_GUI_NS_END

#endif
