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

#ifndef dr__types__hxx__
# error dont include arch files directly
#endif

DR_NS_BEGIN


#define DR_TYPE_BIGINT			long long


#define DR_INT8				char
#define DR_INT16			short
#define DR_INT32			int
#define DR_INT64			DR_TYPE_BIGINT
#define DR_INTPTR			int


#define DR_SIZE_CHAR			1
#define DR_SIZE_SHORT			2
#define DR_SIZE_INT			4
#define DR_SIZE_LONG			4
#define DR_SIZE_BIGINT			8
#define DR_SIZE_PTR			4

#define DR_BASE_CHAR			DR_INT8
#define DR_BASE_SHORT			DR_INT16
#define DR_BASE_INT			DR_INT32
#define DR_BASE_LONG			DR_INT32
#define DR_BASE_BIGINT			DR_INT64


#define DR_ALIGN_TYPE			int
#define DR_ALIGN			4

#define DR_ALIGN_ALLOC			16


DR_NS_END
