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

#include <dr/Const.hxx>

#include <dr/gui/Widget.hxx>
#include <dr/gui/Layout.hxx>
#include <dr/gui/Style.hxx>
#include <dr/gui/StyleFactory.hxx>
#include <dr/gui/Visual.hxx>
#include <dr/gui/Paint.hxx>
#include <dr/gui/dev/Widget_impl.hxx>

DR_GUI_NS_BEGIN

DR_OBJECT_DEF(DR_GUI_NS_STR, Widget, Object);
DR_OBJECT_IMPL_SIMPLE(Widget);


DR_EXPORT_DTS const String Widget::style_name(Const::string("widget_none"));
DR_EXPORT_MET const String Widget::notify_enabled(Const::string(DR_GUI_NS_STRP("Widget::notify_enabled")));


DR_EXPORT_MET Widget::Widget(Gui *gui_):
	type_flags(TF_IsWidget|TF_Visible),
	gui(gui_),
	parent(NULL),
	name(Null()),
	geometry(None()),
	main_impl(NULL),
	focused_child(NULL),
	hold_layout(NULL),
	style_fac(gui->accStyleFactory(), true)
{
	shell = new shell_data;
	shell->is_active = false;

	child_first = child_last = NULL;
	child_next = child_prev = NULL;

	if (type_flags&TF_ReqDisabled)
		type_flags |= TF_Disabled;
	else
		type_flags &= ~TF_Disabled;

	gui->insertObject(this);
}

DR_EXPORT_MET Widget::Widget(Widget *parent_):
	type_flags(TF_IsWidget|TF_Visible),
	gui(parent_->gui),
	parent(parent_),
	name(Null()),
	geometry(None()),
	main_impl(NULL),
	focused_child(NULL),
	hold_layout(NULL),
	style_fac(gui->accStyleFactory(), true)
{
	shell = NULL;

	child_first = child_last = NULL;
	child_next = child_prev = NULL;

	if (type_flags&TF_ReqDisabled)
		type_flags |= TF_Disabled;
	else
		type_flags &= ~TF_Disabled;

	parent->gui_insertChild(this);
}

DR_EXPORT_MET Widget::~Widget()
{
	DR_Assert(main_impl == NULL);
	if (shell)
		delete shell;
}

DR_EXPORT_MET bool Widget::unrefed()
{
	if (gui)
		gui_removing();
	return Object::unrefed();
}

DR_EXPORT_MET void Widget::gui_removing()
{
	while (child_first)
		child_first->gui_destroy();

	if (parent)
		parent->gui_removeChild(this);
	else
		gui->removeObject(this);

	delete main_impl;

	gui = NULL;
}

DR_EXPORT_MET void Widget::gui_removeChild(Widget *obj)
{
	onRemovingChild(obj);
	if (obj->child_prev == NULL) {
		DR_Assert(child_first == obj);
		if ((child_first = obj->child_next) != NULL)
			obj->child_next->child_prev = NULL;
		else
			child_last = NULL;
	}
	else {
		DR_Assert(child_first != obj);
		if ((obj->child_prev->child_next = obj->child_next) != NULL) {
			obj->child_next->child_prev = obj->child_prev;
		}
		else {
			DR_Assert(child_last == obj);
			child_last = obj->child_prev;
		}
	}
}

DR_EXPORT_MET void Widget::gui_insertChild(Widget *obj)
{
	if ((obj->child_next = child_first) != NULL)
		child_first->child_prev = obj;
	else
		child_last = child_first;
	obj->child_prev = NULL;
	child_first = obj;

	obj->ref();
}

DR_EXPORT_MET void Widget::gui_destroy()
{
	gui_removing();

	unref();
}

DR_EXPORT_MET void Widget::onRemovingChild(Widget *child)
{
	if (child == focused_child)
		focused_child = NULL;
	if (child == hold_layout)
		hold_layout = NULL;
}

DR_EXPORT_MET String Widget::getStyleName()
{
	return style_name;
}

DR_EXPORT_MET Widget *Widget::accShell()
{
	Widget *shell_;

	for (shell_ = this; shell_->parent; shell_ = (Widget *)shell_->parent);

	return shell_;
}

DR_EXPORT_MET Widget *Widget::accFinalFocused()
{
	Widget *focus;
	
	for (focus = accShell(); focus->focused_child; focus = focus->focused_child);

	return focus;
}

DR_EXPORT_MET int Widget::handleEvent(Event *evt)
{
	switch (evt->type) {
	case Event::ET_Close:
		gui_destroy();
		return 0;

	case Event::ET_MoveFocus:
		return handleInherited(evt);

	case Event::ET_Repaint:
		onRepaint(evt);
		return 0;

#if 0
	case Event::ET_FocusIn:
		Fatal::plog("get focus: %s@%p\n", classname().utf8().toStr(), this);
		break;

	case Event::ET_FocusOut:
		Fatal::plog("lost focus: %s@%p\n", classname().utf8().toStr(), this);
		break;
#endif

	default:
		break;
	}

	return -1;
}

DR_EXPORT_MET int Widget::handleInherited(Event *evt)
{
	switch (evt->type) {
	case Event::ET_MoveFocus:
		{
			Event_MoveFocus *mf = (Event_MoveFocus *)evt;
			if (!loosingFocus())
				return 0;
			if (mf->forward) {
				if (focused_child) {
					focused_child = focused_child->child_next;
				}
				else {
					focused_child = child_first;
				}
				for (; focused_child; focused_child = focused_child->child_next) {
					if ((focused_child->type_flags&(TF_Disabled|TF_Unfocusable)) == 0)
						break;
				}
			}
			else {
				if (focused_child) {
					focused_child = focused_child->child_prev;
				}
				else {
					focused_child = child_last;
				}
				for (; focused_child; focused_child = focused_child->child_prev) {
					if ((focused_child->type_flags&(TF_Disabled|TF_Unfocusable)) == 0)
						break;
				}
			}
			if (!focused_child)
				break;
			focused_child->settingFocus();
			return 0;
		}
		break;

	default:
		break;
	}
	return -1;
}

DR_EXPORT_MET void Widget::setEnabled(bool enabled_)
{
	if (((type_flags&TF_ReqDisabled) == 0) == enabled_)
		return;
	l_setEnabled(enabled_);
	if (parent) {
		((type_flags&TF_ReqDisabled) || (parent->type_flags&TF_Disabled)) ? (type_flags |= TF_Disabled) : (type_flags &= ~TF_Disabled);
	}
	else {
		(type_flags&TF_ReqDisabled) ? (type_flags |= TF_Disabled) : (type_flags &= ~TF_Disabled);
	}
	onEnabledChanged(enabled_);
}

DR_EXPORT_MET void Widget::onEnabledChanged(bool enabled_)
{
	if (Eslot1<void, bool> *n = findNotifierEnabled())
		(*n)(enabled_);
}

DR_EXPORT_MET bool Widget::loosingFocus()
{
	type_flags &= ~TF_Focused;
	return true;
}

DR_EXPORT_MET bool Widget::settingFocus()
{
	type_flags |= TF_Focused;
	return true;
}

DR_GUI_MET void Widget::onInit()
{
	if (!main_impl) {
		Gui_impl::widget_init_data wd;
		wd.parent = parent ? ((Widget *)parent)->main_impl : NULL;
		wd.geometry = geometry;
		wd.visual = accVisual();
		accGuiImpl()->createWidget(this, &wd);
	}
	if (isVisible())
		main_impl->show(true);
}

DR_EXPORT_MET void Widget::init()
{
	onInit();
	initChilds();
}

DR_EXPORT_MET void Widget::initChilds()
{
	Widget *child;
	for (child = child_first; child; child = child->child_next)
		child->init();
}

DR_GUI_MET bool Widget::onShow(bool show_)
{
	if (isVisible() == show_)
		return true;
	l_setVisible(show_);
	return true;
}

DR_EXPORT_MET void Widget::onRepaint(Event *evt)
{
	paintBase(tref(main_impl->paint(visual)));
}

DR_EXPORT_MET void Widget::paintBase(Paint *painter)
{
	painter->setForeground(style_fac->clr_background);
	painter->fillClip();
}


DR_GUI_MET bool Widget::show(bool show_)
{
	main_impl->show(show_);
	return true;
}

DR_GUI_MET void Widget::redraw()
{
	if (main_impl) {
		Rect r = geometry;
		r.move0();
		main_impl->redraw(r);
	}
}

DR_GUI_MET void Widget::redraw(const Rect &rect)
{
	main_impl->redraw(rect);
}

DR_GUI_MET bool Widget::setFocus()
{
	Widget *cur_focus, *change_focus;
	change_focus = this;
	for (cur_focus = this; cur_focus && cur_focus->accParent(); cur_focus = cur_focus->accParent()) {
		if (cur_focus->accParent()->focused_child != cur_focus)
			change_focus = cur_focus;
	}
	if (change_focus->accParent()) {
		change_focus = change_focus->accParent();
	}
	for (cur_focus = change_focus; cur_focus->focused_child; cur_focus = cur_focus->focused_child);
#if 0
	for (change_focus = change_focus->accParent(); cur_focus != change_focus; cur_focus = change_focus->accParent()) {
		if (!cur_focus->loosingFocus())
			return false;
	}
#else
	if (!cur_focus->loosingFocus())
		return false;
#endif
#if 0
	for (; cur_focus->focused_child; cur_focus = cur_focus->focused_child);
	if (!cur_focus->loosingFocus())
		return false;
#endif

	if (!settingFocus()) {
		cur_focus->settingFocus();
		return false;
	}
	for (cur_focus = this; cur_focus != change_focus; cur_focus = cur_focus->accParent()) {
		cur_focus->accParent()->focused_child = cur_focus;
	}
	return true;
}

DR_GUI_MET void Widget::setName(const String &name_)
{
	name = name_;
	if (main_impl)
		main_impl->changeName(name_);
}

DR_GUI_MET Visual *Widget::p_assignVisual()
{
	DR_Assert(*visual == NULL);
	if (parent)
		visual.setDoref(((Widget *)parent)->accVisual());
	else
		visual.setDoref(gui->visuals[0]);
	return visual;
}

DR_EXPORT_MET Eslot_g *Widget::findNotifier(const String &notif_id)
{
	if (Notifier *p = notifiers.accValue(notif_id)) {
		if (!p->isNull())
			return p;
	}
	return NULL;
}

DR_GUI_MET void Widget::notifyEnabled(const Eslot1<void, bool> &slot)
{
	notifiers[notify_enabled] = slot;
}


DR_GUI_NS_END
