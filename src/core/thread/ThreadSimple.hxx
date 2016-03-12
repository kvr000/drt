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

#ifndef dr__ThreadSimple__hxx__
# define dr__ThreadSimple__hxx__

#include <dr/Thread.hxx>

DR_NS_BEGIN


/**
 * Simple thread implementation
 *
 * This object provides easy to use approach for creating the threads. The
 * thread is created and run by single call
 */
class DR_PUB ThreadSimple: public Thread
{
	DR_OBJECT_DECL_SIMPLE(ThreadSimple, Thread);

public:
	static ThreadSimple *		go(void *(*func)(void *), void *arg);
	DR_MINLINE static ThreadSimple *go(const Eslot0<void> &run_)	{ return p_go(run_.aAdd<void *>().rIgnore<void *>(), Eslot1<void, void *>((Eimpl_g *)NULL), NULL); }
	template <typename D>
	DR_MINLINE static ThreadSimple *go(const Eslot1<void *, D *> &run_, const Eslot1<void, D *> &destroy, D *data) { return p_go(run_, destroy, data); }

protected:
	virtual void *			run();

	DR_RINLINE			ThreadSimple(const Eslot1<void *, void *> &run_, const Eslot1<void, void *> &destroy_, void *data_): run_slot(run_), destroy_slot(destroy_), data(data_) {}

protected:
	static ThreadSimple *		p_go(const Eslot1<void *, void *> &run, const Eslot1<void, void *> &destroy, void *data);

protected:
	Eslot1<void *, void *>		run_slot;
	Eslot1<void, void *>		destroy_slot;

	void *				data;
};


DR_NS_END

#endif
