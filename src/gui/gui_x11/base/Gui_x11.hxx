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

#ifndef dr__gui__x11__Gui_x11__hxx__
# define dr__gui__x11__Gui_x11__hxx__

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <dr/gui/x11/X11wrap/redef_x11.hxx>

#include <dr/WeakRef.hxx>
#include <dr/Subsystem.hxx>

#include <dr/gui/Event.hxx>
#include <dr/gui/Gui.hxx>
#include <dr/gui/dev/Gui_impl.hxx>

#include <dr/MsgSync_Socket.hxx>

DR_GUI_NS_BEGIN


class Widget_x11;
class Font_x11;
class Visual_x11;

class Gui_SS_x11;


class Gui_x11: public Gui_impl
{
	DR_OBJECT_DECL_SIMPLE(Gui_x11, Gui_impl);

public:
					Gui_x11(Gui_SS_x11 *ss);
	virtual				~Gui_x11();

public:
	virtual void			open(Gui *gui);
	virtual void			flush();
	DR_RINLINE void			dbg_sync();

	/* implementation methods */
	virtual void			createWidget(Widget *owner, widget_init_data *data);
	virtual void			createWidgetDummy(Widget *owner, widget_init_data *data);
	virtual Font_impl *		createFont(Font *base);
	virtual Font_impl *		defaultFont(Font *base);

public:
	MsgSync_Socket::WaitMode	socket_precheck();
	static void			socket_notify(WeakRef<Gui_x11> *weakref);

	static int			xerror_handler(Display *display, XErrorEvent *ev);

public: /* expose list */
	void				expose_remove(Widget_x11 *widget);
	void				expose_insert(Widget_x11 *widget);

protected:
	void				evl_target(Event *evt, XID xid);
	void				evl_enabled(Event *evt, XID xid);
	void				evl_enbfocus(Event *evt, XID xid);
	void				evl_key(XEvent *ev);
	void				evl_button(XEvent *ev);

public: // protected, but public for implementation
	Display *			display;

public: // protected, but public for implementation
	WeakRef<Gui_x11> *		weak_ref;
	Ref<MsgSync_Socket>		sock_sync;
	Gui_SS_x11 *			global;

public: // protected, but public for implementation
	int				pendev_count;
	bool				syncing;

public:	/* input */
	XIM				xim;
	XIC				xic;
	unsigned			last_keycode;

public: /* defaults */
	Visual_x11 *			def_visual;
	Font_x11 *			def_font;

public: /* wm messages */
	Atom				wm_delwin;
	Atom				wm_quitapp;
	Atom				wm_protocols;
	Atom				wm_takefocus;

public:
	Widget_x11 *			expose_list;

protected:
	enum If
	{
		IF_GUI_IMPL,
	};
};

class Gui_SS_x11: public Subsystem
{
public:
	/**/				Gui_SS_x11();
	virtual				~Gui_SS_x11();

public:
	Object *			create(const String &object);
	void *				create(const String &object, const String &iface);
	Object *			create(int object);
	void *				create(int object, int iface);

public: // for implementation
	XContext			object_context;
};

DR_RINLINE void Gui_x11::dbg_sync()
{
	if (syncing)
		XSync(display, False);
}


DR_GUI_NS_END

#endif
