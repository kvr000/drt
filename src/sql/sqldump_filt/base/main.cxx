#include <unistd.h>

#include <dr/x_kw.hxx>
#include <dr/Except.hxx>
#include <dr/Array.hxx>
#include <dr/List.hxx>
#include <dr/Hash.hxx>
#include <dr/InvalidFormatExcept.hxx>
#include <dr/io/File.hxx>
#include <dr/io/StreamBuffer.hxx>
#include <dr/IntEvaluator.hxx>

#include <dr/sql/sqldump_filt/QueueManager.hxx>

DR_SQL_SQLDUMP_FILT_NS_USE;

DR_NS_USE;


static void processOptions(char **argv, QueueManager *queue_manager)
{
	char *program_name = *argv++;

	for (; *argv; ++argv) {
		if (**argv == '-') {
			if (strcmp(*argv, "-r") == 0) {
				++argv;
				if (*argv == NULL) {
					fprintf(stderr, "-r option requires a file\n");
					exit(2);
				}
				xtry {
					ERef<dr::io::Handle> file(new dr::io::File(*argv, dr::io::File::M_READ));
					ERef<dr::io::StreamBuffer> stream(new dr::io::StreamBuffer(file));
					queue_manager->addRuleFile(stream);
				}
				xcatch (Except, ex) {
					fprintf(stderr, "failed to read rules %s: %s\n", *argv, ex->stringify().utf8().toStr());
					exit(2);
				}
				xend;
			}
			else if (strcmp(*argv, "-d") == 0) {
				++argv;
				if (*argv == NULL) {
					fprintf(stderr, "-d option requires a file\n");
					exit(2);
				}
				char *e;
				for (e = *argv; isalnum(*e) || *e == '_'; e++) ;
				if (*e != '=') {
					fprintf(stderr, "-d requires format name=value\n");
					exit(2);
				}
				char *f;
				for (f = e+1; isdigit(*f); f++) ;
				if (*f != '\0' || f == e+1) {
					fprintf(stderr, "-d requires format name=value\n");
					exit(2);
				}
				queue_manager->defineConstant(String(*argv, e-*argv), strtoll(e+1, NULL, 10));
			}
			else if (strcmp(*argv, "-p") == 0) {
				++argv;
				if (*argv == NULL) {
					fprintf(stderr, "-p option requires a file\n");
					exit(2);
				}
				ssize_t cnt = atoi(*argv);
				if (cnt <= 0) {
					fprintf(stderr, "-p requires positive number\n");
					exit(2);
				}
				queue_manager->setNumWorkers(cnt);
				queue_manager->setNumPending(cnt*8);
			}
			else if (strcmp(*argv, "-h") == 0) {
				fprintf(stderr, "Usage: %s -r rule_file ... -d n=v ... -p parallel\n"
						"-r rule_file\tread rule file for extract\n"
						"-d n=v\tdefine variable n to v\n"
						"-p count\tnumber of processing threads\n"
						"-h\t\thelp\n"
						, program_name);
				exit(2);
			}
			else {
				fprintf(stderr, "unknown option %s: use -h for usage\n", *argv);
				exit(2);
			}
		}
		else {
			fprintf(stderr, "unexpected argument: %s\n", *argv);
			exit(2);
		}
	}
}

static void processExtract(QueueManager *queue_manager)
{
	ERef<dr::io::File> input_file(dr::io::File::createFromOsHandle((void *)fileno(stdin), dr::io::Handle::M_NOCLOSE|dr::io::Handle::M_READ));
	ERef<dr::io::StreamBuffer> input_buf(new dr::io::StreamBuffer(input_file));
	input_buf->setCacheSize(1500000);

	Blob line_data;
	while (!(line_data = input_buf->tryReadLine()).isNull()) {
		if (queue_manager->putNextWork(line_data)) {
			line_data = queue_manager->waitProcessedData();
			fwrite(line_data.toStr(), 1, line_data.getSize(), stdout);
		}
	}
	while (!(line_data = queue_manager->waitProcessedData()).isNull()) {
		fwrite(line_data.toStr(), 1, line_data.getSize(), stdout);
	}
}

int main(int argc, char **argv)
{
	MM::enableThreadCache(1);
	ERef<QueueManager> queue_manager(new QueueManager());

	processOptions(argv, queue_manager);

	processExtract(queue_manager);

	return 0;
}
