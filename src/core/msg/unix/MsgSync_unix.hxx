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

#ifndef dr__MsgSync_unix__hxx__
# define dr__MsgSync_unix__hxx__

#include <dr/MsgSync.hxx>
#include <dr/MsgSync_Socket.hxx>

DR_NS_BEGIN


/**
 * Message Synchronisation unix implementation
 *
 * UNIX implementation of Message Synchronisation, contains additional support
 * for sockets
 */
class DR_PUB MsgSync_unix: public MsgSync, public MsgSync_Socket
{
	DR_OBJECT_DECL(MsgSync_unix, MsgSync);
	DR_REDIR_BEHAV();

protected:
	typedef struct socket_notif_data
	{
		int			fd;
		WaitMode		mode;
		Eslot0<WaitMode>	precheck;
		Eslot1<void, WaitMode> notif;
		socket_notif_data()						: precheck(Null()), notif(Null()) {}
	} socket_notif_data;


protected:
	virtual				~MsgSync_unix();

public:
	/* */				MsgSync_unix(Thread *thr);

public:	/* main interface */
	void				threadWake();
	void				threadSleep();
	bool				threadXchg(MsgSync *new_sync);

public:	/* MsgSync_Socket interface */
	void				registerSocket(void *fhandle, WaitMode mode, const Eslot0<WaitMode> &precheck, const Eslot1<void, WaitMode> &notif);
	void				unregisterSocket(void *fhandle);

protected:
	int				method;
	/* main interface */
	pthread_mutex_t			sync_mutex;
	pthread_cond_t			sync_cond;

	/* MsgSync_Socket interface */
	TList<socket_notif_data, ComparInv<socket_notif_data> >	socket_list;
	int				pipe_fd[2];
};


DR_NS_END

#endif
