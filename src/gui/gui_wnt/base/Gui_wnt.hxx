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

#ifndef dr__gui__wnt__Gui_wnt__hxx__
# define dr__gui__wnt__Gui_wnt__hxx__

#include <windows.h>

#include <dr/gui/wnt/def_gui_wnt.hxx>

#include <dr/WeakRef.hxx>
#include <dr/Subsystem.hxx>

#include <dr/gui/Event.hxx>
#include <dr/gui/Gui.hxx>

#include <dr/MsgSync.hxx>
#include <dr/MsgSync_WntMsg.hxx>

#include <dr/gui/dev/Gui_impl.hxx>

DR_GUI_NS_BEGIN
class Gui_wnt;
DR_GUI_NS_END

#include <dr/gui/wnt/Font_wnt.hxx>
#include <dr/gui/wnt/Visual_wnt.hxx>

DR_GUI_NS_BEGIN


class Widget_wnt;
class Font_wnt;
class Visual_wnt;

class Gui_SS_wnt;


class DR_GUI_WNT_PUB Gui_wnt: public Gui_impl
{
	DR_OBJECT_DECL_SIMPLE(Gui_wnt, Gui_impl);

public:
	/* constructor */		Gui_wnt(Gui_SS_wnt *ss);
	virtual				~Gui_wnt();

public:
	virtual void			open(Gui *gui);
	virtual void			flush();
	DR_RINLINE void			dbg_sync();

	/* implementation methods */
	virtual void			createWidget(Widget *owner, widget_init_data *data);
	virtual void			createWidgetDummy(Widget *owner, widget_init_data *data);
	virtual Font_impl *		createFont(Font *base);
	virtual Font_impl *		defaultFont(Font *base);

protected:
	static void			msg_notify(WeakRef<Gui_wnt> *);
	static void			evl_enabled(Event *evt, Widget_wnt *widget);
	static void			evl_enbfocus(Event *evt, Widget_wnt *widget);
	static void			evl_key(UINT msg, WPARAM wparam, LPARAM lparam, Widget_wnt *widget);
	static LRESULT CALLBACK		wndproc_handler(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);


public: // protected, but public for implementation
	Ref<WeakRef<Gui_wnt> >		weak_ref;
	Ref<MsgSync_WntMsg>		msg_sync;
	Gui_SS_wnt *			global;

public: // protected, but public for implementation
	int				pendev_count;
	bool				syncing;

public:	/* input */
public: /* defaults */
	Ref<Visual_wnt>			default_visual;
	Ref<Font_wnt>			default_font;

public:
	HINSTANCE			app_instance;
	ATOM				window_class;

protected:
	enum If
	{
		IF_GUI_IMPL,
	};
};

class Gui_SS_wnt: public Subsystem
{
public:
	/**/				Gui_SS_wnt();
	virtual				~Gui_SS_wnt();

public:
	Object *			create(const String &object);
	void *				create(const String &object, const String &iface);
	Object *			create(int object);
	void *				create(int object, int iface);
};

DR_RINLINE void Gui_wnt::dbg_sync()
{
}


DR_GUI_NS_END

#endif
