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

#ifndef dr__app__def__hxx__
# define dr__app__def__hxx__

#include <dr/def.hxx>


#ifdef COMPILING_drapp
# define DR_APP_PUB	DR_EXPORT_CLS
# define DR_APP_FUN	DR_EXPORT_FUN
# define DR_APP_MET	DR_EXPORT_MET
# define DR_APP_MTS	DR_EXPORT_MTS
# define DR_APP_DAT	DR_EXPORT_DAT
# define DR_APP_DTC	DR_EXPORT_DTC
# define DR_APP_CLS	DR_EXPORT_CLS
#else
# define DR_APP_PUB	DR_IMPORT_CLS
# define DR_APP_FUN	DR_IMPORT_FUN
# define DR_APP_MET	DR_IMPORT_MET
# define DR_APP_MTS	DR_IMPORT_MTS
# define DR_APP_DAT	DR_IMPORT_DAT
# define DR_APP_DTC	DR_IMPORT_DTC
# define DR_APP_CLS	DR_IMPORT_CLS
#endif

#define DR_APP_NS_USE using namespace dr; using namespace dr::app;
#define DR_APP_NS dr::app
#define DR_APP_NSP(rest) dr::app::rest
#define DR_APP_NS_STR "dr::app::"
#define DR_APP_NS_STRP(rest) "dr::app::"rest
#define DR_APP_NS_BEGIN namespace dr \
{ \
	namespace app \
	{
#define DR_APP_NS_END }; };


DR_APP_NS_BEGIN
DR_APP_NS_END

#endif
