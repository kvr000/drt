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

#include <stdlib.h>

#include <dr/def.hxx>
#include <dr/emits.hxx>

#include <dr/testenv/def_testenv.hxx>

DR_TESTENV_NS_BEGIN


#if (defined DR_LIBC_CYGWIN)
# define DR_TESTENV_MAIN() int main()
#elif (defined DR_OS_WCE)
# include <windows.h>
# define DR_TESTENV_MAIN() int WINAPI WinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
#elif (defined DR_OS_WNT) && 0
# include <windows.h>
# define DR_TESTENV_MAIN() int WINAPI WinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
#else
# define DR_TESTENV_MAIN() int main()
#endif

#define TESTNS(s) namespace nt_##s \
{
#define TESTNSE(s) }
#define TESTRUN(s) do { dr::Fatal::plog(#s ": start\n"); nt_##s::test(); dr::Fatal::plog(#s ": end\n"); } while (0)

#define CHECK(x) do { if (!(x)) { dr::Fatal::plog("%s:%d: %s failed\n", __FILE__, __LINE__, #x); DR_TESTENV_NS test_abort(); } } while (0)
#define CHECK_SIGN(x, y) do { int CHECK_x = x, CHECK_y = y; if ((CHECK_x < 0) != (CHECK_y < 0) || (CHECK_x == 0) != (CHECK_y == 0) || (CHECK_x > 0) != (CHECK_y > 0)) { dr::Fatal::plog("%s:%d: %s failed\n", __FILE__, __LINE__, #x); DR_TESTENV_NS test_abort(); } } while (0)
#define CHECK_RESET(ok) do { if (!(ok)) { dr::Fatal::plog("%s:%d: %s failed\n", __FILE__, __LINE__, #ok); DR_TESTENV_NS test_abort(); } } while (0)

DR_TESTENV_PUB void test_sleep(long time);
DR_TESTENV_PUB void test_abort();
DR_TESTENV_PUB void test_init();
DR_TESTENV_PUB long test_bench(const Eslot0<void> &func, long count);


DR_TESTENV_NS_END
