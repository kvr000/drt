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

#include <dr/x_kw.hxx>

#include <dr/gui/Style.hxx>

#include <dr/gui/wnt/Gui_wnt.hxx>
#include <dr/gui/wnt/Widget_wnt.hxx>

#include <commctrl.h>

DR_GUI_NS_BEGIN


void Gui_wnt::evl_enbfocus(Event *evt, Widget_wnt *widget)
{
	Widget *w;
	if ((w = widget->owner->accFinalFocused()) != NULL) {
		if (w->isEnabled()) {
			w->accGui()->dispatchEvent(w, evt);
		}
	}
}

void Gui_wnt::evl_enabled(Event *evt, Widget_wnt *widget)
{
	if (widget->owner->isEnabled())
		widget->accGui()->dispatchEvent(widget->owner, evt);
}

void Gui_wnt::evl_key(UINT msg, WPARAM wparam, LPARAM lparam, Widget_wnt *widget)
{
	Event_Key dev((const None &)None());

	if (msg == WM_KEYDOWN) {
		MSG m;
		dev.type = Event::ET_KeyPress;
		while (PeekMessage(&m, NULL, WM_CHAR, WM_CHAR, PM_REMOVE)) {
			WCHAR w = m.wParam;
			dev.str.append(&w, 1);
		}
	}
	else {
		DR_Assert(msg == WM_KEYUP);
		dev.type = Event::ET_KeyRelease;
	}

	switch (wparam) {
	case VK_ESCAPE:
		dev.sym = Key::S_Escape;
		break;

	case VK_TAB:
		dev.sym = Key::S_Tab;
		break;

	case VK_LEFT:
		dev.sym = Key::S_Left;
		break;

	case VK_RIGHT:
		dev.sym = Key::S_Right;
		break;

	case VK_UP:
		dev.sym = Key::S_Up;
		break;

	case VK_DOWN:
		dev.sym = Key::S_Down;
		break;

	case VK_SPACE:
		dev.sym = Key::S_Space;
		break;

	default:
		dev.sym = Key::S_None;
		break;
	}

	evl_enbfocus(&dev, widget);
}

LRESULT CALLBACK Gui_wnt::wndproc_handler(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	Widget_wnt *widget = (Widget_wnt *)GetWindowLong(wnd, 0);
	if (widget) {
		switch (msg) {
		case WM_KILLFOCUS:
			{
				Event_Focus dev(Event::ET_FocusOut);
				widget->accGui()->dispatchEvent(widget->owner, &dev);
			}
			return 0;

		case WM_SETFOCUS:
			{
				Event_Focus dev(Event::ET_FocusIn);
				widget->accGui()->dispatchEvent(widget->owner, &dev);
			}
			return 0;

		case WM_MOUSEMOVE:
			if (!widget->mouse_is_in) {
#if !defined DR_OS_WCE && !defined DR_LIBC_CYGWIN
				TRACKMOUSEEVENT tme;
				memset(&tme, 0, sizeof(tme));
				tme.cbSize = sizeof(tme);
				tme.dwFlags = TME_LEAVE;
				tme.hwndTrack = wnd;
				_TrackMouseEvent(&tme);
#endif
				widget->mouse_is_in = true;
				Event dev(Event::ET_Enter);
				widget->accGui()->dispatchEvent(widget->owner, &dev);
			}
			break;

#ifndef DR_OS_WCE
		case WM_MOUSELEAVE:
			{
				widget->mouse_is_in = false;
				Event dev(Event::ET_Leave);
				widget->accGui()->dispatchEvent(widget->owner, &dev);
			}
			return 0;
#endif

		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
			{
				Event_MouseClick dev(Event::ET_ButtonDown);
				dev.button = (msg == WM_LBUTTONDOWN) ? 0 : (msg == WM_MBUTTONDOWN) ? 1 : (msg == WM_RBUTTONDOWN) ? 2 : 0;
				evl_enabled(&dev, widget);
			}
			return 0;

		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
			{
				Event_MouseClick dev(Event::ET_ButtonUp);
				dev.button = (msg == WM_LBUTTONUP) ? 0 : (msg == WM_MBUTTONUP) ? 1 : (msg == WM_RBUTTONUP) ? 2 : 0;
				evl_enabled(&dev, widget);
			}
			return 0;

		case WM_KEYDOWN:
			{
				evl_key(msg, wparam, lparam, widget);
			}
			break;

		case WM_KEYUP:
			{
				evl_key(msg, wparam, lparam, widget);
			}
			break;

		case WM_CHAR:
			return 0;

		case WM_CLOSE:
			{
				Event_Close dev;
				widget->accGui()->dispatchEvent(widget->owner, &dev);
			}
			break;

		case WM_ERASEBKGND:
			{
			}
			break;

		case WM_PAINT:
			{
				Event_Repaint dev;
				widget->accGui()->dispatchEvent(widget->owner, &dev);
			}
			return 0;
		}
	}
	return DefWindowProcW(wnd, msg, wparam, lparam);
}

void Gui_wnt::msg_notify(WeakRef<Gui_wnt> *backref)
{
	Gui_wnt *gui;

	if ((gui = backref->get()) == NULL)
		return;
	gui->ref();

	xtry {
		MSG msg;

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
#if 0
retry_read:
		for (;;) {
			XEvent ev;

			if ((gui = backref->get()) == NULL)
				return;

			if (gui->pendev_count == 0)
				break;

			do {
				XNextEvent(gui->display, &ev);
				gui->pendev_count--;
			} while (XFilterEvent(&ev, XDefaultRootWindow(gui->display)));

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

			}

			if ((gui->pendev_count = XPending(gui->display)) > 0)
				goto retry_read;
#endif
	}
	xcatchany {
		xsafe(gui->unref());
		xrethrowany;
	}
	xend;
}


DR_GUI_NS_END
