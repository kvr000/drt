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

#ifndef dr__serv__io__def__hxx__
# define dr__serv__io__def__hxx__

#include <dr/def.hxx>


#ifdef COMPILING_drservio
# define DR_SERV_IO_PUB	DR_EXPORT_CLS
# define DR_SERV_IO_FUN	DR_EXPORT_FUN
# define DR_SERV_IO_MET	DR_EXPORT_MET
# define DR_SERV_IO_MTS	DR_EXPORT_MTS
# define DR_SERV_IO_DAT	DR_EXPORT_DAT
# define DR_SERV_IO_DTC	DR_EXPORT_DTC
# define DR_SERV_IO_CLS	DR_EXPORT_CLS
#else
# define DR_SERV_IO_PUB	DR_IMPORT_CLS
# define DR_SERV_IO_FUN	DR_IMPORT_FUN
# define DR_SERV_IO_MET	DR_IMPORT_MET
# define DR_SERV_IO_MTS	DR_IMPORT_MTS
# define DR_SERV_IO_DAT	DR_IMPORT_DAT
# define DR_SERV_IO_DTC	DR_IMPORT_DTC
# define DR_SERV_IO_CLS	DR_IMPORT_CLS
#endif

#define DR_SERV_IO_NS_USE using namespace dr; using namespace dr::serv::io;
#define DR_SERV_IO_NS dr::serv::io
#define DR_SERV_IO_NSP(rest) dr::serv::io::rest
#define DR_SERV_IO_NS_STR "dr::serv::io::"
#define DR_SERV_IO_NS_STRP(rest) "dr::serv::io::"rest
#define DR_SERV_IO_NS_BEGIN namespace dr \
{ \
	namespace serv \
	{ \
		namespace io \
		{
#define DR_SERV_IO_NS_END }; }; };


DR_SERV_IO_NS_BEGIN
DR_SERV_IO_NS_END

#endif