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

#ifndef dr__def__hxx__
# define dr__def__hxx__


#define DR_USE_NS 1
#if DR_USE_NS
#define DR_NS_USE using namespace dr;
#define DR_NS dr::
#define DR_NSP(rest) dr::rest
#define DR_NS_STR "dr::"
#define DR_NS_STRP(rest) "dr::"rest
#define DR_NS_BEGIN namespace dr \
{
#define DR_NS_END };
#else
#define DR_NS_USE
#define DR_NS
#define DR_NS_BEGIN
#define DR_NS_END
#endif


#include <dr/arch/def.arch.hxx>


#if (defined __GNUC__) || (defined __GCCE__)
# define DR_CXX_GCC 1
#elif (defined _MSC_VER)
# define DR_CXX_VC 1
# pragma warning(disable:4251)
#else
# error unknown compiler
#endif

#ifdef DR_CXX_GCC
# ifdef DR_NOINLINE 
#  define DR_DINLINE inline __attribute__((noinline))
#  define DR_MINLINE inline __attribute__((noinline))
#  define DR_RINLINE inline __attribute__((noinline))
#  define DR_NINLINE inline __attribute__((noinline))
# else
#  define DR_DINLINE inline __attribute__((always_inline))
#  define DR_MINLINE inline
#  define DR_RINLINE inline
#  define DR_NINLINE __attribute__((noinline))
# endif
#else
# define DR_DINLINE inline
# define DR_MINLINE inline
# define DR_RINLINE inline
# define DR_NINLINE 
#endif

#define DR_CONSTRUCT
#define DR_DESTRUCT
#define DR_MET(modifiers)

#if (defined DR_OS_UNIX)
# define DR_IMPORT_FUN
# define DR_IMPORT_MET
# define DR_IMPORT_MTS
# define DR_IMPORT_DAT
# define DR_IMPORT_DTC
# define DR_IMPORT_DTS
# define DR_IMPORT_CLS
# define DR_EXPORT_FUN
# define DR_EXPORT_MET
# define DR_EXPORT_MTS
# define DR_EXPORT_DAT
# define DR_EXPORT_DTC
# define DR_EXPORT_DTS
# define DR_EXPORT_CLS
#elif (defined DR_OS_SYMBIAN)
# define DR_IMPORT_FUN __declspec(dllimport)
# define DR_IMPORT_MET __declspec(dllimport)
# define DR_IMPORT_MTS __declspec(dllimport)
# define DR_IMPORT_DAT __declspec(dllimport)
# define DR_IMPORT_DTC __declspec(dllimport)
# define DR_IMPORT_DTS __declspec(dllimport)
# define DR_IMPORT_CLS __declspec(dllimport)
# define DR_EXPORT_FUN __declspec(dllexport)
# define DR_EXPORT_MET __declspec(dllexport)
# define DR_EXPORT_MTS __declspec(dllexport)
# define DR_EXPORT_DAT __declspec(dllexport)
# define DR_EXPORT_DTC __declspec(dllexport)
# define DR_EXPORT_DTS __declspec(dllexport)
# define DR_EXPORT_CLS __declspec(dllexport)
#elif (defined DR_OS_WNT) && (!defined DR_LIBC_CYGWIN)
# define DR_IMPORT_FUN __declspec(dllimport)
# define DR_IMPORT_MET __declspec(dllimport)
# define DR_IMPORT_MTS __declspec(dllimport)
# define DR_IMPORT_DAT __declspec(dllimport)
# define DR_IMPORT_DTC __declspec(dllimport)
# define DR_IMPORT_DTS __declspec(dllimport)
# define DR_IMPORT_CLS __declspec(dllimport)
# define DR_EXPORT_FUN __declspec(dllexport)
# define DR_EXPORT_MET __declspec(dllexport)
# define DR_EXPORT_MTS __declspec(dllexport)
# define DR_EXPORT_DAT __declspec(dllexport)
# define DR_EXPORT_DTC __declspec(dllexport)
# define DR_EXPORT_DTS __declspec(dllexport)
# define DR_EXPORT_CLS __declspec(dllexport)
#else
# define DR_IMPORT_FUN
# define DR_IMPORT_MET
# define DR_IMPORT_MTS
# define DR_IMPORT_DAT
# define DR_IMPORT_DTC
# define DR_IMPORT_DTS
# define DR_IMPORT_CLS
# define DR_EXPORT_FUN
# define DR_EXPORT_MET
# define DR_EXPORT_MTS
# define DR_EXPORT_DAT
# define DR_EXPORT_DTC
# define DR_EXPORT_DTS
# define DR_EXPORT_CLS
#endif

#define DR_NON_FUN
#define DR_NON_MET
#define DR_NON_MTS
#define DR_NON_DAT
#define DR_NON_DTC
#define DR_NON_DTS
#define DR_NON_CLS

#define DR_ANON_BEGIN namespace {
#define DR_ANON_END }

#ifndef DR_DFLAGS
# define DR_DFLAGS 0
#endif
#if DR_DFLAGS > 0
# define DR_DCODE(text) text
# define DR_DINVALPTR(ptr) *(void **)ptr = (void *)0xcdcdcdc0
#else
# define DR_DCODE(text) do { } while (0)
# define DR_DINVALPTR(ptr) do { } while (0)
#endif

#define DR_LOG(args) (Fatal::flog args)
#define DR_LOG0(args) ((DR_DFLAGS&1)?Fatal::flog args:(void)0)
#define DR_LOG1(args) ((DR_DFLAGS&2)?Fatal::flog args:(void)0)
#define DR_LOG2(args) ((DR_DFLAGS&4)?Fatal::flog args:(void)0)
#define DR_LOG3(args) ((DR_DFLAGS&8)?Fatal::flog args:(void)0)
#define DR_LOG4(args) ((DR_DFLAGS&16)?Fatal::flog args:(void)0)


#endif
