#include <dr/x_kw.hxx>
#include <dr/Except.hxx>
#include <dr/Ref.hxx>
#include <dr/Variant.hxx>
#include <dr/io/File.hxx>

#define MEMSIZE (1024*1024)

DR_NS_USE;

int main(int argc, char **argv)
{
	io::FileOffset pos;
	io::FileOffset end;
	String filename;

	if (argc != 3) {
		dr::Fatal::plog("Usage: %s filename filesize\n");
		return 2;
	}

	filename.setUtf8(argv[1]);
	end = tref(new Variant(Blob(argv[2])))->toInt();

	Ref<dr::io::File> fd;

	xtry {
		fd.setNoref(new dr::io::File(String(argv[1]), dr::io::File::M_WRITE|dr::io::File::M_CREATE|dr::io::File::M_APPEND));
		pos = fd->seek(0, 2);
	}
	xcatch (Except, ex) {
		Fatal::plog("Failed to open file %s for writing: %s\n", filename.utf8().toStr(), ex->stringify().utf8().toStr());
		return 1;
	}
	xend;

	void *mem = MM::alloc(MEMSIZE);
	memset(mem, 0, MEMSIZE);

	Fatal::plog("File opened, writing from %lld to %lld\n", (long long)pos, (long long)end);

	xtry {
		while (pos < end) {
			size_t towr = end-pos > MEMSIZE ? MEMSIZE : end-pos;
			pos += fd->write(mem, towr);
		}
		fd.setNull();
	}
	xcatch (Except, ex) {
		Fatal::plog("Failed to write to file %s: %s\n", filename.utf8().toStr(), ex->stringify().utf8().toStr());
		return 1;
	}
	xend;
	return 0;
}
