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

#ifndef dr__gui__def_gui__hxx__
# define dr__gui__def_gui__hxx__


#include <dr/def.hxx>

#ifdef COMPILING_drgui
# define DR_GUI_PUB	DR_EXPORT_CLS
# define DR_GUI_FUN	//DR_EXPORT_FUN
# define DR_GUI_MET	//DR_EXPORT_MET
# define DR_GUI_MTS	//DR_EXPORT_MTS
# define DR_GUI_DAT	//DR_EXPORT_DAT
# define DR_GUI_DTC	//DR_EXPORT_DTC
# define DR_GUI_DTS	//DR_EXPORT_DTS
# define DR_GUI_CLS	//DR_EXPORT_CLS
#else
# define DR_GUI_PUB	DR_IMPORT_CLS
# define DR_GUI_FUN	//DR_IMPORT_FUN
# define DR_GUI_MET	//DR_IMPORT_MET
# define DR_GUI_MTS	//DR_IMPORT_MTS
# define DR_GUI_DAT	//DR_IMPORT_DAT
# define DR_GUI_DTC	//DR_IMPORT_DTC
# define DR_GUI_DTS	//DR_IMPORT_DTS
# define DR_GUI_CLS	//DR_IMPORT_CLS
#endif

#define DR_GUI_NS_USE using namespace dr; using namespace dr::gui;
#define DR_GUI_NS dr::gui
#define DR_GUI_NSP(rest) dr::gui::rest
#define DR_GUI_NS_STR "dr::gui::"
#define DR_GUI_NS_STRP(rest) "dr::gui::" rest
#define DR_GUI_NS_BEGIN namespace dr \
{ \
	namespace gui \
	{
#define DR_GUI_NS_END }; };


DR_GUI_NS_BEGIN
DR_GUI_NS_END

#endif
