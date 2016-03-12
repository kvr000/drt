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

#include <dr/gui/Style_Simple.hxx>
#include <dr/gui/Style_Simple.style.hxx>
#include <dr/gui/StyleFac.hxx>
#include <dr/gui/Paint.hxx>

#include <dr/gui/dev/Gui_impl.hxx>
#include <dr/gui/dev/Widget_impl.hxx>

DR_GUI_NS_BEGIN


DR_EXPORT_MET Style_Simple *Style_Simple::create(StyleFac *stylefac, Widget *widget)
{
	return new Style_Simple(stylefac, widget);
}

DR_EXPORT_MET int Style_Simple::handleEventSelf(int style_orig, Event *evt)
{
	int r;
	DR_Assert(style_orig == GO_MAIN);

	switch (evt->type) {
	case Event::ET_Repaint:
		if ((r = widget->onRepaint(evt)) < 0) {
			Paint *paint;
			if ((paint = paintStart()) != NULL) {
				paintBase(paint);
				paintEnd(paint);
			}
			return true;
		}
		goto out;

	default:
		break;
	}
	r = Style::handleEventSelf(style_orig, evt);

out:
	return r;
}

DR_EXPORT_MET int Style_Simple::handleEvent(int style_orig, Event *evt)
{
	int r;

	if ((r = handleEventCheck(evt)) >= 0)
		goto out;

	r = handleEventSelf(style_orig, evt);

out:
	return r;
}

Widget_impl *Style_Simple::gui_getParent(int gui_orig)
{
	DR_Assert(gui_orig == GO_MAIN);

	return widget->getParent()?widget->getParent()->getImpl():NULL;
}

void Style_Simple::gui_getGeometry(int gui_orig, Rect *geom)
{
	DR_Assert(gui_orig == GO_MAIN);

	*geom = widget->geometry;
}

Widget_impl *Style_Simple::gui_getGeometryAndParent(int gui_orig, Rect *geom)
{
	DR_Assert(gui_orig == GO_MAIN);

	*geom = widget->geometry;
	return widget->getParent()?widget->getParent()->getImpl():NULL;
}

int Style_Simple::gui_getFocused()
{
	return GO_MAIN;
}

DR_EXPORT_MET bool Style_Simple::init()
{
	Gui_impl::widget_init_data ini;

	if (simple_impl)
		return false;

	ini.geom = widget->geometry;
	ini.parent = widget->getParent()?widget->getParent()->getMain():NULL;
	ini.visual = widget->getVisual();

	simple_impl = widget->getGuiImpl()->createWidget(this, 0, &ini);

	return true;
}

DR_EXPORT_MET bool Style_Simple::onShow(bool show)
{
	if (!Style::onShow(show))
		return false;

	if (simple_impl)
		simple_impl->show(show);

	return true;
}

DR_EXPORT_MET bool Style_Simple::onSetFocus(Style *targ)
{
	if (!Style::onSetFocus(targ))
		return false;

	simple_impl->setFocus();

	return true;
}

DR_EXPORT_MET void Style_Simple::changeName(const String &name)
{
}

DR_EXPORT_MET Paint *Style_Simple::paintStart()
{
	if (simple_impl)
		return simple_impl->paint(widget->getVisual());
	return NULL;
}

DR_EXPORT_MET void Style_Simple::paintBase(Paint *paint)
{
	paint->setForeground(stylefac->clr_background);
	paint->fillClip();
}

DR_EXPORT_MET Widget_impl *Style_Simple::getMain()
{
	return simple_impl;
}


DR_GUI_NS_END
