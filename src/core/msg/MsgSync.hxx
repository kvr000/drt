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

#ifndef dr__MsgSync__hxx__
# define dr__MsgSync__hxx__

#include <dr/Object.hxx>
#include <dr/Thread.hxx>

DR_NS_BEGIN


/**
 * Message synchronisation object
 *
 * Ancestor of all message synchronization objects
 */
class DR_PUB MsgSync: public Object
{
	DR_OBJECT_DECL_SIMPLE(MsgSync, Object);

public:
	static MsgSync *		createDefault(Thread *thr);

public:
	/* constructor */		MsgSync(Thread *thread_);

public:
	/**
	 * waits until another message arrives
	 *
	 * @param timeout_ns
	 * 	maximum time to wait for message (in nanoseconds)
	 *
	 * @return 0
	 * 	if the function timed out
	 * @return 1
	 * 	if there is new message
	 */
	virtual int			threadSleep(Sint64 timeout_ns) = 0;
	virtual void			threadWake() = 0;
	virtual bool			threadXchg(MsgSync *new_sync);

protected:
	Thread *			thread;
};


DR_NS_END

#endif
