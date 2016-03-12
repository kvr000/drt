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

#include <dr/gui/Style.hxx>

#include <dr/gui/wnt/Widget_wnt.hxx>
#include <dr/gui/wnt/Paint_wnt.hxx>
#include <dr/gui/wnt/Visual_wnt.hxx>

DR_GUI_NS_BEGIN


Widget_wnt::~Widget_wnt()
{
	DestroyWindow(impl_window);
}

void Widget_wnt::changeName(const String &name)
{
	SetWindowTextW(impl_window, name.wide().toStr());
}

void Widget_wnt::show(bool show_)
{
	if (show_)
		ShowWindow(impl_window, owner->accParent() ? SW_SHOW : SW_MAXIMIZE);
	else
		ShowWindow(impl_window, SW_HIDE);
}

void Widget_wnt::setFocus()
{
	SetFocus(impl_window);
}

void Widget_wnt::redraw(const Rect &rect)
{
	InvalidateRgn(impl_window, NULL, TRUE);
}

Paint *Widget_wnt::paint(Visual *visual)
{
	Paint_wnt *p = new Paint_wnt(accGuiImpl(), this, (Visual_wnt *)visual);
	if (!p->initContext()) {
		// dummy ?
	}
	return p;
}

void Gui_wnt::createWidget(Widget *owner, widget_init_data *data)
{
	Rect geom(data->geometry);

	Widget_wnt *w;
	Widget_wnt *parent = (Widget_wnt *)data->parent;
	HWND win;

	while (parent && parent->isDummy()) {
		Widget *wo = parent->owner;
		geom.move(wo->geometry.getLT());
		parent = (Widget_wnt *)(wo->accParent())->main_impl;
	}

#if 0
#define SCREEN_WIDTH native_geom.right-native_geom.left+1
#define SCREEN_HEIGHT native_geom.bottom-native_geom.top+1
#elif 0
#define SCREEN_WIDTH GetSystemMetrics(SM_CXSCREEN)
#define SCREEN_HEIGHT GetSystemMetrics(SM_CYSCREEN)
#else
#define SCREEN_WIDTH CW_USEDEFAULT
#define SCREEN_HEIGHT CW_USEDEFAULT
#endif

	if (!parent) {
		RECT native_geom = { geom.getL(), geom.getT(), geom.getR(), geom.getB() };
		AdjustWindowRectEx(&native_geom, WS_VISIBLE|WS_CLIPCHILDREN|WS_BORDER|WS_CAPTION, FALSE, 0);
#ifdef DR_OS_WCE
		win = CreateWindowExW(0, (WCHAR *)(LONG_PTR)window_class, owner->accName().wide().toStr(), WS_MAXIMIZEBOX|WS_MINIMIZEBOX|WS_SYSMENU|WS_VISIBLE|WS_CLIPCHILDREN, CW_USEDEFAULT /*native_geom.left*/, CW_USEDEFAULT /*native_geom.top*/, SCREEN_WIDTH /*native_geom.right-native_geom.left+1*/, SCREEN_HEIGHT /*native_geom.bottom-native_geom.top+1*/, NULL, NULL, app_instance, data);
#else
		win = CreateWindowExW(0, (WCHAR *)(LONG_PTR)window_class, owner->accName().wide().toStr(), WS_OVERLAPPEDWINDOW|WS_VISIBLE|WS_CLIPCHILDREN, CW_USEDEFAULT /*native_geom.left*/, CW_USEDEFAULT /*native_geom.top*/, native_geom.right-native_geom.left+1, native_geom.bottom-native_geom.top+1, NULL, NULL, app_instance, data);
#endif
		//SetForegroundWindow(win);
	}
	else {
		win = CreateWindowExW(0, (WCHAR *)(LONG_PTR)window_class, owner->accName().wide().toStr(), WS_VISIBLE|WS_CLIPCHILDREN|WS_CHILD, geom.getL(), geom.getT(), geom.getW(), geom.getH(), parent->impl_window, NULL, app_instance, data);
	}

	if (win == NULL) {
		createWidgetDummy(owner, data);
		return;
	}
	w = new Widget_wnt(this, owner, win);
	SetWindowLong(win, 0, (LONG_PTR)w);
#ifdef DR_OS_WCE
	//InvalidateRgn(win, NULL, TRUE);
	//PostMessageW(win, WM_PAINT, 0, 0);
	UpdateWindow(win);
#endif

	dbg_sync();
}

void Gui_wnt::createWidgetDummy(Widget *owner, widget_init_data *data)
{
	new Widget_wnt(this, owner, NULL);
}


DR_GUI_NS_END
