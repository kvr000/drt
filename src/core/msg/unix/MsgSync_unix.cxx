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

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>

#include <dr/Const.hxx>
#include <dr/SysError.hxx>
#include <dr/MsgSync.hxx>
#include <dr/Thread.msg.hxx>

#include "MsgSync_unix.hxx"

DR_NS_BEGIN


DR_OBJECT_DEF(DR_NS_STR, MsgSync_unix, MsgSync);

//DR_OBJECT_IMPL_IFACE1(MsgSync_unix, MsgSync_Socket);
DR_EXPORT_MET const Static *MsgSync_unix::getObjectStatic() const
{
	if (comp_static)
		return comp_static;
	return createObjectStatic(Const::string(DR_NS_STRP("MsgSync_unix")), &comp_static, Super::getObjectStatic(), (char *)(void *)(MsgSync_Socket *)this-(char *)(void *)this, MsgSync_Socket::getIfaceStatic(), 0, NULL);
}


/* constructor/destructor */
MsgSync_unix::MsgSync_unix(Thread *thread_):
	DR_OBJECT_INIT(MsgSync_unix, MsgSync, (thread_))
{
	int err;
	DR_LOG1(("%s: creating MsgSync_unix: %p (thread %p)\n", DR_FUNCTION, this, thread_));

	if ((err = pthread_mutex_init(&sync_mutex, NULL)) != 0) {
		DR_THROWNEW(SysError(err));
	}

	if ((err = pthread_cond_init(&sync_cond, NULL)) != 0) {
		DR_THROWNEW(SysError(err));
	}
#if 0
	printf("init: mutex: %8.8x %8.8x %8.8x %8.8x %8.8x %8.8x %8.8x %8.8x\n", ((int *)sync_mutex)[0], ((int *)sync_mutex)[1], ((int *)sync_mutex)[2], ((int *)sync_mutex)[3], ((int *)sync_mutex)[4], ((int *)sync_mutex)[5], ((int *)sync_mutex)[6], ((int *)sync_mutex)[7]);
	printf("init: cond:  %8.8x %8.8x %8.8x %8.8x %8.8x %8.8x %8.8x %8.8x\n", ((int *)sync_cond)[0], ((int *)sync_cond)[1], ((int *)sync_cond)[2], ((int *)sync_cond)[3], ((int *)sync_cond)[4], ((int *)sync_cond)[5], ((int *)sync_cond)[6], ((int *)sync_cond)[7]);
#endif

	pipe_fd[0] = pipe_fd[1] = -1;
}

MsgSync_unix::~MsgSync_unix()
{
	DR_LOG1(("%s: destroying MsgSync_unix: %p\n", DR_FUNCTION, this));
	if (pipe_fd[0] >= 0) {
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}

	pthread_mutex_destroy(&sync_mutex);
	pthread_cond_destroy(&sync_cond);
}

/* MsgSync_Socket interface */
void MsgSync_unix::registerSocket(void *os_handle, WaitMode mode, const Eslot0<WaitMode> &precheck, const Eslot1<void, WaitMode> &notif)
{
	socket_notif_data data;
	data.fd = (SintPtr)os_handle;
	data.mode = mode;
	data.precheck = precheck;
	data.notif = notif;
	socket_list.append(data);
}

void MsgSync_unix::unregisterSocket(void *os_handle)
{
	TList<socket_notif_data>::Node *node;

	for (node = socket_list.iterFirst(); node->v.fd != (SintPtr)os_handle; node = socket_list.iterNext(node)) ;
	socket_list.remove(node);
}

bool MsgSync_unix::threadXchg(MsgSync *new_sync)
{
	if (socket_list.iterFirst())
		return false;
	return true;
}

/* thread sync interface */
void MsgSync_unix::threadSleep()
{
	TList<socket_notif_data>::Node *node;
	int highest = -1;
	int rhighest;
	fd_set rs, ws, es;

	/* set select sets */
	FD_ZERO(&rs); FD_ZERO(&ws); FD_ZERO(&es);
	for (node = socket_list.iterFirst(); node; node = socket_list.iterNext(node)) {
		if (!node->v.precheck.isNull()) {
			WaitMode mode;
			if ((mode = node->v.precheck()) != 0) {
				ERef<Message> msg(new Message);
				msg->setSlot(*(Eslot1<void, void *> *)((void *)&node->v.notif), (void *)(mode|W_PRECHECK));
				thread->addMessage_lr(msg.getAndNull());
			}
		}
		if (node->v.mode&W_READ) {
			FD_SET(node->v.fd, &rs);
			if (highest < node->v.fd)
				highest = node->v.fd;
		}
		if (node->v.mode&W_WRITE) {
			FD_SET(node->v.fd, &ws);
			if (highest < node->v.fd)
				highest = node->v.fd;
		}
		if (node->v.mode&W_EXCEPT) {
			FD_SET(node->v.fd, &es);
			if (highest < node->v.fd)
				highest = node->v.fd;
		}
	}
	if (highest < 0) {
		/* no fd is registered, perform wait via condition */
		int err;
#if 0
		printf("lock: mutex: %8.8x %8.8x %8.8x %8.8x %8.8x %8.8x %8.8x %8.8x\n", ((int *)sync_mutex)[0], ((int *)sync_mutex)[1], ((int *)sync_mutex)[2], ((int *)sync_mutex)[3], ((int *)sync_mutex)[4], ((int *)sync_mutex)[5], ((int *)sync_mutex)[6], ((int *)sync_mutex)[7]);
		printf("lock: cond:  %8.8x %8.8x %8.8x %8.8x %8.8x %8.8x %8.8x %8.8x\n", ((int *)sync_cond)[0], ((int *)sync_cond)[1], ((int *)sync_cond)[2], ((int *)sync_cond)[3], ((int *)sync_cond)[4], ((int *)sync_cond)[5], ((int *)sync_cond)[6], ((int *)sync_cond)[7]);
		pthread_mutex_init(&sync_mutex, NULL);
		pthread_cond_init(&sync_cond, NULL);
#endif
		if ((err = pthread_mutex_lock(&sync_mutex)) != 0) {
			DR_THROWNEW(SysError(err));
		}
		method = 0;
		if (thread->msg_setWaiting()) {
			DR_LOG1(("%s: %p: selecting method %d\n", DR_FUNCTION, this, method));
			pthread_cond_wait(&sync_cond, &sync_mutex);
			DR_LOG1(("%s: being woke\n", DR_FUNCTION));
		}
		pthread_mutex_unlock(&sync_mutex);
	}
	else {
		/* some fd registered, perform wait via select() */
		if (pipe_fd[0] < 0) {
			while (pipe(pipe_fd) < 0) {
				DR_Fatal(BString("Failed to create pipe for thread synchronization: ")+BString(strerror(errno)));
			}
			fcntl(pipe_fd[0], F_SETFD, FD_CLOEXEC); fcntl(pipe_fd[1], F_SETFD, FD_CLOEXEC);
			fcntl(pipe_fd[0], F_SETFL, O_NONBLOCK); fcntl(pipe_fd[1], F_SETFL, O_NONBLOCK);
		}
		FD_SET(pipe_fd[0], &rs);
		if (highest < pipe_fd[0])
			highest = pipe_fd[0];
		pthread_mutex_lock(&sync_mutex);
		method = 1;
		if (!thread->msg_setWaiting()) {
			pthread_mutex_unlock(&sync_mutex);
			return;
		}
		pthread_mutex_unlock(&sync_mutex);
		while ((rhighest = select(highest+1, &rs, &ws, &es, NULL)) < 0) {
			switch (errno) {
			case EINTR:
				break;
			default:
				DR_Fatal(BString("select returned ")+BString(strerror(errno)));
			}
		}
		thread->msg_unsetWaiting();
		rhighest = highest+1;
		if (FD_ISSET(pipe_fd[0], &rs)) {
			char buf[256];
			while (read(pipe_fd[0], buf, sizeof(buf)) == sizeof(buf)) ;
		}

		/* check for set nodes */
		for (node = socket_list.iterFirst(); node; node = socket_list.iterNext(node)) {
			WaitMode mode = (WaitMode)0;
			if (node->v.fd > rhighest)
				continue;
			if (FD_ISSET(node->v.fd, &rs))
				mode |= node->v.mode&W_READ;
			if (FD_ISSET(node->v.fd, &ws))
				mode |= node->v.mode&(W_WRITE);
			if (FD_ISSET(node->v.fd, &es))
				mode |= node->v.mode&(W_EXCEPT);
			if (mode) {
				ERef<Message> msg(new Message);
				msg->setSlot(*(Eslot1<void, void *> *)((void *)&node->v.notif), (void *)mode);
				thread->addMessage_lr(msg.getAndNull());
			}
		}
	}
}

void MsgSync_unix::threadWake()
{
	pthread_mutex_lock(&sync_mutex);
	thread->msg_unsetWaiting();
	switch (method) {
	case 0:
		pthread_cond_signal(&sync_cond);
		break;

	case 1:
		write(pipe_fd[1], &thread, 1);
		break;
	}
	pthread_mutex_unlock(&sync_mutex);
}


MsgSync *MsgSync::createDefault(Thread *thread)
{
	return new MsgSync_unix(thread);
}


DR_NS_END
