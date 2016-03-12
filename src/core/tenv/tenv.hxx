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

#include <dr/tenv/def.hxx>

DR_TENV_NS_BEGIN


#if (defined DR_LIBC_CYGWIN)
# define DR_TENV_MAIN() int main()
#elif (defined DR_OS_WCE)
# include <windows.h>
TENV_MAIN() int WINAPI WinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
#elif (defined DR_OS_WNT) && 0
# include <windows.h>
# define DR_TENV_MAIN() int WINAPI WinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
#else
# define DR_TENV_MAIN() int main()
#endif

#define TENV_NS(s) namespace nt_##s \
{
#define TENV_NSE(s) }
#define TENV_RUN(s) do { dr::Fatal::plog(#s ": start\n"); dr::tenv::tenv_run(#s, dr::Eslot0<void>(&nt_##s::test)); dr::Fatal::plog(#s ": end\n"); } while (0)

#define TENV_CHECK(x) do { if (!(x)) { dr::Fatal::plog("%s:%d: %s failed\n", __FILE__, __LINE__, #x); DR_TENV_NS tenv_abort(); } } while (0)
#define TENV_CHECK_EXCEPTION(statement, exception) do { int drcheck_got_ex = 0; DR_TRY { statement; } DR_CATCH(exception, drcheck_ex) { drcheck_got_ex = 1; } DR_ENDTRY; TENV_CHECK(drcheck_got_ex); } while (0)
#define TENV_CHECK_SIGN(x, y) do { int TENV_CHECK_x = x, TENV_CHECK_y = y; if ((TENV_CHECK_x < 0) != (TENV_CHECK_y < 0) || (TENV_CHECK_x == 0) != (TENV_CHECK_y == 0) || (TENV_CHECK_x > 0) != (TENV_CHECK_y > 0)) { dr::Fatal::plog("%s:%d: %s failed\n", __FILE__, __LINE__, #x); DR_TENV_NS tenv_abort(); } } while (0)
#define TENV_CHECK_RESET(ok) do { if (!(ok)) { dr::Fatal::plog("%s:%d: %s failed\n", __FILE__, __LINE__, #ok); DR_TENV_NS tenv_abort(); } } while (0)

DR_TENV_PUB void tenv_init();
DR_TENV_PUB void tenv_sleep(long time);
DR_TENV_PUB void tenv_abort();
DR_TENV_PUB void tenv_run(const char *name, const Eslot0<void> &func);
DR_TENV_PUB long tenv_bench(const Eslot0<void> &func, long count);
DR_TENV_PUB long tenv_parbench(int num_threads, const Eslot0<void> &func, long count);


DR_TENV_NS_END
