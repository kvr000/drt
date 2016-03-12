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

#ifndef dr__MM_impl__hxx__
# define dr__MM_impl__hxx__

DR_NS_BEGIN


/**
 * Memory management implementations support
 */
class DR_PUB MM_impl
{
public:
	enum
	{
		MMD_THREAD	= 0x1,
		MMD_CLEAN	= 0x010000,
		MMD_LOG		= 0x020000,
		MMD_BTRACE	= 0x040000,
		MMD_REMEMBER	= 0x080000,
		MMD_PENDING	= 0x100000,
		MMD_TEST	= 0x200000,
		MMD_BOUND	= 0x400000,
		MMD_STOPS	= 0x800000,
	};

public: /* debug interface, implementation, ... */
	static void			setDebug(const char *opts);
	static void			setImpl(void *(*impl_allocS_)(size_t size), void (*impl_freeS_)(void *m, size_t size), void *(*impl_alloc_)(size_t size), void (*impl_free_)(void *m), void *(*impl_realloc_)(void *m, size_t size), void *(*impl_regrow_)(void *m, size_t size));
	static void			setThreadImpl(void (*impl_initingThread_)(), void (*impl_closingThread_)(), void (*impl_flushThreadCache)());

public: /* implementation callbacks */
	static void *			(*impl_allocS)(size_t size);
	static void			(*impl_freeS)(void *p, size_t size);
	static void *			(*impl_alloc)(size_t size);
	static void			(*impl_free)(void *p);
	static void *			(*impl_realloc)(void *p, size_t size);
	static void *			(*impl_regrow)(void *p, size_t size);
	static void			(*impl_flushThreadCache)();
	static void			(*impl_initingThread)();
	static void			(*impl_closingThread)();

	/** debug options */
	static int			debug_options;
	static void *			debug_stops[16];
	static size_t			debug_stops_length;
	static Refcnt			debug_stop_seq;
	static Refcnt			debug_current_seq;

protected: /* intialization functions */
	static void *			init_allocS(size_t size);
	static void			init_freeS(void *m, size_t size);
	static void *			init_alloc(size_t size);
	static void			init_free(void *m);
	static void *			init_realloc(void *m, size_t size);
	static void *			init_regrow(void *m, size_t size);
	static void			init_initingThread();
};


DR_NS_END

#endif
