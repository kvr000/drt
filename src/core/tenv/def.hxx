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

#ifndef dr__tenv__def__hxx__
# define dr__tenv__def__hxx__


#include <dr/def.hxx>

#ifdef COMPILING_drtenv
# define DR_TENV_PUB	DR_EXPORT_CLS
# define DR_TENV_FUN	DR_EXPORT_FUN
# define DR_TENV_MET	DR_EXPORT_MET
# define DR_TENV_MTS	DR_EXPORT_MTS
# define DR_TENV_DAT	DR_EXPORT_DAT
# define DR_TENV_DTC	DR_EXPORT_DTC
# define DR_TENV_CLS	DR_EXPORT_CLS
#else
# define DR_TENV_PUB	DR_IMPORT_CLS
# define DR_TENV_FUN	DR_IMPORT_FUN
# define DR_TENV_MET	DR_IMPORT_MET
# define DR_TENV_MTS	DR_IMPORT_MTS
# define DR_TENV_DAT	DR_IMPORT_DAT
# define DR_TENV_DTC	DR_IMPORT_DTC
# define DR_TENV_CLS	DR_IMPORT_CLS
#endif

#define DR_TENV_NS_USE using namespace dr; using namespace dr::tenv;
#define DR_TENV_NS dr::tenv::
#define DR_TENV_NSP(rest) dr::tenv::rest
#define DR_TENV_NS_STR "dr::tenv::"
#define DR_TENV_NS_STRP(rest) "dr::tenv::"rest
#define DR_TENV_NS_BEGIN namespace dr \
{ \
	namespace tenv \
	{
#define DR_TENV_NS_END }; };


DR_TENV_NS_BEGIN
DR_TENV_NS_END


#endif
