#include <stdio.h>

#include <dr/x_kw.hxx>
#include <dr/Const.hxx>
#include <dr/IntEvaluator.hxx>
#include <dr/InvalidFormatExcept.hxx>

#include <dr/sql/sqldump_filt/QueueManager.hxx>

DR_SQL_SQLDUMP_FILT_NS_BEGIN

DR_NS_USE;

DR_OBJECT_DEF(DR_SQL_SQLDUMP_FILT_NS_STR, QueueManager, Object);
DR_OBJECT_IMPL_SIMPLE(QueueManager);

const Blob QueueManager::NULL_blob(Const::bstring("NULL"));

QueueManager::QueueManager():
	proc_counter(0),
	return_counter(0),
	num_free_workers(0),
	max_workers(1),
	max_pending(2),
	access_lock(MutexCond::create(), false),
	is_waiting(false)
{
}

void QueueManager::addRuleFile(dr::io::StreamBuffer *stream)
{
	Blob line;
	while (!(line = stream->tryReadLine()).isNull()) {
		while (isspace(line[line.getSize()-1]))
			line.removeRight(1);
		const char *s = line.toStr();
		String tname;
		while (isspace(*s))
			s++;
		if (*s == '#' || *s == '\0')
			continue;
		{
			const char *t = s;
			const char *te;
			for (te = t; isalnum(*te) || *te == '_'; te++);
			for (s = te; isspace(*s); s++);
			if (te == t || *s != ':') {
				xthrownew(InvalidFormatExcept("rule syntax", "missing table name or colon"));
			}
			s++;
			tname.setUtf8(t, te-t);
		}
		evaluators[tname].append(tref(new IntEvaluator(String::createUtf8(s, line.toStr()+line.getSize()-s))));
	}
}

void QueueManager::defineConstant(const String &name, Sint64 value)
{
	constants[name] = value;
}

void QueueManager::setNumWorkers(unsigned max_count)
{
	max_workers = max_count;
}

void QueueManager::setNumPending(unsigned max_count)
{
	max_pending = max_count;
}

Sint64 QueueManager::getNextWork(Blob *data)
{
	access_lock->lock();
	xtry {
		for (;;) {
			if (max_workers == 0) {
				access_lock->unlock();
				return -1;
			}
			if (SList<Blob>::Node *n = waiting_data.iterFirst()) {
				*data = n->v;
				waiting_data.remove(n);
				Sint64 new_counter = proc_counter++;
				num_free_workers--;
				access_lock->signal();
				access_lock->unlock();
				return new_counter;
			}
			access_lock->wait();
		}
	}
	xcatchany {
		access_lock->unlock();
		xrethrowany;
	}
	xend;
}

void QueueManager::putProcessedData(Sint64 id, const Blob &data)
{
	access_lock->lock();
	xtry {
		processed_data[id] = data;
		if (is_waiting && id == return_counter)
			access_lock->broadcast();
		num_free_workers++;
		access_lock->unlock();
	}
	xcatchany {
		access_lock->unlock();
		xrethrowany;
	}
	xend;
}

bool QueueManager::putNextWork(const Blob &data)
{
	access_lock->lock();
	xtry {
		waiting_data.append(data);
		if (num_free_workers < waiting_data.count()) {
			if (all_workers.count() < max_workers) {
				all_workers.insert(tref(ThreadSimple::go((void *(*)(void *))&QueueManager::runWorker, this)));
				num_free_workers++;
			}
		}
		else {
			access_lock->signal();
		}
		bool pop_data = waiting_data.count()+processed_data.count()-num_free_workers > max_pending || processed_data.find(return_counter) != NULL;
		access_lock->unlock();
		return pop_data;
	}
	xcatchany {
		access_lock->unlock();
		xrethrowany;
	}
	xend;
}

Blob QueueManager::waitProcessedData()
{
	access_lock->lock();
	xtry {
		for (;;) {
			if (Blob *n = processed_data.accValue(return_counter)) {
				Blob data = *n;
				processed_data.remove(return_counter);
				return_counter++;
				access_lock->unlock();
				return data;
			}
			else if (return_counter == proc_counter) {
				access_lock->unlock();
				return Null();
			}
			is_waiting = true;
			access_lock->wait();
			is_waiting = false;
		}
	}
	xcatchany {
		access_lock->unlock();
		xrethrowany;
	}
	xend;
}

void QueueManager::destroy()
{
	access_lock->lock();
	access_lock->broadcast();
	max_workers = 0;
	while (RList<ThreadSimple>::Node *n = all_workers.iterFirst()) {
		access_lock->unlock();
		n->v->wait();
		access_lock->lock();
		all_workers.remove(n);
	}
	access_lock->unlock();
	Super::destroy();
}

void *QueueManager::runWorker(QueueManager *queue_manager)
{
	Sint64 id;
	Blob data;
	while ((id = queue_manager->getNextWork(&data)) >= 0) {
		data = queue_manager->processOneData(id+1, data);
		queue_manager->putProcessedData(id, data);
		MM::flushThreadCache();
	}
	return NULL;
}

ssize_t QueueManager::getNextValues(SArray<Blob> *values, Blob *ins_string, const Blob &line_data, size_t pos)
{
	const char *line = line_data;
	size_t val_pos = pos;
	switch (line[pos]) {
	case ';':
		if (pos+1 != line_data.getSize() && (line[pos+1] != '\n' || pos+2 != line_data.getSize())) {
			xthrownew(InvalidFormatExcept("INSERT", "line not finished with ;\\n"));
		}
		return -1;

	case '(':
		pos++;
		break;

	default:
		xthrownew(InvalidFormatExcept("INSERT", "missing left parenthesis"));
		break;
	}

	ins_string->setEmpty();
	values->resize(0);
	for (;;) {
		size_t spos = pos;
		for (;;) {
			switch (line[pos]) {
			case ',':
				values->append(line_data.mid(spos, pos-spos));
				pos++;
				goto next_col;
				break;
			case ')':
				values->append(line_data.mid(spos, pos-spos));
				pos++;
				goto finished;
				break;
			case '\'':
				pos++;
				for (;;) {
					switch (line[pos]) {
					case '\'':
						pos++;
						goto end_quote;
					case '\\':
						pos++;
						/* fall through */
					case '\0':
						if (pos == line_data.getSize()) {
							xthrownew(InvalidFormatExcept("INSERT", "missing closing apostrophe"));
						}
						/* fall through */
					default:
						pos++;
					}
				}
end_quote:
				break;
			case '\0':
				if (pos == line_data.getSize()) {
					xthrownew(InvalidFormatExcept("INSERT", "missing right parenthesis"));
				}
				/* fall through */
			default:
				pos++;
			}
		}
next_col:;
	}

finished:
	ins_string->copyLen(line+val_pos, pos-val_pos);
	switch (line[pos]) {
	case ',':
		pos++;
		break;
	case ';':
		break;
	}
	return pos;
}


Blob QueueManager::processOneData(Sint64 line_no, const Blob &line_data)
{
	Blob output;
	String tname;
	const char *line = line_data.toStr();
	if (memcmp(line, "INSERT INTO `", 13) != 0)
		return "";
	ssize_t pos = 13;
	{
		ssize_t tend_p = line_data.find('`', pos);
		if (tend_p < 0) {
			fprintf(stderr, "stdin:%lld: cannot find ` at the end of table name\n", (long long)line_no);
			return "";
		}
		tname.setUtf8(line_data.mid(pos, tend_p-pos));
		for (pos = tend_p+1; isspace(line[pos]); pos++);
		if (memcmp(line+pos, "VALUES", 6) != 0) {
			fprintf(stderr, "stdin:%lld: expected VALUES\n", (long long)line_no);
			return "";
		}
		for (pos += 6; isspace(line[pos]); pos++);
	}
	fprintf(stderr, "processing table %s (%ld)\n", tname.utf8().toStr(), (long)line_data.getSize());
	RList<Evaluator> *tev;
	if (RList<Evaluator> *ed = evaluators.accValue(tname)) {
		tev = ed;
	}
	else {
		return "";
	}

	int val_count = 0;
	for (;;) {
		Blob ins_str;
		SArray<Blob> values;
		if ((pos = getNextValues(&values, &ins_str, line_data, pos)) < 0)
			break;
		ERef<Evaluator::Arguments::Values> vals(new Evaluator::Arguments::Values());;
		for (size_t i = 0; i < values.count(); i++) {
			if (isdigit(*values[i].toStr())) {
				vals->operator[](i) = strtoll(values[i].toStr(), NULL, 10);
			}
			else if (values[i] == NULL_blob) {
				vals->operator[](i) = 0;
			}
			else {
				// skip value
			}
		}
		ERef<Evaluator::Arguments> args(new Evaluator::Arguments(&constants, vals));
		for (RList<Evaluator>::Node *n = tev->iterFirst(); n; n = tev->iterNext(n)) {
			xtry {
				if (!n->v->evaluate(args))
					continue;
			}
			xcatch (Except, ex) {
				fprintf(stderr, "failed to evaluate expression %s: %s\n", n->v->getExpression().utf8().toStr(), ex->stringify().utf8().toStr());
				continue;
			}
			xend;
			if (val_count++ == 0) {
				output.append("INSERT INTO ").append(tname.utf8()).append(" VALUES ");
			}
			else {
				output.append(",");
			}
			output.append(ins_str);
			break;
		}
	}
	if (val_count != 0) {
		output.append(";\n");
	}
	return output;
}


DR_SQL_SQLDUMP_FILT_NS_END
