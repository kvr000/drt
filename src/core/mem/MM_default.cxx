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
//#include <unistd.h>

#include <dr/types.hxx>
#include <dr/Atomic.hxx>
#include <dr/Lockp.hxx>
#include <dr/Hash.hxx>
#include <dr/Mutex.hxx>
#include <dr/Subsystem.hxx>
#include <dr/MM.hxx>
#include <dr/MemExcept.hxx>
#include <dr/dev/Thread_impl.hxx>
#include <dr/dev/MM_impl.hxx>

#include <dr/dev/MM_default.hxx>

#ifdef DR_CXX_VC
# define strdup _strdup
#endif

DR_NS_BEGIN


const unsigned char MM_default::guard_beg[32] = { 0xcb, 0xcb, 0xcb, 0xcb, 0xcb, 0xcb, 0xcb, 0xcb, 0xcb, 0xcb, 0xcb, 0xcb, 0xcb, 0xcb, 0xcb, 0xcb, 0xcb, 0xcb, 0xcb, 0xcb, 0xcb, 0xcb, 0xcb, 0xcb, 0xcb, 0xcb, 0xcb, 0xcb, 0xcb, 0xcb, 0xcb, 0xcb };
const unsigned char MM_default::guard_fin[48] = { 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf };


void MM_default::setup(const char *opts, int set_level)
{
	MM_impl::setImpl(&impl_alloc, (void (*)(void *m, size_t size))&impl_free, &impl_alloc, &impl_free, &impl_regrow, &impl_regrow);
	MM_impl::setThreadImpl(NULL, &impl_closingThread, &impl_closingThread);

	Fatal::s_init();
	Subsystem::s_init();

	instance.string_cache = new StringCache;
	instance.remember_hash = new RememberHash;
	instance.remember_mutex = Mutex::create();

	MM_impl::debug_options = set_level;

	Lockp::s_init();
}

MM_default::MM_default()
{
	MM::s_init();
}

MM_default::~MM_default()
{
	if (MM_impl::debug_options&MM_impl::MMD_REMEMBER) {
		RememberHash::Node *n;
		Fatal::plog("pending memory (%ld):\n", (long)remember_hash->count());

		for (n = remember_hash->iterFirst(); n; n = remember_hash->iterNext(n)) {
			Fatal::plog("block %p:\n", (char *)n->k+sizeof(SintPtr));
			Fatal::pdata(n->v.bt, strlen(n->v.bt));
			Fatal::pdata("\n", 1);
		}
	}
}

void *MM_default::impl_alloc(size_t size_)
{
	size_t size = size_+sizeof(SintPtr);
	void *out;

	if (MM_impl::debug_options&MM_impl::MMD_BOUND)
		size = ((size+DR_ALIGN_ALLOC-1)&~((size_t)DR_ALIGN_ALLOC-1))+64;

	if (size > 1024) {
		if (!(out = (char *)::malloc(size+DR_ALIGN_ALLOC)+DR_ALIGN_ALLOC-sizeof(SintPtr)))
			MemExcept::throwInstance(size+DR_ALIGN_ALLOC);
	}
	else {
		char *add = NULL;
		size_t alloc;
		void *volatile *queue;
		void *remain = NULL;
		size_t remain_size = 0;

		if (MM_impl::debug_options&MM_impl::MMD_THREAD) {
			Thread_impl *thr = Thread_impl::currentImpl();
			if (0) {
				queue = NULL;
				alloc = 0;
			}
#if DR_SIZE_PTR < DR_ALIGN_ALLOC
			else if (size <= sizeof(void *)) {
				queue = &thr->thr_alsmall_ptr1[0];
				alloc = 512-4*sizeof(void *);
				size = sizeof(void *);
			}
#if 2*DR_SIZE_PTR < DR_ALIGN_ALLOC
			else if (size <= 2*sizeof(void *)) {
				queue = &thr->thr_alsmall_ptr2[0];
				alloc = 512-4*sizeof(void *);
				size = 2*sizeof(void *);
			}
#if 4*DR_SIZE_PTR < DR_ALIGN_ALLOC
# error TODO
#endif // 4*DR_SIZE_PTR < DR_ALIGN_ALLOC
#endif // 2*DR_SIZE_PTR < DR_ALIGN_ALLOC
#endif // DR_SIZE_PTR < DR_ALIGN_ALLOC
			else if ((size-1)/DR_ALIGN_ALLOC < sizeof(thr->thr_alsmall_small)/sizeof(thr->thr_alsmall_small[0])) {
				queue = &thr->thr_alsmall_small[(size-1)/DR_ALIGN_ALLOC][0];
				alloc = 1024-4*sizeof(void *);
				size = (size+DR_ALIGN_ALLOC-1)&~((size_t)DR_ALIGN_ALLOC-1);
			}
			else {
				size = (size+DR_ALIGN_ALLOC-1)&~((size_t)DR_ALIGN_ALLOC-1);
				if (size <= 192) {
					queue = &thr->thr_alsmall_bigger[0][0];
					size = 192;
				}
				else if (size <= 256) {
					queue = &thr->thr_alsmall_bigger[1][0];
					size = 256;
				}
				else if (size <= 384) {
					queue = &thr->thr_alsmall_bigger[2][0];
					size = 384;
				}
				else if (size <= 512) {
					queue = &thr->thr_alsmall_bigger[3][0];
					size = 512;
				}
				else if (size <= 768) {
					queue = &thr->thr_alsmall_bigger[4][0];
					size = 768;
				}
				else {
					DR_Assert(size <= 1024);
					queue = &thr->thr_alsmall_bigger[5][0];
					size = 1024;
				}
				alloc = 4096-4*sizeof(void *);
			}
			if (*queue != NULL) {
				out = *queue;
				*queue = *(void **)out;
				goto has_out;
			}
		}

		if (0) {
			queue = NULL;
			alloc = 0;
		}
#if DR_SIZE_PTR < DR_ALIGN_ALLOC
		else if (size <= sizeof(void *)) {
			queue = &instance.alsmall_ptr;
			alloc = 512-4*sizeof(void *);
			size = sizeof(void *);
		}
#if 2*DR_SIZE_PTR < DR_ALIGN_ALLOC
		else if (size <= 2*sizeof(void *)) {
			queue = &instance.alsmall_ptr2;
			alloc = 512-4*sizeof(void *);
			size = 2*sizeof(void *);
		}
#if 4*DR_SIZE_PTR < DR_ALIGN_ALLOC
# error TODO
#endif // 4*DR_SIZE_PTR < DR_ALIGN_ALLOC
#endif // 2*DR_SIZE_PTR < DR_ALIGN_ALLOC
#endif // DR_SIZE_PTR < DR_ALIGN_ALLOC
		else if ((size-1)/DR_ALIGN_ALLOC < sizeof(instance.alsmall_small)/sizeof(instance.alsmall_small[0])) {
			queue = &instance.alsmall_small[(size-1)/DR_ALIGN_ALLOC];
			alloc = 1024-4*sizeof(void *);
			size = (size+DR_ALIGN_ALLOC-1)&~((size_t)DR_ALIGN_ALLOC-1);
		}
		else {
			size = (size+DR_ALIGN_ALLOC-1)&~((size_t)DR_ALIGN_ALLOC-1);
			if (size <= 192) {
				queue = &instance.alsmall_bigger[0];
				size = 192;
			}
			else if (size <= 256) {
				queue = &instance.alsmall_bigger[1];
				size = 256;
			}
			else if (size <= 384) {
				queue = &instance.alsmall_bigger[2];
				size = 384;
			}
			else if (size <= 512) {
				queue = &instance.alsmall_bigger[3];
				size = 512;
			}
			else if (size <= 768) {
				queue = &instance.alsmall_bigger[4];
				size = 768;
			}
			else {
				DR_Assert(size <= 1024);
				queue = &instance.alsmall_bigger[5];
				size = 1024;
			}
			alloc = 4096-4*sizeof(void *);
		}
		if (*queue == NULL) {
go_alloc:
			char *end;
			char *cur;

			if (!(add = (char *)::malloc(alloc)))
				MemExcept::throwInstance(alloc);
			end = add+alloc;
			add += DR_ALIGN_ALLOC-sizeof(SintPtr);

			cur = add;
			for (;;) {
				char *next = cur+size;
				if (next <= end) {
					*(void **)cur = next;
					cur = next;
				}
				else {
					remain = cur;
					remain_size = end-cur;
					*(void **)(cur-size) = NULL;
					break;
				}
			}
			if ((out = Lockp::i_lock(queue)) == NULL) {
				out = add;
				add = NULL;
			}
		}
		else {
			if ((out = Lockp::i_lock(queue)) == NULL) {
				Lockp::i_unlock(queue, out);
				goto go_alloc;
			}
		}
		Lockp::i_unlock(queue, *(void **)out);
		if (add) {
			add -= DR_ALIGN_ALLOC-sizeof(SintPtr);
			::free(add);
		}
		else {
			if (remain_size > 0) {
				if (0) {
				}
#if DR_SIZE_PTR < DR_ALIGN_ALLOC
				else if (remain_size < 2*sizeof(void *)) {
					queue = &instance.alsmall_ptr;
				}
#if 2*DR_SIZE_PTR < DR_ALIGN_ALLOC
				else if (remain_size < 4*sizeof(void *)) {
					queue = &instance.alsmall_ptr2;
				}
#if 4*DR_SIZE_PTR < DR_ALIGN_ALLOC
# error TODO
#endif // 4*DR_SIZE_PTR < DR_ALIGN_ALLOC
#endif // 2*DR_SIZE_PTR < DR_ALIGN_ALLOC
#endif // DR_SIZE_PTR < DR_ALIGN_ALLOC
				else if ((remain_size)/DR_ALIGN_ALLOC <= sizeof(instance.alsmall_small)/sizeof(instance.alsmall_small[0])) {
					queue = &instance.alsmall_small[(remain_size-DR_ALIGN_ALLOC)/DR_ALIGN_ALLOC];
				}
				else if (remain_size < 192)
					queue = &instance.alsmall_small[sizeof(instance.alsmall_small)/sizeof(instance.alsmall_small[0])-1];
				else if (remain_size < 256)
					queue = &instance.alsmall_bigger[0];
				else if (remain_size < 384)
					queue = &instance.alsmall_bigger[1];
				else if (remain_size < 512)
					queue = &instance.alsmall_bigger[2];
				else if (remain_size < 768)
					queue = &instance.alsmall_bigger[3];
				else if (remain_size < 1024)
					queue = &instance.alsmall_bigger[4];
				else
					DR_AssertInvalid();

				*(void **)remain = Lockp::i_lock(queue);
				Lockp::i_unlock(queue, remain);
			}
		}
	}

has_out:
	if ((MM_impl::debug_options&~(MM_impl::MMD_THREAD)) != 0) {
		Atomic::inc(&MM_impl::debug_current_seq);
		if (MM_impl::debug_options&MM_impl::MMD_CLEAN)
			memset(out, 0xcd, size);
		if (MM_impl::debug_options&MM_impl::MMD_BOUND) {
			memset(out, 0xcb, 32);
			memset((char *)out+32+sizeof(SintPtr)+size_, 0xcf, size-size_-sizeof(SintPtr)-32);
			out = (char *)out+32;
		}
		if (MM_impl::debug_options&(MM_impl::MMD_LOG|MM_impl::MMD_BTRACE)) {
			char buf[128];
			sprintf(buf, "allocated block %p (%ld)\n", (char *)out+sizeof(SintPtr), (long)size_);
			if (MM_impl::debug_options&(MM_impl::MMD_BTRACE))
				Fatal::printBacktrace(buf);
			else
				Fatal::log(buf);
		}
		if (MM_impl::debug_options&MM_impl::MMD_TEST) {
			if (!(MM_impl::debug_options&MM_impl::MMD_BOUND)) {
				size_t i;
				for (i = size_+sizeof(SintPtr); i < size; i++)
					((unsigned char *)out)[i] = 0xcd;
			}
		}
		if (MM_impl::debug_options&MM_impl::MMD_REMEMBER) {
			size_t btsize;
			bool created;
			RememberHash::Node *hc;
			StringCache::Node *sc;
			instance.remember_mutex->lock();
			hc = instance.remember_hash->create(out, &created);
			if (!created)
				DR_AssertMsg("returning already referenced memory");
			while ((btsize = Fatal::getBacktrace(instance.str, instance.str_len)) >= instance.str_len) {
				instance.str = (char *)::realloc(instance.str, btsize+32);
				instance.str_len = btsize+32;
			}
			sc = instance.string_cache->create(instance.str, &created);
			if (created)
				sc->k = strdup(instance.str);
			hc->v.bt = sc->k;
			hc->v.size = size_;
			instance.remember_mutex->unlock();
		}
		if (MM_impl::debug_options&MM_impl::MMD_PENDING) {
			char buf[256];
			sprintf(buf, "allocated block %p (%ld), pending %ld\n", (char *)out+sizeof(SintPtr), (long)size_, (long)instance.remember_hash->count());
			Fatal::pdata(buf, strlen(buf));
		}
		if (MM_impl::debug_options&MM_impl::MMD_STOPS) {
			if (MM_impl::debug_current_seq == MM_impl::debug_stop_seq) {
				Fatal::debugStop("memory sequence breakpoint\n");
			}
			for (size_t i = 0; i < MM_impl::debug_stops_length; i++) {
				if ((char *)out+sizeof(SintPtr) == MM_impl::debug_stops[i]) {
					Fatal::debugStop("memory block breakpoint\n");
				}
			}
		}
	}
	((SintPtr *)out)[0] = size_;
	return (char *)out+sizeof(SintPtr);
}

void MM_default::impl_free(void *m)
{
	size_t size_ = ((SintPtr *)m)[-1];
	size_t size = size_+sizeof(SintPtr);

	m = (char *)m-sizeof(SintPtr);

	if ((MM_impl::debug_options&~(MM_impl::MMD_THREAD)) != 0) {
		if (MM_impl::debug_options&MM_impl::MMD_BOUND)
			size = ((size+DR_ALIGN_ALLOC-1)&~((size_t)DR_ALIGN_ALLOC-1))+64;
		if (MM_impl::debug_options&(MM_impl::MMD_LOG|MM_impl::MMD_BTRACE)) {
			char buf[128];
			sprintf(buf, "freeing block %p (%ld)\n", (char *)m+sizeof(SintPtr), (long)size_);
			if (MM_impl::debug_options&(MM_impl::MMD_BTRACE))
				Fatal::printBacktrace(buf);
			else
				Fatal::log(buf);
		}
		if (MM_impl::debug_options&MM_impl::MMD_REMEMBER) {
			RememberHash::Node *rmem;
			instance.remember_mutex->lock();
			if (!(rmem = instance.remember_hash->find(m))) {
				DR_AssertMsg("freeing not allocated memory");
			}
			DR_Assert(rmem->v.size == size_);
			instance.remember_hash->remove(m);
			instance.remember_mutex->unlock();
		}
		if (MM_impl::debug_options&MM_impl::MMD_PENDING) {
			char buf[256];
			sprintf(buf, "freeing block %p (%ld), pending %ld\n", (char *)m+sizeof(SintPtr), (long)size_, (long)instance.remember_hash->count());
			Fatal::plog(buf);
		}
		if (MM_impl::debug_options&MM_impl::MMD_TEST) {
			if (!(MM_impl::debug_options&MM_impl::MMD_BOUND)) {
				size_t i;
				for (i = size_+sizeof(SintPtr); i < size; i++)
					DR_Assert(((unsigned char *)m)[i] == 0xcd);
			}
		}
		if (MM_impl::debug_options&MM_impl::MMD_BOUND) {
			m = (char *)m-32;
			if (memcmp(m, guard_beg, 32) != 0) {
				char buf[256];
				sprintf(buf, "block %p overwrote guard_beg block", (char *)m+32+sizeof(SintPtr));
				DR_AssertMsg(buf);
			}
			if (memcmp((char *)m+32+sizeof(SintPtr)+size_, guard_fin, size-size_-sizeof(SintPtr)-32) != 0) {
				char buf[256];
				sprintf(buf, "block %p overwrote guard_fin block", (char *)m+32+sizeof(SintPtr));
				DR_AssertMsg(buf);
			}
		}
		if (MM_impl::debug_options&MM_impl::MMD_CLEAN)
			memset(m, 0xcd, size);
	}
	if (size > 1024) {
		::free((char *)m+sizeof(SintPtr)-DR_ALIGN_ALLOC);
	}
	else {
		void *volatile *queue;
		void *prev;
		if (MM_impl::debug_options&MM_impl::MMD_THREAD) {
			Thread_impl *thr = Thread_impl::currentImpl();
			if (0) {
			}
#if DR_SIZE_PTR < DR_ALIGN_ALLOC
			else if (size <= sizeof(void *)) {
				queue = &thr->thr_alsmall_ptr1[0];
			}
#if 2*DR_SIZE_PTR < DR_ALIGN_ALLOC
			else if (size <= 2*sizeof(void *)) {
				queue = &thr->thr_alsmall_ptr2[0];
			}
#if 4*DR_SIZE_PTR < DR_ALIGN_ALLOC
# error TODO
#endif // 4*DR_SIZE_PTR < DR_ALIGN_ALLOC
#endif // 2*DR_SIZE_PTR < DR_ALIGN_ALLOC
#endif // DR_SIZE_PTR < DR_ALIGN_ALLOC
			else if ((size-1)/DR_ALIGN_ALLOC < sizeof(thr->thr_alsmall_small)/sizeof(thr->thr_alsmall_small[0])) {
				queue = &thr->thr_alsmall_small[(size-1)/DR_ALIGN_ALLOC][0];
			}
			else {
				if (size <= 192) {
					queue = &thr->thr_alsmall_bigger[0][0];
				}
				else if (size <= 256) {
					queue = &thr->thr_alsmall_bigger[1][0];
				}
				else if (size <= 384) {
					queue = &thr->thr_alsmall_bigger[2][0];
				}
				else if (size <= 512) {
					queue = &thr->thr_alsmall_bigger[3][0];
				}
				else if (size <= 768) {
					queue = &thr->thr_alsmall_bigger[4][0];
				}
				else {
					DR_Assert(size <= 1024);
					queue = &thr->thr_alsmall_bigger[5][0];
				}
			}
			if (*queue == NULL)
				*(queue+1) = m;
			*(void **)m = *queue;
			*queue = m;
			return;
		}
		if (0) {
		}
#if DR_SIZE_PTR < DR_ALIGN_ALLOC
		else if (size <= sizeof(void *)) {
			queue = &instance.alsmall_ptr;
		}
#if 2*DR_SIZE_PTR < DR_ALIGN_ALLOC
		else if (size <= 2*sizeof(void *)) {
			queue = &instance.alsmall_ptr2;
		}
#if 4*DR_SIZE_PTR < DR_ALIGN_ALLOC
# error TODO
#endif // 4*DR_SIZE_PTR < DR_ALIGN_ALLOC
#endif // 2*DR_SIZE_PTR < DR_ALIGN_ALLOC
#endif // DR_SIZE_PTR < DR_ALIGN_ALLOC
		else if ((size-1)/DR_ALIGN_ALLOC < sizeof(instance.alsmall_small)/sizeof(instance.alsmall_small[0])) {
			queue = &instance.alsmall_small[(size-1)/DR_ALIGN_ALLOC];
		}
		else {
			if (size <= 192) {
				queue = &instance.alsmall_bigger[0];
			}
			else if (size <= 256) {
				queue = &instance.alsmall_bigger[1];
			}
			else if (size <= 384) {
				queue = &instance.alsmall_bigger[2];
			}
			else if (size <= 512) {
				queue = &instance.alsmall_bigger[3];
			}
			else if (size <= 768) {
				queue = &instance.alsmall_bigger[4];
			}
			else {
				DR_Assert(size <= 1024);
				queue = &instance.alsmall_bigger[5];
			}
		}
		prev = Lockp::i_lock(queue);
		*(void **)m = prev;
		Lockp::i_unlock(queue, m);
	}
}

void *MM_default::impl_regrow(void *m, size_t newsize)
{
	void *nm;
	size_t oldsize = ((SintPtr *)m)[-1];

	if (oldsize >= newsize)
		return m;
	if ((nm = impl_alloc(newsize)) == NULL)
		return NULL;
	memcpy(nm, m, oldsize);
	impl_free(m);
	return nm;
}

size_t MM_default::impl_size(void *m)
{
	return ((SintPtr *)m)[-1];
}

DR_RINLINE void MM_default::insertGlobalCache(void *volatile *queue, void *first, void *last)
{
	if (first != NULL) {
		void *prev = Lockp::i_lock(queue);
		*(void **)last = prev;
		Lockp::i_unlock(queue, first);
	}
}

void MM_default::impl_closingThread()
{
	Thread_impl *thr = Thread_impl::currentImpl();
#if DR_SIZE_PTR < DR_ALIGN_ALLOC
	insertGlobalCache(&instance.alsmall_ptr, thr->thr_alsmall_ptr1[0], thr->thr_alsmall_ptr1[1]); thr->thr_alsmall_ptr1[0] = NULL;
#if 2*DR_SIZE_PTR < DR_ALIGN_ALLOC
	insertGlobalCache(&instance.alsmall_ptr2, thr->thr_alsmall_ptr2[0], thr->thr_alsmall_ptr2[1]); thr->thr_alsmall_ptr2[0] = NULL;
#if 4*DR_SIZE_PTR < DR_ALIGN_ALLOC
# error TODO
#endif // 4*DR_SIZE_PTR < DR_ALIGN_ALLOC
#endif // 2*DR_SIZE_PTR < DR_ALIGN_ALLOC
#endif // DR_SIZE_PTR < DR_ALIGN_ALLOC
	for (size_t i = 0; i < sizeof(instance.alsmall_small)/sizeof(instance.alsmall_small[0]); i++) {
		insertGlobalCache(&instance.alsmall_small[i], thr->thr_alsmall_small[i][0], thr->thr_alsmall_small[i][1]); thr->thr_alsmall_small[i][0] = NULL;
	}
	for (size_t i = 0; i < 6; i++) {
		insertGlobalCache(&instance.alsmall_bigger[i], thr->thr_alsmall_bigger[i][0], thr->thr_alsmall_bigger[i][1]); thr->thr_alsmall_bigger[i][0] = NULL;
	}
}


MM_default MM_default::instance;


DR_NS_END
