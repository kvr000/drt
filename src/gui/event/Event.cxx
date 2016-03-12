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

//#include <alloca.h>
#include <stdlib.h>

#include <dr/gui/Gui.hxx>
#include <dr/gui/Event.hxx>
#include <dr/gui/Widget.hxx>

DR_GUI_NS_BEGIN


DR_EXPORT_MET bool Gui::dispatchEvent(Widget *widget, Event *evt)
{
	Event *altevt = NULL;
	int r;

	evt->target = widget;

	if (!widget->isFocused()) {
		switch (evt->type) {
		case Event::ET_ButtonDown:
			if (!widget->setFocus())
				return false;
			break;

		default:
			break;
		}
	}

	r = widget->handleEvent(evt);

	if (r >= 0)
		return true;

	switch (evt->type) {
	case Event::ET_Repaint:
		goto out_nohandle;

	case Event::ET_Close:
		goto out_nohandle;

	case Event::ET_KeyPress:
		{
			Event_Key *evtkey = (Event_Key *)evt;
			switch (evtkey->sym) {
			case Key::S_Tab:
				altevt = new(alloca(sizeof(Event_MoveFocus))) Event_MoveFocus(true);
				break;

			default:
				break;
			}
		}
		break;

	default:
		break;
	}

	if (altevt) {
		if ((r = widget->handleEvent(altevt)) > 0)
			goto out;
		if (r == 0)
			evt = NULL;
	}

	for (widget = widget->accParent(); widget; widget = widget->accParent()) {
		if (evt) {
			if ((r = widget->handleInherited(evt)) > 0)
				goto out;
			else if (r == 0) {
				evt = NULL;
				if (altevt == NULL)
					goto out;
			}
		}
		if (altevt) {
			if ((r = widget->handleInherited(altevt)) > 0)
				goto out;
		}
	}

out_nohandle:
	r = false;
out:
	DR_Assert(r == (int)false || r == (int)true);
	return r != 0;
}


DR_GUI_NS_END
