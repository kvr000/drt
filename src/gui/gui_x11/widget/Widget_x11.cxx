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

#include <dr/gui/x11/Widget_x11.hxx>
#include <dr/gui/x11/Paint_x11.hxx>
#include <dr/gui/x11/Visual_x11.hxx>

DR_GUI_NS_BEGIN


/* a little documentation about expose_prev/next:
 *
 * When expose_next is 0x4, the object is not in expose_list
 * When expose_prev is 0x4, expose_rect is not valid
 */

Widget_x11::~Widget_x11()
{
	XDeleteContext(accDisplay(), impl_window, accGuiImpl()->global->object_context);
	XDestroyWindow(accDisplay(), impl_window);

	if (expose_next != (void *)4)
		accGuiImpl()->expose_remove(this);
}

void Widget_x11::changeName(const String &name)
{
	// TODO
}

void Widget_x11::show(bool show_)
{
	if (show_)
		XMapWindow(accDisplay(), impl_window);
	else
		XUnmapWindow(accDisplay(), impl_window);
}

void Widget_x11::setFocus()
{
	XSetInputFocus(accDisplay(), impl_window, X11_RevertToNone, CurrentTime);
}

void Widget_x11::addExpose(const Rect &add)
{
	DR_Assert(add.fx >= 0 && add.fy >= 0);
	if (expose_prev == (void *)4)
		expose_rect = add;
	else
		expose_rect.extend(add);
	if (expose_next == (void *)4)
		accGuiImpl()->expose_insert(this);
}

void Widget_x11::redraw(const Rect &rect)
{
	addExpose(rect);
}

Paint *Widget_x11::paint(Visual *visual)
{
	XRectangle clip;
	GC gc;

	if (expose_prev == (void *)4)
		return NULL;

	if (expose_next != (void *)4)
		accGuiImpl()->expose_remove(this);
	expose_prev = (Widget_x11 *)4;

	gc = XCreateGC(accDisplay(), impl_window, 0, NULL);
	clip.x = expose_rect.getL(); clip.y = expose_rect.getT(); clip.width = expose_rect.getW(); clip.height = expose_rect.getH();
	XSetClipRectangles(accDisplay(), gc, 0, 0, &clip, 1, Unsorted);

	accGuiImpl()->dbg_sync();

	return new Paint_x11(accGuiImpl(), expose_rect, this, (Visual_x11 *)visual, gc);
}


void Gui_x11::expose_remove(Widget_x11 *widget)
{
	if (widget->expose_prev == NULL) {
		DR_Assert(expose_list == widget);
		if ((expose_list = widget->expose_next) != NULL)
			expose_list->expose_prev = NULL;
	}
	else {
		if ((widget->expose_prev->expose_next = widget->expose_next) != NULL)
			widget->expose_prev->expose_next->expose_prev = widget->expose_prev;
	}
	widget->expose_next = (Widget_x11 *)4;
}

void Gui_x11::expose_insert(Widget_x11 *widget)
{
	if ((widget->expose_next = expose_list) != NULL)
		widget->expose_next->expose_prev = widget;
	widget->expose_prev = NULL;
	expose_list = widget;
}

void Gui_x11::createWidget(Widget *owner, widget_init_data *data)
{
	Rect geom(data->geometry);
	Atom protocols[2];
	XSetWindowAttributes wattr;
	Widget_x11 *w;
	Widget_x11 *parent = (Widget_x11 *)data->parent;
	Window win;
	long wattr_mask = CWEventMask|CWDontPropagate;

	wattr.event_mask = KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask|ButtonMotionMask|EnterWindowMask|LeaveWindowMask|PointerMotionMask|ExposureMask|FocusChangeMask;
	wattr.do_not_propagate_mask = KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask|PointerMotionMask|ButtonMotionMask;

	while (parent && parent->isDummy()) {
		Widget *wo = parent->owner;
		geom.move(wo->geometry.getLT());
		parent = (Widget_x11 *)(wo->accParent())->main_impl;
	}

	if ((win = XCreateWindow(display, parent?parent->impl_window:DefaultRootWindow(display), geom.getL(), geom.getT(), geom.getW(), geom.getH(), 0, data->visual->depth, InputOutput, CopyFromParent, wattr_mask, &wattr)) == X11_None) {
		createWidgetDummy(owner, data);
		return;
	}
	w = new Widget_x11(this, owner, win);
	XSaveContext(display, win, global->object_context, (XPointer)w);
	protocols[0] = wm_delwin;
	protocols[1] = wm_takefocus;
	XSetWMProtocols(display, win, protocols, 2);

	dbg_sync();
}

void Gui_x11::createWidgetDummy(Widget *owner, widget_init_data *data)
{
	new Widget_x11(this, owner, X11_None);
}


DR_GUI_NS_END
