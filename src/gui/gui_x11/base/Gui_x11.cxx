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

#include <stdio.h>
#include <locale.h>

#include <dr/gui/x11/X11wrap/Xresource.h>

#include <dr/Const.hxx>
#include <dr/Subsystem.hxx>
#include <dr/BadSubsysException.hxx>
#include <dr/MsgSync.hxx>
#include <dr/x_kw.hxx>

#include <dr/gui/x11/Widget_x11.hxx>
#include <dr/gui/x11/Visual_x11.hxx>
#include <dr/gui/x11/Font_x11.hxx>

#include <dr/gui/x11/Gui_x11.hxx>

DR_GUI_NS_BEGIN


DR_OBJECT_DEF(DR_GUI_NS_STR, Gui_x11, Gui_impl);

DR_OBJECT_IMPL_SIMPLE(Gui_x11);

Gui_x11::Gui_x11(Gui_SS_x11 *ss):
	DR_OBJECT_INIT(Gui_x11, Gui_impl, ()),
	global(ss)
{
	global->busy_ref();

	weak_ref = new WeakRef<Gui_x11>(this);
	display = NULL;
	syncing = false;
	expose_list = NULL;
}

Gui_x11::~Gui_x11()
{
	if (display) {
		if (*sock_sync) {
			sock_sync->unregisterSocket((void *)(size_t)ConnectionNumber(display));
			sock_sync.setNull();
		}
		def_font->unref();
		def_visual->unref();
		XDestroyIC(xic);
		XCloseIM(xim);
		XCloseDisplay(display);
	}
	weak_ref->reset();
	weak_ref->unref();
	global->busy_unref();
}

void Gui_x11::open(Gui *gui_)
{
	Eslot1<void, MsgSync_Socket::WaitMode> islot(Eslot1<void, WeakRef<Gui_x11> *>(&Gui_x11::socket_notify).a1Set<IRef<WeakRef<Gui_x11> > >(weak_ref).aAdd<MsgSync_Socket::WaitMode>());
	Eslot0<MsgSync_Socket::WaitMode> cslot(this, &Gui_x11::socket_precheck);

	// HACK: for now...
	setlocale(LC_ALL, "");

	if ((display = XOpenDisplay(NULL)) == NULL)
		xthrownew(BadSubsysException(Const::string("Gui_SS_x11"), "failed to open display"));
	gui = gui_;

	if ((xim = XOpenIM(display, NULL, NULL, NULL)) != NULL) {
		xic = XCreateIC(xim, XNInputStyle, XIMPreeditNothing|XIMStatusNothing, XNClientWindow, XDefaultRootWindow(display), NULL);
		XmbResetIC(xic);
	}
	last_keycode = 0;
	dbg_sync();

	/* register server socket */
	xtry {
		sock_sync.setNoref((MsgSync_Socket *)Thread::getMsgIface(MsgSync_Socket::iface_name));
	}
	xcatch (Exception, ex) {
		xthrownew(BadSubsysException(ex.getAndNull(), Const::string("Gui_SS_x11"), "failed to get MsgSync_Socket interface"));
	}
	xend;
	xtry {
		sock_sync->registerSocket((void *)(size_t)ConnectionNumber(display), MsgSync_Socket::W_READ, cslot, islot);
	}
	xcatchany {
		sock_sync.setNull();
		xrethrowany;
	}
	xend;

	/* setup & init */
	XSetErrorHandler(&xerror_handler);
	pendev_count = 0;

	/* init WM atoms */
	wm_delwin = XInternAtom(display, "WM_DELETE_WINDOW", False);
	wm_quitapp = XInternAtom(display, "WM_QUIT_APP", False);
	wm_protocols = XInternAtom(display, "WM_PROTOCOLS", False);
	wm_takefocus = XInternAtom(display, "WM_TAKE_FOCUS", False);
	dbg_sync();

	/* register visuals */
	{
		def_visual = new Visual_x11(this, DefaultVisual(display, DefaultScreen(display)));
		gui->visuals.appendDoref(def_visual);
	}
	dbg_sync();

	/* create default font */
	{
		XFontStruct *font_str;

		if ((font_str = XLoadQueryFont(display, "fixed")) == NULL)
			goto out_visual;

		def_font = new Font_x11(this, font_str);
	}
	dbg_sync();

	return;
#if 0
out_font:
	def_font->unref();
#endif

out_visual:
	gui->visuals.resize(0);
	def_visual->unref();

//out_socket:
	sock_sync->unregisterSocket((void *)(size_t)ConnectionNumber(display));
	sock_sync.setNull();

//out_display:
	gui = NULL;

	XCloseDisplay(display);
	display = NULL;
}

void Gui_x11::flush()
{
	XFlush(display);
}

int Gui_x11::xerror_handler(Display *display, XErrorEvent *ev)
{
	int pos;
	char buf[512];

	pos = sprintf(buf, "X Error: code %d major %d minor %d:\n", ev->error_code, ev->request_code, ev->minor_code);
	XGetErrorText(display, ev->error_code, buf+pos, sizeof(buf)-pos-2);
	strcat(buf, "\n");

	Fatal::printFailed(buf);

	return 1;
}

Object *Gui_SS_x11::create(const String &object)
{
	if (object == Gui_impl::comp_name)
		return (Gui_impl *)new Gui_x11(this);
	return NULL;
}

void *Gui_SS_x11::create(const String &object, const String &iface)
{
	if (!object.isNull())
		return Subsystem::create(object, iface);
	if (iface == Gui_impl::comp_name)
		return (Gui_impl *)new Gui_x11(this);
	return NULL;
}

Object *Gui_SS_x11::create(int object)
{
	return NULL;
}

void *Gui_SS_x11::create(int object, int iface)
{
	return NULL;
}

Gui_SS_x11::Gui_SS_x11()
{
	object_context = XUniqueContext();
}

Gui_SS_x11::~Gui_SS_x11()
{
	// no way to release XContext
}

extern "C" DR_EXPORT_MTS Subsystem *createSS_drGui_x11(const String &name)
{
	return new Gui_SS_x11;
}


DR_GUI_NS_END
