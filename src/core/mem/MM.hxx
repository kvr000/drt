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

#ifndef dr__MM__hxx__
# define dr__MM__hxx__

#include <stdlib.h>

#include <dr/def_core.hxx>

DR_NS_BEGIN


/**
 * Memory management
 */
class DR_PUB MM
{
public: /* dr allocator */
	/* sized allocations */
	static void *			allocS(size_t size);
	static void			freeS(void *p, size_t size);

	/* generic allocations, reference counted, reportable size */
	static void *			alloc(size_t size);
	static void *			realloc(void *p, size_t size);
	static void *			regrow(void *p, size_t size);
	static void			free(void *p);

public: /* native allocator */
	static void *			nat_allocS(size_t size);
	static void			nat_freeS(void *p, size_t size);

	/* generic allocations, reference counted, reportable size */
	static void *			nat_alloc(size_t size);
	static void *			nat_realloc(void *p, size_t size);
	static void *			nat_regrow(void *p, size_t size);
	static void			nat_free(void *p);

public:
	static void			flushThreadCache();

public:
	static void			enableThreadCache(int enable);

public:
	static void			initingThread();
	static void			closingThread();

public:
	static void			s_init();
};


DR_NS_END

#endif
