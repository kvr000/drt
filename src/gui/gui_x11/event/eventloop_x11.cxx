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

#define XK_MISCELLANY
#define XK_LATIN1
#include <X11/keysymdef.h>

#include <dr/x_kw.hxx>

#include <dr/gui/Style.hxx>

#include <dr/gui/x11/Gui_x11.hxx>
#include <dr/gui/x11/Widget_x11.hxx>

DR_GUI_NS_BEGIN


void Gui_x11::evl_target(Event *evt, XID xid)
{
	Widget_x11 *wg;

	if (XFindContext(display, xid, global->object_context, (XPointer *)(void *)&wg) != 0)
		return;
	gui->dispatchEvent(wg->owner, evt);
}

void Gui_x11::evl_enabled(Event *evt, XID xid)
{
	Widget_x11 *wg;

	if (XFindContext(display, xid, global->object_context, (XPointer *)(void *)&wg) != 0)
		return;
	if (!wg->owner->isEnabled())
		return;
	gui->dispatchEvent(wg->owner, evt);
}

void Gui_x11::evl_enbfocus(Event *evt, XID xid)
{
	Widget *w;
	Widget_x11 *wg;

	if (XFindContext(display, xid, global->object_context, (XPointer *)(void *)&wg) != 0)
		return;

	if ((w = (Widget *)wg->owner->accFinalFocused()) != NULL) {
		if (w->isEnabled())
			gui->dispatchEvent(w, evt);
	}
}

void Gui_x11::evl_key(XEvent *ev)
{
	Status status;
	KeySym sym;
	int ret;
	wchar_t *str;
	Event_Key dev((const None &)None());

	if (ev->xkey.type == KeyPress) {
		dev.type = Event::ET_KeyPress;
		if (!(dev.repeat = last_keycode == ev->xkey.keycode))
			last_keycode = ev->xkey.keycode;
		str = dev.str.lockWide((ret = 3));
retry:
		ret = XwcLookupString(xic, &ev->xkey, str, ret, &sym, &status);
		switch (status) {
		case XBufferOverflow:
			str = dev.str.lockWide(ret);
			goto retry;

		case XLookupNone:
			dev.str.unlock(ret);
			return;

		case XLookupChars:
			dev.str.unlock(ret);
			sym = NoSymbol;
			break;

		case XLookupKeySym:
			dev.str.unlock(ret);
			dev.str.setNull();
			break;

		case XLookupBoth:
			dev.str.unlock(ret);
			break;

		default:
			DR_AssertTodo();
			return;
		}
	}
	else {
		XEvent ev2;
		DR_Assert(ev->xkey.type == KeyRelease);
		if (XCheckTypedEvent(display, KeyPress, &ev2)) {
			XPutBackEvent(display, &ev2);
			last_keycode = ev->xkey.keycode;
			if (ev2.xkey.keycode == ev->xkey.keycode && ev2.xkey.time <= ev->xkey.time+10)
				return;
		}
		dev.type = Event::ET_KeyRelease;
		dev.repeat = false;
		dev.str.setNull();
		sym = XLookupKeysym(&ev->xkey, 0);
	}

	switch (sym) {
	case NoSymbol:
		dev.sym = Key::S_None;
		break;

	case XK_Escape:
		dev.sym = Key::S_Escape;
		break;

	case XK_Tab:
		dev.sym = Key::S_Tab;
		break;

	case XK_Left:
		dev.sym = Key::S_Left;
		break;

	case XK_Right:
		dev.sym = Key::S_Right;
		break;

	case XK_Up:
		dev.sym = Key::S_Up;
		break;

	case XK_Down:
		dev.sym = Key::S_Down;
		break;

	case XK_space:
		dev.sym = Key::S_Space;
		break;

	default:
		dev.sym = Key::S_None;
		break;
	}
	evl_enbfocus(&dev, ev->xkey.window);
}

void Gui_x11::evl_button(XEvent *ev)
{
	Event_MouseClick dev((const None &)None());

	dev.type = (ev->xbutton.type == ButtonPress)?Event::ET_ButtonDown:Event::ET_ButtonUp;
	dev.button = ev->xbutton.button-1;

	evl_enabled(&dev, ev->xbutton.window);
}

MsgSync_Socket::WaitMode Gui_x11::socket_precheck()
{
	if (expose_list)
		return MsgSync_Socket::W_PRECHECK;
	XFlush(display);
	return (MsgSync_Socket::WaitMode)0;
}

void Gui_x11::socket_notify(WeakRef<Gui_x11> *backref)
{
	Gui_x11 *gui;

	if ((gui = backref->get()) == NULL)
		return;
	gui->ref();

	xtry {

retry_read:
		for (;;) {
			XEvent ev;

			for (;;) {
				if (gui->pendev_count-- == 0)
					goto reget_pending;
				XNextEvent(gui->display, &ev);
				if (!XFilterEvent(&ev, XDefaultRootWindow(gui->display)))
					break;
			}

			switch (ev.type) {
			case KeyPress:
			case KeyRelease:
				gui->evl_key(&ev);
				break;

			case ButtonPress:
			case ButtonRelease:
				gui->evl_button(&ev);
				break;

			case MotionNotify:
				break;

			case EnterNotify:
			case LeaveNotify:
				{
					Event dev((ev.type == EnterNotify)?Event::ET_Enter:Event::ET_Leave);
					gui->evl_target(&dev, ev.xany.window);
				}
				break;

			case FocusIn:
			case FocusOut:
				{
					Event_Focus dev((ev.type == FocusIn)?Event::ET_FocusIn:Event::ET_FocusOut);
					gui->evl_target(&dev, ev.xany.window);
				}
				break;

			case KeymapNotify:
				break;

			case Expose:
			case GraphicsExpose:
				{
					Widget_x11 *wg;
					if (XFindContext(gui->display, ev.xexpose.window, gui->global->object_context, (XPointer *)(void *)&wg) != 0)
						break;
					wg->addExpose(Rect(ev.xexpose.x, ev.xexpose.y, ev.xexpose.width, ev.xexpose.height));
				}
				break;

			case NoExpose:
				{
					Widget_x11 *wg;
					if (XFindContext(gui->display, ev.xexpose.window, gui->global->object_context, (XPointer *)(void *)&wg) != 0)
						break;
				}
				break;

			case VisibilityNotify:
			case CreateNotify:
			case DestroyNotify:
			case UnmapNotify:
			case MapNotify:
			case MapRequest:
			case ReparentNotify:
			case ConfigureNotify:
			case ConfigureRequest:
			case GravityNotify:
			case ResizeRequest:
			case CirculateNotify:
			case CirculateRequest:
			case PropertyNotify:
			case SelectionClear:
			case SelectionRequest:
			case SelectionNotify:
				//case GuiColormapNotify:
				break;

			case ClientMessage:
				if (ev.xclient.message_type == gui->wm_protocols) {
					if ((Atom)ev.xclient.data.l[0] == gui->wm_takefocus) {
#if 1 // ignore ? We have FocusIn/FocusOut
						Event_Focus dev(Event::ET_FocusIn);
						gui->evl_target(&dev, ev.xany.window);
#endif
					}
					else if ((Atom)ev.xclient.data.l[0] == gui->wm_delwin || (Atom)ev.xclient.data.l[1] == gui->wm_quitapp) {
						Event_Close dev;
						gui->evl_target(&dev, ev.xany.window);
					}
				}
				break;

			case MappingNotify:
				if (ev.xmapping.request != MappingPointer)
					XRefreshKeyboardMapping(&ev.xmapping);
				break;

			}
		}

reget_pending:
		if ((gui->pendev_count = XPending(gui->display)) > 0)
			goto retry_read;

		while (gui->expose_list) {
			Widget_x11 *widget;
			Event_Repaint dev;

			widget = gui->expose_list;
			gui->expose_remove(widget);
			gui->gui->dispatchEvent(widget->owner, &dev);
		}
	}
	xcatchany {
		xsafe(gui->unref());
		xrethrowany;
	}
	xend;
	xsafe(gui->unref());
}


DR_GUI_NS_END
