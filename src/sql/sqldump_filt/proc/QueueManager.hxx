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

#include <dr/Object.hxx>
#include <dr/Blob.hxx>
#include <dr/Hash.hxx>
#include <dr/MutexCond.hxx>
#include <dr/ThreadSimple.hxx>
#include <dr/io/StreamBuffer.hxx>
#include <dr/sql/sqldump_filt/def_sqldump_filt.hxx>

DR_SQL_SQLDUMP_FILT_NS_BEGIN

DR_NS_USE


class QueueManager: public Object
{
	DR_OBJECT_DECL_SIMPLE(QueueManager, Object);

public:
	DR_CONSTRUCT			QueueManager();

public:
	virtual void			addRuleFile(dr::io::StreamBuffer *rules);
	virtual void			defineConstant(const String &name, Sint64 value);

public:
	virtual Sint64			getNextWork(Blob *data);
	virtual void			putProcessedData(Sint64 id, const Blob &data);

public:
	/**
	 * @return true
	 * 	if I+O buffers are overloaded and should be popped
	 * @return false
	 * 	if no action currently required
	 */
	virtual bool			putNextWork(const Blob &data);

	/**
	 * @return Null
	 * 	if there are no more data
	 * @return non-Null
	 * 	next buffer
	 */
	virtual Blob			waitProcessedData();

public:
	virtual void			setNumWorkers(unsigned max_count);
	virtual void			setNumPending(unsigned max_count);

public:
	Blob				processOneData(Sint64 line_no, const Blob &input);

protected:
	static void *			runWorker(QueueManager *queue_manager);
	static ssize_t			getNextValues(SArray<Blob> *values, Blob *ins_string, const Blob &line_data, size_t pos);

protected:
	virtual void			destroy();

protected:
	RList<ThreadSimple>		all_workers;
	SList<Blob>			waiting_data;
	THash<Sint64, Blob>		processed_data;
	Sint64				proc_counter;
	Sint64				return_counter;
	RHash<String,RList<Evaluator> >	evaluators;
	Evaluator::ArgumentsHash::Constants constants;

	unsigned			num_free_workers;

	unsigned			max_workers;
	unsigned			max_pending;

	Ref<MutexCond>			access_lock;
	bool				is_waiting;

protected:
	static const Blob		NULL_blob;
};


DR_SQL_SQLDUMP_FILT_NS_END
