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
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#ifndef DR_NO_BACKTRACE
#include <execinfo.h>
#endif

#include <dr/Thread.hxx>
#include <dr/Mutex.hxx>
#include <dr/Assert.hxx>

#if defined DR_OS_UNIX
#include <signal.h>
#endif
#if defined DR_OS_SYMBIAN
extern "C" {
ssize_t snprintf(char *buf, size_t bufsize, const char *format, ...);
ssize_t vsnprintf(char *buf, size_t bufsize, const char *format, va_list args);
};
#elif (defined DR_OS_WNT)
#include <windows.h>
#endif

#if (defined DR_CXX_VC)
# define snprintf _snprintf
# define vsnprintf _vsnprintf
#endif

#if (defined DR_OS_WCE)
# define getenv(x) NULL
# define write(fd, buf, sz_) do { if ((void *)fd != (void *)INVALID_HANDLE_VALUE) WriteFile((HANDLE)fd, buf, sz_, NULL, NULL); size_t sz = sz_; WCHAR tbuf[1024]; if (sz >= 1024) sz = 1023; tbuf[sz] = '\0'; while (sz-- > 0) tbuf[sz] = buf[sz]; OutputDebugStringW(tbuf); } while (0)
# define abort() 
#elif (defined DR_OS_WNT)
# define getenv(x) NULL
# define write(fd, buf, sz_) do { if ((void *)fd != (void *)INVALID_HANDLE_VALUE) WriteFile((HANDLE)fd, buf, sz_, NULL, NULL); size_t sz = sz_; WCHAR tbuf[1024]; if (sz >= 1024) sz = 1023; tbuf[sz] = '\0'; while (sz-- > 0) tbuf[sz] = buf[sz]; OutputDebugStringW(tbuf); } while (0)
# define abort() 
#else
# include <unistd.h>
#endif

DR_NS_BEGIN


enum
{
	FTD_OUTPUT	= 1,
	FTD_BTRACE	= 2,
	FTD_LOG_BT	= 4,
};

static void *fatal_fd = (void *)
#if !(defined DR_OS_WNT)
	2
#elif (defined DR_OS_WCE)
	-1
#else
	STD_ERROR_HANDLE
#endif
	;
static Mutex *print_mutex = NULL;
static int debug_options = -1;


#if (defined DR_OS_UNIX)
static void fatal_handler(int signo)
{
	signal(signo, SIG_DFL);
	Fatal::printFailed("");
	raise(signo);
}
#endif

void Fatal::s_init()
{
	const char *opts;
	if (debug_options >= 0)
		return;
	debug_options = 0;

	Mutex::createOnce(&print_mutex);
	Thread::current();
	
	debug_options = FTD_OUTPUT|FTD_BTRACE;
	if ((opts = getenv("DRO_FATAL")) != NULL) {
		setDebug(opts);
	}
}

void Fatal::setDebug(const char *opts)
{
	while (*opts != '\0') {
		switch (*opts) {
		case '.':
			debug_options |= FTD_LOG_BT;
			setSigHandlers(".");
			break;
		case 'n':
			debug_options = 0;
			break;
		case 'B':
			debug_options = ~FTD_BTRACE;
			break;
		case 'O':
			debug_options = ~FTD_OUTPUT;
			break;
		case 't':
			debug_options |= FTD_LOG_BT;
			break;
		case 'f':
			fatal_fd = (void *)strtol(opts, (char **)&opts, 0);
			opts--;
			break;
		case 's':
			setSigHandlers(".");
			break;
		}
		opts++;
	}
}

void Fatal::setSigHandlers(const char *opts)
{
	while (*opts != '\0') {
		switch (*opts) {
#ifdef DR_OS_UNIX
		case '.':
			signal(SIGSEGV, &fatal_handler);
			signal(SIGABRT, &fatal_handler);
			signal(SIGBUS, &fatal_handler);
			signal(SIGILL, &fatal_handler);
			signal(SIGFPE, &fatal_handler);
			break;

		case 'a':
			signal(SIGABRT, &fatal_handler);
			break;

		case 'b':
			signal(SIGBUS, &fatal_handler);
			break;

		case 'f':
			signal(SIGFPE, &fatal_handler);
			break;

		case 'i':
			signal(SIGILL, &fatal_handler);
			break;

		case 's':
			signal(SIGSEGV, &fatal_handler);
			break;
#endif
		}
		opts++;
	}
}

void Fatal::failedAssert(const char *expr, const char *file, int line, const char *func)
{
	char buf[1024];
	snprintf(buf, 1024, "%s:%d: expr '%s' failed in function %s\n", file, line, expr, func);
	buf[1023] = '\0';
	
	handleFailed(buf);
}

bool Fatal::failedFatal(const char *msg, const char *file, int line, const char *func)
{
	char buf[1024];
	snprintf(buf, 1024, "%s:%d: %s (in function %s)\n", file, line, msg, func);
	buf[1023] = '\0';
	
	handleFailed(buf);

	return false;
}

void Fatal::failedTodo(const char *msg, const char *file, int line, const char *func)
{
	char buf[1024];
	snprintf(buf, 1024, "%s:%d: %s (in function %s)\n", file, line, msg, func);
	buf[1023] = '\0';
	
	handleFailed(buf);
}

void Fatal::behavNone()
{
	debug_options = 0;
}

void Fatal::behavOutput(void *fd)
{
	fatal_fd = fd;
}

void Fatal::handleFailed(const char *msg)
{
	printFailed(msg);
	abort();
}

void Fatal::printFailed(const char *msg)
{
	if (debug_options&FTD_BTRACE)
		printBacktrace(msg);
	else
		log(msg);
}

size_t Fatal::getBacktrace(char *bto, size_t bto_size)
{
	size_t pos = 0;

#ifndef DR_NO_BACKTRACE
	int bc;
	void **bs = (void **)alloca(1024*sizeof(void *));
	if ((bc = backtrace(bs, 1024)) >= 0) {
		int ibt;
		char **bt;
		while (bs[bc-1] == bs[bc-2])
			bc--;
		bt = backtrace_symbols(bs, bc);
		for (ibt = 1; ibt < bc; ibt++) {
			size_t l = strlen(bt[ibt]);
			if (pos+l+1 <= bto_size) {
				memcpy(bto+pos, bt[ibt], l);
				bto[pos+l] = '\n';
			}
			pos += l+1;
		}
		free(bt);
	}
#endif
	if (pos < bto_size)
		bto[pos] = '\0';
	return pos;
}

void Fatal::printBacktrace(const char *msg)
{
	ssize_t size;
	char buf[256];

	if (!(debug_options&FTD_OUTPUT))
		return;

	Mutex::createOnce(&print_mutex);
	print_mutex->lock();
	size = sprintf(buf, "--- Thread %p backtrace:\n", Thread::current());
	if (fatal_fd != (void *)-1) {
		write((int)(SintPtr)fatal_fd, msg, strlen(msg));
		write((int)(SintPtr)fatal_fd, buf, size);
	}
#ifndef DR_NO_BACKTRACE
	int bc;
	void **bs = (void **)alloca(1024*sizeof(void *));
	if ((bc = backtrace(bs, 1024)) >= 0)
		backtrace_symbols_fd(bs, bc, (SintPtr)fatal_fd);
#endif
	size = sprintf(buf, "--- Thread %p end\n", Thread::current());
	if (fatal_fd != (void *)-1)
		write((int)(SintPtr)fatal_fd, buf, size);
	print_mutex->unlock();
}

void Fatal::plog(const char *msg, ...)
{
	char buf[4096];
	va_list va;

	va_start(va, msg);
	vsnprintf(buf, sizeof(buf), msg, va); buf[sizeof(buf)-1] = '\0';

	if (fatal_fd != (void *)-1)
		write((int)(SintPtr)fatal_fd, buf, strlen(buf));
}

void Fatal::pdata(const char *msg, size_t size)
{
	if (fatal_fd != (void *)-1)
		write((int)(SintPtr)fatal_fd, msg, size);
}

void Fatal::log(const char *msg)
{
	if (!(debug_options&FTD_OUTPUT))
		return;

	if (debug_options&FTD_LOG_BT)
		printBacktrace(msg);
	else {
		Mutex::createOnce(&print_mutex);
		print_mutex->lock();
		if (fatal_fd != (void *)-1)
			write((int)(SintPtr)fatal_fd, msg, strlen(msg));
		print_mutex->unlock();
	}
}

void Fatal::flog(const char *msg, ...)
{
	char buf[4096];
	va_list va;

	if (!(debug_options&FTD_OUTPUT))
		return;

	va_start(va, msg);
	vsnprintf(buf, sizeof(buf), msg, va); buf[sizeof(buf)-1] = '\0';

	log(buf);
}


DR_NS_END
