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
	Hash<Sint64, Blob>		processed_data;
	Sint64				proc_counter;
	Sint64				return_counter;
	Hash<String, RList<Evaluator> >	evaluators;
	Evaluator::Arguments::Constants	constants;

	unsigned			num_free_workers;

	unsigned			max_workers;
	unsigned			max_pending;

	Ref<MutexCond>			access_lock;
	bool				is_waiting;
};


DR_SQL_SQLDUMP_FILT_NS_END
