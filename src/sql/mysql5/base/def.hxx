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

#ifndef dr__sql__mysql5__def__hxx__
# define dr__sql__mysql5__def__hxx__

#include <dr/def.hxx>


#ifdef DR_COMPILING_VFS
# define DR_VFS_FUN DR_EXPORT_FUN
# define DR_VFS_MET DR_EXPORT_MET
# define DR_VFS_MTS DR_EXPORT_MTS
# define DR_VFS_DAT DR_EXPORT_DAT
# define DR_VFS_DTC DR_EXPORT_DTC
# define DR_VFS_CLS DR_EXPORT_CLS
#else
# define DR_VFS_FUN DR_IMPORT_FUN
# define DR_VFS_MET DR_IMPORT_MET
# define DR_VFS_MTS DR_IMPORT_MTS
# define DR_VFS_DAT DR_IMPORT_DAT
# define DR_VFS_DTC DR_IMPORT_DTC
# define DR_VFS_CLS DR_IMPORT_CLS
#endif

#define DR_SQL_MYSQL5_NS_USE using namespace dr; using namespace dr::sql; using namespace dr::sql::mysql5;
#define DR_SQL_MYSQL5_NS dr::sql::mysql5
#define DR_SQL_MYSQL5_NSP(rest) dr::sql::rest::mysql5
#define DR_SQL_MYSQL5_NS_STR "dr::sql::mysql5::"
#define DR_SQL_MYSQL5_NS_STRP(rest) "dr::sql::mysql5::"rest
#define DR_SQL_MYSQL5_NS_BEGIN namespace dr \
{ \
	namespace sql \
	{ \
		namespace mysql5 \
		{
#define DR_SQL_MYSQL5_NS_END }; }; };


DR_SQL_MYSQL5_NS_BEGIN
DR_SQL_MYSQL5_NS_END

#endif
