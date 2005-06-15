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
#include <string.h>

#include <dr/x_kw.hxx>
#include <dr/pe_error.hxx>

#include <dr/Ref.hxx>
#include <dr/Exception.hxx>
#include <dr/Variant.hxx>

#include <dr/app/WorkerPoolManager.hxx>

#include <dr/tenv/tenv.hxx>

/*drt
 * ns:	dr::app::t
 */

DR_APP_NS_USE;
DR_TENV_NS_USE


#define TEST_WORKER

#ifdef TEST_WORKER
TENV_NS(worker);

class TestWorker: public WorkerPoolManager
{
	virtual void			runWork(Object *obj)
	{
		//Fatal::plog("Thread %p processing %ld\n", Thread::current(), ((Variant *)obj)->toInt());
	}
};

void test()
{
	ERef<WorkerPoolManager> wm(new TestWorker);

	wm->setMaxWorkers(16);

	for (int i = 0; i < 1024; i++) {
		wm->addWork(tref(new Variant((Sint64)i)));
		tref(wm->checkWorkResult());
	}
	while (!tref(wm->waitWorkResult()).isNull()) ;
}
TENV_NSE(worker);
#endif

DR_TENV_MAIN()
{
	MM::enableThreadCache(1);
	tenv_init();
#ifdef TEST_WORKER
	TENV_RUN(worker);
#endif
	return 0;
}
