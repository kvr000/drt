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

#ifndef dr__gui__Gui__hxx__
# define dr__gui__Gui__hxx__

#include <dr/emits.hxx>
#include <dr/Array.hxx>
#include <dr/base.hxx>

#include <dr/gui/def_gui.hxx>
#include <dr/gui/StyleFactory.hxx>

#include <dr/gui/Visual.hxx>

DR_GUI_NS_BEGIN


class Gui;
class Gui_impl;
class StyleFactory;
class Widget;
class Style;
class Visual;
class Event;
class Font;


class DR_GUI_PUB Gui: public Object
{
	DR_OBJECT_DECL_SIMPLE(Gui, Object);

public:
	/* constructor */		Gui();
	virtual				~Gui();

public:
	void				setOptions(const String &options);

public:
	void				open(const Eslot1<bool, Gui *> &check_impl = Null());
	void				close();
	void				flush();

	void				runUntilQuit();

public: /* signals */
	Eslot1<bool, Gui *>		nowindow;
	Eslot1<void, Gui *>		quited;

protected:
	void				insertObject(Widget *obj);
	void				removeObject(Widget *obj);

	StyleFactory *			accStyleFactory();

public:
	void				createStyle(Widget *targ);

public:
	static bool			dispatchEvent(Widget *target, Event *evt);

protected:
	void				l_close();

public: /* read only */
	Gui_impl *			impl;
	Ref<StyleFactory>		style_fac;

protected:
	int				window_count;

	Widget *			object_list;

public:
	RArray<Visual>			visuals;

protected:
	String				options;

public:
	friend class Widget;
	friend class Style;
	friend class Font;
};


DR_GUI_NS_END

#endif
