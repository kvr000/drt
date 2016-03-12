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

#ifndef dr__MsgSync_wnt__hxx__
# define dr__MsgSync_wnt__hxx__

#include <dr/MsgSync.hxx>
#include <dr/MsgSync_Socket.hxx>
#include <dr/MsgSync_WntMsg.hxx>

DR_NS_BEGIN



/**
 * Message Synchronisation unix implementation
 *
 * UNIX implementation of Message Synchronisation, contains additional support
 * for sockets
 */
class DR_PUB MsgSync_wnt: public MsgSync /*, public MsgSync_Socket*/, public MsgSync_WntMsg
{
	DR_OBJECT_DECL(MsgSync_wnt, MsgSync);
	DR_REDIR_BEHAV();

protected:
	virtual				~MsgSync_wnt();

public:
	/* */				MsgSync_wnt(Thread *thr);

public: /* MsgSync_WntMsg interface */
	virtual void			registerWntMsgReceiver(const Eslot0<void> &notif);
	virtual void			unregisterWntMsgReceiver();

public:	/* main interface */
	void				threadWake();
	int				threadSleep(Sint64 timeout_ns);
	bool				threadXchg(MsgSync *new_sync);

protected:
	int				method;
	/* main interface */
	CRITICAL_SECTION		sync_mutex;
	HANDLE				sync_event;

	Eslot0<void>			msg_listener;
};


DR_NS_END

#endif
