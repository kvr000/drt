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

#include <dr/x_kw.hxx>
#include <dr/Const.hxx>

#include <dr/app/WorkerPoolManager.hxx>

#include <dr/app/WorkerPoolThread.hxx>

#include "_gen/WorkerPoolThread-all.hxx"

DR_APP_NS_BEGIN

DR_NS_USE;


/*drt
 * include:	dr/app/def.hxx
 * include: 	dr/Thread.hxx
 * ns:		dr::app
 *
 * forward:				class WorkerPoolManager;
 */

/*drt
 * class:	WorkerPoolThread::WorkMessage
 * type:	struct
 * ancestor:	dr::Message
 *
 * at:	Ref<Object>			work;
 *
 * friend:				class WorkerPoolThread;
 * friend:				class WorkerPoolManager;
 *
 * doc:{
 * 	Communication message between WorkerPoolManager and WorkerPoolThread
 * }doc
 */

DR_MET(public)
WorkerPoolThread::WorkMessage::WorkMessage(Object *work_):
	work(work_, true)
{
	setNone();
}


/*drt
 * class:	WorkerPoolThread
 * ancestor:	dr::Thread
 *
 * at:	WorkerPoolManager *			manager;
 *
 * doc:{
 *	Worker Thread
 * }doc
 */

DR_MET(public)
WorkerPoolThread::WorkerPoolThread(WorkerPoolManager *manager_):
	Thread(),
	manager(manager_)
{
}

DR_MET(protected)
WorkerPoolThread::WorkMessage *WorkerPoolThread::checkNextWork()
{
	Ref<WorkMessage> wmsg;
	// check for on way message first
	if (Message *msg = waitAnyMessageTimeout(0)) {
		wmsg.setNoref((WorkMessage *)msg->getIfaceUnref(WorkMessage::comp_name));
		goto out;
	}

	// all under the lock, check for on way message and then for pending
	// work, otherwise register as free worker
	{
		MutexCondLocker lock(manager->access_lock);
		if (Message *msg = waitAnyMessageTimeout(0)) {
			wmsg.setNoref((WorkMessage *)msg->getIfaceUnref(WorkMessage::comp_name));
			goto out;
		}
		else if (manager->max_workers < 0) {
			MM::flushThreadCache();
			manager->workerExiting(this);
			return NULL;
		}
		else if (manager->pending_work.count() > 0) {
			wmsg = manager->pending_work.removeFirst();
			manager->free_workers.remove(this);
			goto out;
		}
		else {
			manager->free_workers[this];
		}
	}

	// without the lock wait for message specified interval
	if (Message *msg = waitAnyMessageTimeout(manager->worker_timeout_ns)) {
		wmsg.setNoref((WorkMessage *)msg->getIfaceUnref(WorkMessage::comp_name));
		goto out;
	}

	// all under the lock, check for on way message and finally exit if
	// still nothing
	{
		MutexCondLocker lock(manager->access_lock);
		if (Message *msg = waitAnyMessageTimeout(0)) {
			wmsg.setNoref((WorkMessage *)msg->getIfaceUnref(WorkMessage::comp_name));
			goto out;
		}
		MM::flushThreadCache();
		manager->workerExiting(this);
		return NULL;
	}

out:
	if (!wmsg->work.isNull()) {
		return wmsg.getAndNull();
	}

	wmsg.setNull();
	MM::flushThreadCache();
	manager->workerExiting(this);
	return NULL;
}

DR_MET(protected)
void WorkerPoolThread::addResult(WorkMessage **msg)
{
	manager->addResult(this, msg);
}

DR_MET(protected virtual)
void WorkerPoolThread::runWork(Object *work)
{
	manager->runWork(work);
}

DR_MET(protected)
void *WorkerPoolThread::run()
{
	while (WorkMessage *msg = checkNextWork()) {
		runWork(msg->work);
		addResult(&msg);
	}
	return NULL;
}


DR_APP_NS_END
