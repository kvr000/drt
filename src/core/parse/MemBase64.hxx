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

#ifndef dr__MemBase64__hxx__
# define dr__MemBase64__hxx__

#include <dr/Blob.hxx>
#include <dr/BString.hxx>

#include <dr/Object.hxx>

DR_NS_BEGIN


/**
 * Binary Parser
 * 
 * the class is intended to convert memory into basic types
 */
class DR_PUB MemBase64: public Object
{
	DR_OBJECT_DECL(MemBase64, Object);

public:
	DR_CONSTRUCT			MemBase64(const void *data, size_t length);
	DR_CONSTRUCT			MemBase64(const Blob *data);

public:
	virtual				~MemBase64();

	static Blob			decodeBase64(const BString &data);
	static ssize_t			decodeBase64(void *dest, const char *str, size_t str_length);
	static BString			encodeBase64(const Blob &data);
	static ssize_t			encodeBase64(char *dest, const void *data, size_t data_length);
};


DR_NS_END

#endif
