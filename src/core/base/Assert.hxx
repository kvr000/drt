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

#ifndef dr__assert_hxx__
# define dr__assert_hxx__

#include <stddef.h>

#include <dr/def_core.hxx>

DR_NS_BEGIN


/**
 * Handling of fatal problems
 *
 * The class provides support for loggin fatal errors, logging the problems,
 * getting backtrace etc.
 */
class DR_PUB Fatal
{
public:
	enum Behav
	{
		BH_NONE,
		BH_OUTPUT,
	};

	/** output nothing */
	static void			behavNone(void);
	/** output to specified fd */
	static void			behavOutput(void *fd);

public:
	/** assertion failed, todo code, unexpected result etc. */
	static void			failedAssert(const char *expr, const char *file, int line, const char *func);
	/** fatal error occured (syscall failure which is fatal for the rest of
	 * the system, ...) */
	static bool			failedFatal(const char *msg, const char *file, int line, const char *func);
	/**
	 * missing handling for specific situation
	 */
	static void			failedTodo(const char *msg, const char *file, int line, const char *func);

public:
	/** handle failure */
	static void			handleFailed(const char *msg);
	/** print failure */
	static void			printFailed(const char *msg);
	/** print failure with backtrace */
	static void			printBacktrace(const char *msg);
	/** debug log */
	static void			log(const char *msg);
	/** formatted debug log */
	static void			flog(const char *msg, ...);
	/** formatted log (printed always) */
	static void			plog(const char *msg, ...);
	/** non formatted log (printed always) */
	static void			pdata(const char *msg, size_t size);

public:
	/** get backtrace */
	static size_t			getBacktrace(char *bt, size_t bt_size);

public:
	/** initialisation */
	static void			s_init();
	/** set additional debug options */
	static void			setDebug(const char *opts);
	/** set signal handlers */
	static void			setSigHandlers(const char *opts);
};

enum
{
	DR_ASSERT_DONT_CALL = 0,
};


template <bool ok>
struct compile_assert
{
	typedef void			assert_name;
	static const size_t val	= 0;
};

template <>
struct compile_assert<false>
{
	typedef Fatal			assert_name;
	//static const size_t val	= 1;
};

template <>
struct compile_assert<true>
{
	typedef void			assert_name;
	static const size_t val		= 1;
};


#if (defined DR_CXX_GCC)
# define DR_FUNCTION __PRETTY_FUNCTION__
#else
# define DR_FUNCTION __FUNCTION__
#endif


#define DR_Assert(expr) (expr) ? (void)0 : Fatal::failedAssert(#expr, __FILE__, __LINE__, DR_FUNCTION)
#define DR_AssertInvalid() Fatal::failedAssert("invalid call", __FILE__, __LINE__, DR_FUNCTION)
#define DR_AssertMsg(msg) Fatal::failedAssert(msg, __FILE__, __LINE__, DR_FUNCTION)
#define DR_AssertTodo() Fatal::failedAssert("TODO", __FILE__, __LINE__, DR_FUNCTION)
#define DR_AssertCompile(expr, reason) do { (void)DR_NSP(compile_assert)<expr>::val; } while (0)

#define DR_Fatal(msg) Fatal::failedFatal(msg, __FILE__, __LINE__, DR_FUNCTION)


DR_NS_END

#endif
