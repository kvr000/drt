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

#include <dr/app/WorkerPoolThread.hxx>

#include <dr/app/WorkerPoolManager.hxx>

#include "_gen/WorkerPoolManager-all.hxx"

DR_APP_NS_BEGIN

DR_NS_USE;

/*drt
 * include:	dr/app/def.hxx
 * include: 	dr/MutexCond.hxx
 * include: 	dr/Ref.hxx
 * include: 	dr/List.hxx
 * include: 	dr/Hash.hxx
 * include: 	dr/Exception.hxx
 * include:	dr/app/WorkerPoolThread.hxx
 * ns:		dr::app
 */

/*drt
 * class:	WorkerPoolManager
 * ancestor:	dr::Object
 *
 * at:	Ref<MutexCond>			access_lock;
 * at:	Thread *			thread;
 * at:	Sint64				worker_timeout_ns;
 * at:	int				max_workers;
 * at:	bool				is_waiting;
 * at:	RList<WorkerPoolThread>		exit_workers;
 * at:	THash<WorkerPoolThread *, bool>	free_workers;
 * at:	RHash<WorkerPoolThread *, WorkerPoolThread::WorkMessage> all_workers;
 * at:	RList<WorkerPoolThread::WorkMessage> pending_work;
 * at:	RList<WorkerPoolThread::WorkMessage> result_msg;
 *
 * friend:				class WorkerPoolThread;
 *
 * doc:{
 *	Worker Thread Pool Manager
 * }doc
 */

DR_MET(public)
WorkerPoolManager::WorkerPoolManager():
	access_lock(MutexCond::create(), false),
	thread(Thread::current()),
	worker_timeout_ns(1000000000),
	max_workers(1),
	is_waiting(false)
{
}

DR_MET(public virtual)
void WorkerPoolManager::setWorkerTimeoutNs(Sint64 timeout_ns)
{
	worker_timeout_ns = timeout_ns;
}

DR_MET(public virtual)
void WorkerPoolManager::setMaxWorkers(unsigned max_count)
{
	max_workers = max_count;
}

DR_MET(protected virtual)
void WorkerPoolManager::addResult(WorkerPoolThread *worker, WorkerPoolThread::WorkMessage **msg_ref)
{
	MutexCondLocker lock(access_lock);
	result_msg.append(mref(msg_ref));
	if (is_waiting)
		access_lock->signal();
	free_workers.create(worker, 0);
	addedResult();
}

DR_MET(protected virtual)
/**
 * Notification the result was added
 *
 * The method might be overwritten by implementor, by default the method does nothing.
 */
void WorkerPoolManager::addedResult()
{
}

DR_MET(public virtual)
void WorkerPoolManager::addWork(Object *work)
{
	MutexCondLocker lock(access_lock);
	pending_work.append(tref(new WorkerPoolThread::WorkMessage(work)));
	while (free_workers.count() < pending_work.count() && all_workers.count() < (unsigned)max_workers) {
		WorkerPoolThread *worker = createWorkerThread();
		all_workers.createNode(worker);
		free_workers.createNode(worker);
		while (exit_workers.count() > 0) {
			exit_workers.removeFirst()->wait();
		}
	}
	if (free_workers.count() > 0) {
		WorkerPoolThread *worker = free_workers.iterFirst()->k;
		free_workers.remove(worker);
		worker->addMessage(pending_work.removeFirst());
	}
}

DR_MET(public virtual)
Object *WorkerPoolManager::checkWorkResult()
{
	MutexCondLocker lock(access_lock);
	if (result_msg.count() != 0) {
		return result_msg.removeFirst()->work.getAndNull();
	}
	return NULL;
}

DR_MET(public virtual)
Object *WorkerPoolManager::waitWorkResult()
{
	MutexCondLocker lock(access_lock);
	for (;;) {
		if (result_msg.count() != 0) {
			return result_msg.removeFirst()->work.getAndNull();
		}
		if (pending_work.count() == 0 && free_workers.count() == all_workers.count()) {
			return NULL;
		}
		is_waiting = true;
		access_lock->wait();
		is_waiting = false;
	}
}

DR_MET(protected virtual)
void WorkerPoolManager::destroy()
{
	{
		MutexCondLocker lock(access_lock);
		max_workers = -1;
		is_waiting = true;
		for (THash<WorkerPoolThread *, bool>::Node *n = free_workers.iterFirst(); n; n = free_workers.iterNext(n)) {
			n->k->addMessage(tref(new WorkerPoolThread::WorkMessage(NULL)));
		}
		while (all_workers.count() > 0) {
			access_lock->wait();
		}
		while (exit_workers.count() > 0) {
			exit_workers.removeFirst()->wait();
		}
	}
	Super::destroy();
}

DR_MET(protected virtual)
/**
 * Default interface to worker
 *
 * Method must not be called, it must be either overwritten by child or
 * relevant WorkerPoolThread::runWork() must be reimplemented.
 */
void WorkerPoolManager::runWork(Object *work)
{
	DR_AssertInvalid();
}

DR_MET(protected virtual)
WorkerPoolThread *WorkerPoolManager::createWorkerThread()
{
	ERef<WorkerPoolThread> worker(new WorkerPoolThread(this));
	worker->start();
	return worker.getAndNull();
}

DR_MET(protected virtual)
void WorkerPoolManager::workerExiting(WorkerPoolThread *worker)
{
	MutexCondLocker lock(access_lock);
	free_workers.remove(worker);
	if (!all_workers.remove(worker)) {
		DR_AssertInvalid();
	}
	if (1) {
		exit_workers.append(tref(worker));
	}
	else {
		worker->unref();
	}
	if (is_waiting)
		access_lock->signal();
}


DR_APP_NS_END
