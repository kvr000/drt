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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef DR_OS_UNIX
#include <signal.h>
#include <unistd.h>
#endif

#include <dr/types.hxx>
#include <dr/Atomic.hxx>
#include <dr/Lockp.hxx>
#include <dr/Hash.hxx>
#include <dr/Mutex.hxx>
#include <dr/Subsystem.hxx>
#include <dr/dev/MM_impl.hxx>
#include <dr/dev/MM_default.hxx>
#include <dr/dev/MM_native.hxx>

#ifdef DR_CXX_VC
# define getenv(str) NULL
#endif

DR_NS_BEGIN


void *(*MM_impl::impl_allocS)(size_t size) = MM_impl::init_allocS;
void (*MM_impl::impl_freeS)(void *p, size_t size) = MM_impl::init_freeS;
void *(*MM_impl::impl_alloc)(size_t size) = MM_impl::init_alloc;
void (*MM_impl::impl_free)(void *p) = MM_impl::init_free;
void *(*MM_impl::impl_realloc)(void *p, size_t size) = MM_impl::init_realloc;
void *(*MM_impl::impl_regrow)(void *p, size_t size) = MM_impl::init_regrow;
void (*MM_impl::impl_initingThread)() = MM_impl::init_initingThread;
void (*MM_impl::impl_closingThread)() = NULL;
void (*MM_impl::impl_flushThreadCache)() = NULL;

int MM_impl::debug_options = -1;

DR_EXPORT_MTS void MM_impl::setDebug(const char *opts)
{
	//debug_options = level;
}

void *MM_impl::init_allocS(size_t size)
{
	MM::s_init();
	return impl_allocS(size);
}

void MM_impl::init_freeS(void *m, size_t size)
{
	MM::s_init();
	return impl_freeS(m, size);
}

void *MM_impl::init_alloc(size_t size)
{
	MM::s_init();
	return impl_alloc(size);
}

void MM_impl::init_free(void *m)
{
	MM::s_init();
	return impl_free(m);
}

void *MM_impl::init_realloc(void *m, size_t size)
{
	MM::s_init();
	return impl_realloc(m, size);
}

void *MM_impl::init_regrow(void *m, size_t size)
{
	MM::s_init();
	return impl_regrow(m, size);
}

void MM_impl::init_initingThread()
{
	MM::s_init();
	if (impl_initingThread)
		impl_initingThread();
}

void MM_impl::setImpl(void *(*impl_allocS_)(size_t size), void (*impl_freeS_)(void *m, size_t size), void *(*impl_alloc_)(size_t size), void (*impl_free_)(void *m), void *(*impl_realloc_)(void *m, size_t size), void *(*impl_regrow_)(void *m, size_t size))
{
	impl_allocS = impl_allocS_;
	impl_freeS = impl_freeS_;
	impl_alloc = impl_alloc_;
	impl_free = impl_free_;
	impl_realloc = impl_realloc_;
	impl_regrow = impl_regrow_;
}

void MM_impl::setThreadImpl(void (*impl_initingThread_)(), void (*impl_closingThread_)(), void (*impl_flushThreadCache_)())
{
	impl_initingThread = impl_initingThread_;
	impl_closingThread = impl_closingThread_;
	impl_flushThreadCache = impl_flushThreadCache_;
}

void MM::s_init()
{
	int set_level = MM_impl::MMD_THREAD;
	void (*method)(const char *opts, int level) = NULL;
	const char *opts;
	if (MM_impl::debug_options >= 0)
		return;
	MM_impl::debug_options = 0;
	if ((opts = getenv("DRO_MM")) != NULL) {
		while (*opts != '\0') {
			switch (*opts) {
			case 'H':
				set_level &= ~MM_impl::MMD_THREAD;
				break;

			case '.':
				set_level |= MM_impl::MMD_CLEAN|MM_impl::MMD_BOUND|MM_impl::MMD_LOG|MM_impl::MMD_BTRACE|MM_impl::MMD_REMEMBER|MM_impl::MMD_PENDING|MM_impl::MMD_TEST;
				break;

			case 'c':
				set_level |= MM_impl::MMD_CLEAN;
				break;

			case 'l':
				set_level |= MM_impl::MMD_LOG;
				break;

			case 'b':
				set_level |= MM_impl::MMD_BTRACE;
				break;

			case 'r':
				set_level |= MM_impl::MMD_REMEMBER;
				break;

			case 'p':
				set_level |= MM_impl::MMD_PENDING;
				break;

			case 't':
				set_level |= MM_impl::MMD_TEST|MM_impl::MMD_CLEAN;
				break;

			case 'm':
				set_level |= MM_impl::MMD_BOUND|MM_impl::MMD_CLEAN;
				break;

			case '_':
				set_level = 0;
				method = &MM_native::setup;
				break;

			case 'Z':
#ifdef DR_OS_UNIX
				raise(SIGSTOP);
#endif
				break;

			default:
				/* ignore */
				break;
			}
			opts++;
		}
	}
	MM_impl::impl_initingThread = NULL;
	if (!method)
		method = &MM_default::setup;
	method(opts, set_level);
}

#define ZERO_PTR ((void *)1024)

DR_EXPORT_MTS void MM::enableThreadCache(int enable)
{
	if (enable)
		MM_impl::debug_options |= MM_impl::MMD_THREAD;
}

DR_EXPORT_MTS void *MM::allocS(size_t size)
{
	if (size == 0)
		return ZERO_PTR;
	return MM_impl::impl_allocS(size);
}

DR_EXPORT_MTS void MM::freeS(void *p, size_t size)
{
	if (size == 0) {
		DR_Assert(p == ZERO_PTR);
		return;
	}
	MM_impl::impl_freeS(p, size);
}

DR_EXPORT_MTS void *MM::alloc(size_t size)
{
	if (size == 0) {
		return ZERO_PTR;
	}
	return MM_impl::impl_alloc(size);
}

DR_EXPORT_MTS void MM::free(void *m_)
{
	if (m_ == ZERO_PTR)
		return;
	if (m_ == NULL)
		return;
	MM_impl::impl_free(m_);
}

DR_EXPORT_MTS void *MM::realloc(void *m_, size_t size)
{
	if (m_ == ZERO_PTR || m_ == NULL) {
		if (size == 0)
			return ZERO_PTR;
		return alloc(size);
	}
	if (size == 0) {
		MM_impl::impl_free(m_);
		return ZERO_PTR;
	}
	return MM_impl::impl_realloc(m_, size);
}

DR_EXPORT_MTS void *MM::regrow(void *m_, size_t size)
{
	if (m_ == ZERO_PTR || m_ == NULL) {
		if (size == 0)
			return ZERO_PTR;
		return alloc(size);
	}
	if (size == 0)
		size = 1;
	return MM_impl::impl_regrow(m_, size);
}

DR_EXPORT_MTS void MM::flushThreadCache()
{
	if (MM_impl::impl_flushThreadCache)
		MM_impl::impl_flushThreadCache();
}

DR_EXPORT_MTS void MM::initingThread()
{
	if (MM_impl::impl_initingThread)
		MM_impl::impl_initingThread();
}

DR_EXPORT_MTS void MM::closingThread()
{
	if (MM_impl::impl_closingThread)
		MM_impl::impl_closingThread();
}


DR_NS_END
