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

#include <dr/gui/x11/Visual_x11.hxx>

DR_GUI_NS_BEGIN


Visual_x11::Visual_x11(Gui_x11 *gui_, X11_Visual *visual):
	Visual(gui_)
{
	XVisualInfo vi_temp;
	int nvi;
	XVisualInfo *vi;

	vi_temp.visualid = visual->visualid;
	vi = XGetVisualInfo(gui_->display, VisualIDMask, &vi_temp, &nvi);

	visual_id = vi->visualid;

	switch (visual->c_class) {
	case TrueColor:
		type = VT_True;

		r_mask = vi->red_mask;
		g_mask = vi->green_mask;
		b_mask = vi->blue_mask;

		depth = vi->depth;

		setup_truecolor();
		break;

	default:
		DR_AssertTodo();
	}

	XFree(vi);
}


DR_GUI_NS_END
