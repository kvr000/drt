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

#ifndef dr__Thread_impl_hxx__
# define dr__Thread_impl_hxx__

#include <dr/Thread.hxx>

DR_NS_BEGIN


/**
 * thread platform dependent implementation
 */
class DR_PUB Thread_impl: public Shared
{
public:
	virtual void			wait() = 0;

protected:
	DR_RINLINE			Thread_impl(Thread *main_)		: main(main_), throw_count(-1) { memset(thr_alsmall_ptr1, 0, sizeof(thr_alsmall_ptr1)); memset(thr_alsmall_ptr2, 0, sizeof(thr_alsmall_ptr2)); memset(thr_alsmall_ptr4, 0, sizeof(thr_alsmall_ptr4)); memset(thr_alsmall_small, 0, sizeof(thr_alsmall_small)); memset(thr_alsmall_bigger, 0, sizeof(thr_alsmall_bigger)); }
	DR_RINLINE			Thread_impl(const None &)		: throw_count(-1) {}
	virtual				~Thread_impl();
	DR_RINLINE void			runMain()				{ main->exit_value = main->run(); }
	DR_RINLINE void			setImpl()				{ main->impl = this; }
	DR_RINLINE void			resetImpl()				{ main->impl = NULL; }
	DR_RINLINE void			setThrown(Throwable *val /*loose*/)	{ main->thrown_value = val; }

	virtual void			mainDestroy()				{ unref(); }

	DR_RINLINE void			threadPreDestroy()			{ MM::closingThread(); }

protected:
	Thread *			main;

public:
	void *				thr_alsmall_ptr1[2];
	void *				thr_alsmall_ptr2[2];
	void *				thr_alsmall_ptr4[2];
	void *				thr_alsmall_small[128/DR_ALIGN_ALLOC][2];
	void *				thr_alsmall_bigger[8][2];

public:
	static Thread *			preinitMainThread();
	static Thread *			postinitMainThread(Thread *preinited);

public:
	/**
	 * gets the current thread implementation object
	 *
	 * @note
	 * 	the method doesn't increment the object count
	 */
	static Thread_impl *		currentImpl();

protected: // runtime context
	int				throw_count;

private:
	friend class Thread;
	friend class Throwable;
};


DR_NS_END

#endif
