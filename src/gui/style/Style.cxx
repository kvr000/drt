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
#include <dr/gui/Style.style.hxx>
#include <dr/gui/Paint.hxx>

DR_GUI_NS_BEGIN


DR_EXPORT_MET Style::Style(StyleFac *stylefac_, Widget *widget_):
	stylefac(stylefac_),
	widget(widget_)
{
}

DR_EXPORT_MET Style::~Style()
{
}

DR_EXPORT_MET bool Style::isDummy()
{
	return false;
}

DR_EXPORT_MET int Style::handleEvent(int style_orig, Event *evt)
{
	int r;

	if ((r = handleEventCheck(evt)) >= 0)
		goto out;

out:
	return r;
}

DR_EXPORT_MET bool Style::onShow(bool show)
{
	if (widget->isVisible() == show)
		return true;
	return widget->onShow(show);
}

DR_EXPORT_MET void Style::setFocus()
{
}

DR_EXPORT_MET void Style::paintBase(Paint *paint)
{
}

DR_EXPORT_MET void Style::paintEnd(Paint *paint)
{
	paint->unref();
}

DR_EXPORT_MET bool Style::onSetFocus(Style *target)
{
	return widget->onSetFocus(target->widget);
}

DR_EXPORT_MET bool Style::onLooseFocus(Style *target)
{
	return widget->onLooseFocus(target->widget);
}

DR_EXPORT_MET void Style::changeName(const String &name)
{
}


DR_GUI_NS_END
