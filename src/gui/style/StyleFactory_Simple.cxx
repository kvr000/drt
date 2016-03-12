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

#include <dr/Const.hxx>

#include <dr/gui/Paint.hxx>

#include <dr/gui/Label_Simple.hxx>
#include <dr/gui/Button_Simple.hxx>
#include <dr/gui/CheckBox_Simple.hxx>
#include <dr/gui/TextEdit_Simple.hxx>

#include <dr/gui/StyleFactory_Simple.hxx>

DR_GUI_NS_BEGIN


DR_OBJECT_DEF(DR_GUI_NS_STR, StyleFactory_Simple, StyleFactory);
DR_OBJECT_IMPL_SIMPLE(StyleFactory_Simple);


DR_EXPORT_MET StyleFactory_Simple::StyleFactory_Simple(Gui *gui_):
	StyleFactory(gui_)
{
}

DR_EXPORT_MET StyleFactory_Simple::~StyleFactory_Simple()
{
}

DR_EXPORT_MET Control *StyleFactory_Simple::createControl(const String &control_name, Widget *parent)
{
	if (control_name == Button::comp_name) {
		return parent ? new Button_Simple(parent) : new Button_Simple(gui);
	}
	if (control_name == CheckBox::comp_name) {
		return parent ? new CheckBox_Simple(parent) : new CheckBox_Simple(gui);
	}
	if (control_name == Label::comp_name) {
		return parent ? new Label_Simple(parent) : new Label_Simple(gui);
	}
	if (control_name == TextEdit::comp_name) {
		return parent ? new TextEdit_Simple(parent) : new TextEdit_Simple(gui);
	}
	return NULL;
}

DR_EXPORT_MET void StyleFactory_Simple::drawFocusMark(Paint *painter, Control *control)
{
	Rect g(control->geometry, Null());
	painter->setForeground(clr_focus);
	painter->setLineStyle(1);
	g.grow(-2);
	painter->drawRectangle(g);
}


DR_GUI_NS_END
