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
#include <dr/Mem.hxx>
#include <dr/BinaryMemReader.hxx>
#include <dr/BinaryMemWriter.hxx>
#include <dr/EndOfDataException.hxx>

#include <dr/tenv/tenv.hxx>

DR_NS_USE
DR_TENV_NS_USE


#define TEST_READER
#define TEST_WRITER
#define TEST_LEBE

#ifdef TEST_LEBE
TENV_NS(lebe);
void test()
{
	char in[5] = { 1, 2, 3, 4, 5 };
	char out_n[sizeof(in)];
	char out_c[sizeof(in)];
#if DR_SEX == DR_SEX_LE
	Mem::copyLe(out_n, in, sizeof(in));
	Mem::copyBe(out_c, in, sizeof(in));
#else
	Mem::copyLe(out_c, in, sizeof(in));
	Mem::copyBe(out_n, in, sizeof(in));
#endif
	TENV_CHECK(memcmp(out_n, "\x01\x02\x03\x04\x05", sizeof(in)) == 0);
	TENV_CHECK(memcmp(out_c, "\x05\x04\x03\x02\x01", sizeof(in)) == 0);
}
TENV_NSE(lebe);
#endif

#ifdef TEST_READER
TENV_NS(reader);
void test()
{
	bool excepted;
	BString parse_data(
			"\x01\x02\x03\x04\x05\x06\x07\x08"
			"\x01\x02\x03\x04"
			"\x01\x02"
			"\x01"
			"\x01\x02\x03\x04\x05\x06\x07\x08"
			"\x01\x02\x03\x04"
			"\x01\x02"
			"\x01"
			"\x88\x83\xc1\xc0\xd0\xa0\x8c\x84\x01"
			"kra"
			);
	BinaryMemReader mr(&parse_data);

	TENV_CHECK(mr.readLe64("le") == 0x0807060504030201ULL);
	TENV_CHECK(mr.readLe32("le") == 0x04030201UL);
	TENV_CHECK(mr.readLe16("le") == 0x0201UL);
	TENV_CHECK(mr.readLe8("le") == 0x01UL);
	TENV_CHECK(mr.readBe64("be") == 0x0102030405060708ULL);
	TENV_CHECK(mr.readBe32("be") == 0x001020304UL);
	TENV_CHECK(mr.readBe16("be") == 0x0102UL);
	TENV_CHECK(mr.readBe8("be") == 0x01UL);
	TENV_CHECK(mr.readVarint64("varint") == 0x0807060504030201ULL);
	TENV_CHECK(mr.readBytes(3, "str") == Blob("kra"));
	excepted = false;
	xtry {
		mr.readLe8("except");
	}
	xcatch (Exception, ex) {
		printf("ok except: %s\n", ex->stringify().utf8().toStr());
		excepted = true;
	}
	xend;
	TENV_CHECK(excepted);
}
TENV_NSE(reader);
#endif


#ifdef TEST_WRITER
TENV_NS(writer);
void test()
{
	bool excepted;
	BString parse_data(
			"\x01\x02\x03\x04\x05\x06\x07\x08"
			"\x01\x02\x03\x04"
			"\x01\x02"
			"\x01"
			"\x01\x02\x03\x04\x05\x06\x07\x08"
			"\x01\x02\x03\x04"
			"\x01\x02"
			"\x01"
			"\x88\x83\xc1\xc0\xd0\xa0\x8c\x84\x01"
			"kra"
			);
	BString dataw;
	dataw.lock(parse_data.getSize());
	BinaryMemWriter mw(&dataw);

	mw.writeLe64(0x0807060504030201ULL, Null());
	mw.writeLe32(0x04030201UL, Null());
	mw.writeLe16(0x0201UL, Null());
	mw.writeLe8(0x01UL, Null());
	mw.writeBe64(0x0102030405060708ULL, Null());
	mw.writeBe32(0x001020304UL, Null());
	mw.writeBe16(0x0102UL, Null());
	mw.writeBe8(0x01UL, Null());
	mw.writeVarint((Uint64)0x0807060504030201ULL, Null());
	mw.writeBytes(Blob("kra"), Null());
	excepted = false;
	xtry {
		mw.writeLe8(0, "except");
	}
	xcatch (Exception, ex) {
		printf("ok except: %s\n", ex->stringify().utf8().toStr());
		excepted = true;
	}
	xend;
	TENV_CHECK(excepted);
	TENV_CHECK(dataw == parse_data);
}
TENV_NSE(writer);
#endif


DR_TENV_MAIN()
{
	tenv_init();
#ifdef TEST_READER
	TENV_RUN(reader);
#endif
#ifdef TEST_WRITER
	TENV_RUN(writer);
#endif
#ifdef TEST_LEBE
	TENV_RUN(lebe);
#endif
	return 0;
}
