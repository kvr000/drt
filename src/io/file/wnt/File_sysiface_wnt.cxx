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

#include <winsock2.h>
#include <windows.h>

#include <dr/pe_error.hxx>
#include <dr/x_kw.hxx>

#include <dr/io/File.hxx>
#include <dr/io/IoExcept.hxx>
#include <dr/io/NonblockExcept.hxx>
#include <dr/io/FileNotFoundExcept.hxx>

#include <dr/io/dev/File_sysiface_wnt.hxx>

DR_IO_NS_BEGIN

int File_sysiface_wnt::mapWntErrorToPosix(int error)
{
	int error_map0[] = {
		0,			// #define NO_ERROR 0L
		DRP_EINVAL,		// #define ERROR_INVALID_FUNCTION 1L
		DRP_ENOENT,		// #define ERROR_FILE_NOT_FOUND 2L
		DRP_ENOENT,		// #define ERROR_PATH_NOT_FOUND 3L
		DRP_EMFILE,		// #define ERROR_TOO_MANY_OPEN_FILES 4L
		DRP_EACCES,		// #define ERROR_ACCESS_DENIED 5L
		DRP_EBADF,		// #define ERROR_INVALID_HANDLE 6L
		-1,			// #define ERROR_ARENA_TRASHED 7L
		DRP_ENOMEM,		// #define ERROR_NOT_ENOUGH_MEMORY 8L
		DRP_EINVAL,		// #define ERROR_INVALID_BLOCK 9L
		DRP_EINVAL,		// #define ERROR_BAD_ENVIRONMENT 10L
		DRP_EINVAL,		// #define ERROR_BAD_FORMAT 11L
		DRP_EACCES,		// #define ERROR_INVALID_ACCESS 12L
		DRP_EINVAL,		// #define ERROR_INVALID_DATA 13L
		DRP_ENOMEM,		// #define ERROR_OUTOFMEMORY 14L
		DRP_ENOENT,		// #define ERROR_INVALID_DRIVE 15L
		DRP_ENOENT,		// #define ERROR_CURRENT_DIRECTORY 16L
		DRP_EXDEV,		// #define ERROR_NOT_SAME_DEVICE 17L
		DRP_EAGAIN,		// #define ERROR_NO_MORE_FILES 18L
		DRP_EROFS,		// #define ERROR_WRITE_PROTECT 19L
		-1,		// #define ERROR_BAD_UNIT 20L
		-1,		// #define ERROR_NOT_READY 21L
		-1,		// #define ERROR_BAD_COMMAND 22L
		-1,		// #define ERROR_CRC 23L
		-1,		// #define ERROR_BAD_LENGTH 24L
		-1,		// #define ERROR_SEEK 25L
		-1,		// #define ERROR_NOT_DOS_DISK 26L
		-1,		// #define ERROR_SECTOR_NOT_FOUND 27L
		-1,		// #define ERROR_OUT_OF_PAPER 28L
		-1,		// #define ERROR_WRITE_FAULT 29L
		-1,		// #define ERROR_READ_FAULT 30L
		-1,		// #define ERROR_GEN_FAILURE 31L
		-1,		// #define ERROR_SHARING_VIOLATION 32L
		-1,		// #define ERROR_LOCK_VIOLATION 33L
		-1,		// #define ERROR_WRONG_DISK 34L
		-1,		// #define ERROR_SHARING_BUFFER_EXCEEDED 36L
		-1,		// #define ERROR_HANDLE_EOF 38L
		-1,		// #define ERROR_HANDLE_DISK_FULL 39L
		-1,		// #define ERROR_NOT_SUPPORTED 50L
		-1,		// #define ERROR_REM_NOT_LIST 51L
		-1,		// #define ERROR_DUP_NAME 52L
		-1,		// #define ERROR_BAD_NETPATH 53L
		-1,		// #define ERROR_NETWORK_BUSY 54L
		-1,		// #define ERROR_DEV_NOT_EXIST 55L
		-1,		// #define ERROR_TOO_MANY_CMDS 56L
		-1,		// #define ERROR_ADAP_HDW_ERR 57L
		-1,		// #define ERROR_BAD_NET_RESP 58L
		-1,		// #define ERROR_UNEXP_NET_ERR 59L
		-1,		// #define ERROR_BAD_REM_ADAP 60L
		-1,		// #define ERROR_PRINTQ_FULL 61L
		-1,		// #define ERROR_NO_SPOOL_SPACE 62L
		-1,		// #define ERROR_PRINT_CANCELLED 63L
		-1,		// #define ERROR_NETNAME_DELETED 64L
		-1,		// #define ERROR_NETWORK_ACCESS_DENIED 65L
		-1,		// #define ERROR_BAD_DEV_TYPE 66L
		-1,		// #define ERROR_BAD_NET_NAME 67L
		-1,		// #define ERROR_TOO_MANY_NAMES 68L
		-1,		// #define ERROR_TOO_MANY_SESS 69L
		-1,		// #define ERROR_SHARING_PAUSED 70L
		-1,		// #define ERROR_REQ_NOT_ACCEP 71L
		-1,		// #define ERROR_REDIR_PAUSED 72L
		-1,		// #define ERROR_FILE_EXISTS 80L
		-1,		// #define ERROR_CANNOT_MAKE 82L
		-1,		// #define ERROR_FAIL_I24 83L
		-1,		// #define ERROR_OUT_OF_STRUCTURES 84L
		-1,		// #define ERROR_ALREADY_ASSIGNED 85L
		-1,		// #define ERROR_INVALID_PASSWORD 86L
		-1,		// #define ERROR_INVALID_PARAMETER 87L
		-1,		// #define ERROR_NET_WRITE_FAULT 88L
		-1,		// #define ERROR_NO_PROC_SLOTS 89L
		-1,		// #define ERROR_TOO_MANY_SEMAPHORES 100L
		-1,		// #define ERROR_EXCL_SEM_ALREADY_OWNED 101L
		-1,		// #define ERROR_SEM_IS_SET 102L
		-1,		// #define ERROR_TOO_MANY_SEM_REQUESTS 103L
		-1,		// #define ERROR_INVALID_AT_INTERRUPT_TIME 104L
		-1,		// #define ERROR_SEM_OWNER_DIED 105L
		-1,		// #define ERROR_SEM_USER_LIMIT 106L
		-1,		// #define ERROR_DISK_CHANGE 107L
		-1,		// #define ERROR_DRIVE_LOCKED 108L
		-1,		// #define ERROR_BROKEN_PIPE 109L
		-1,		// #define ERROR_OPEN_FAILED 110L
		-1,		// #define ERROR_BUFFER_OVERFLOW 111L
		-1,		// #define ERROR_DISK_FULL 112L
		-1,		// #define ERROR_NO_MORE_SEARCH_HANDLES 113L
		-1,		// #define ERROR_INVALID_TARGET_HANDLE 114L
		-1,		// #define ERROR_INVALID_CATEGORY 117L
		-1,		// #define ERROR_INVALID_VERIFY_SWITCH 118L
		-1,		// #define ERROR_BAD_DRIVER_LEVEL 119L
		-1,		// #define ERROR_CALL_NOT_IMPLEMENTED 120L
		-1,		// #define ERROR_SEM_TIMEOUT 121L
		-1,		// #define ERROR_INSUFFICIENT_BUFFER 122L
		-1,		// #define ERROR_INVALID_NAME 123L
		-1,		// #define ERROR_INVALID_LEVEL 124L
		-1,		// #define ERROR_NO_VOLUME_LABEL 125L
		-1,		// #define ERROR_MOD_NOT_FOUND 126L
		-1,		// #define ERROR_PROC_NOT_FOUND 127L
		-1,		// #define ERROR_WAIT_NO_CHILDREN 128L
		-1,		// #define ERROR_CHILD_NOT_COMPLETE 129L
		-1,		// #define ERROR_DIRECT_ACCESS_HANDLE 130L
		-1,		// #define ERROR_NEGATIVE_SEEK 131L
		-1,		// #define ERROR_SEEK_ON_DEVICE 132L
		-1,		// #define ERROR_IS_JOIN_TARGET 133L
		-1,		// #define ERROR_IS_JOINED 134L
		-1,		// #define ERROR_IS_SUBSTED 135L
		-1,		// #define ERROR_NOT_JOINED 136L
		-1,		// #define ERROR_NOT_SUBSTED 137L
		-1,		// #define ERROR_JOIN_TO_JOIN 138L
		-1,		// #define ERROR_SUBST_TO_SUBST 139L
		-1,		// #define ERROR_JOIN_TO_SUBST 140L
		-1,		// #define ERROR_SUBST_TO_JOIN 141L
		-1,		// #define ERROR_BUSY_DRIVE 142L
		-1,		// #define ERROR_SAME_DRIVE 143L
		-1,		// #define ERROR_DIR_NOT_ROOT 144L
		-1,		// #define ERROR_DIR_NOT_EMPTY 145L
		-1,		// #define ERROR_IS_SUBST_PATH 146L
		-1,		// #define ERROR_IS_JOIN_PATH 147L
		-1,		// #define ERROR_PATH_BUSY 148L
		-1,		// #define ERROR_IS_SUBST_TARGET 149L
		-1,		// #define ERROR_SYSTEM_TRACE 150L
		-1,		// #define ERROR_INVALID_EVENT_COUNT 151L
		-1,		// #define ERROR_TOO_MANY_MUXWAITERS 152L
		-1,		// #define ERROR_INVALID_LIST_FORMAT 153L
		-1,		// #define ERROR_LABEL_TOO_LONG 154L
		-1,		// #define ERROR_TOO_MANY_TCBS 155L
		-1,		// #define ERROR_SIGNAL_REFUSED 156L
		-1,		// #define ERROR_DISCARDED 157L
		-1,		// #define ERROR_NOT_LOCKED 158L
		-1,		// #define ERROR_BAD_THREADID_ADDR 159L
		-1,		// #define ERROR_BAD_ARGUMENTS 160L
		-1,		// #define ERROR_BAD_PATHNAME 161L
		-1,		// #define ERROR_SIGNAL_PENDING 162L
		-1,		// #define ERROR_MAX_THRDS_REACHED 164L
		-1,		// #define ERROR_LOCK_FAILED 167L
		-1,		// #define ERROR_BUSY 170L
		-1,		// #define ERROR_CANCEL_VIOLATION 173L
		-1,		// #define ERROR_ATOMIC_LOCKS_NOT_SUPPORTED 174L
		-1,		// #define ERROR_INVALID_SEGMENT_NUMBER 180L
		-1,		// #define ERROR_INVALID_ORDINAL 182L
		-1,		// #define ERROR_ALREADY_EXISTS 183L
		-1,		// #define ERROR_INVALID_FLAG_NUMBER 186L
		-1,		// #define ERROR_SEM_NOT_FOUND 187L
		-1,		// #define ERROR_INVALID_STARTING_CODESEG 188L
		-1,		// #define ERROR_INVALID_STACKSEG 189L
		-1,		// #define ERROR_INVALID_MODULETYPE 190L
		-1,		// #define ERROR_INVALID_EXE_SIGNATURE 191L
		-1,		// #define ERROR_EXE_MARKED_INVALID 192L
		-1,		// #define ERROR_BAD_EXE_FORMAT 193L
		-1,		// #define ERROR_ITERATED_DATA_EXCEEDS_64k 194L
		-1,		// #define ERROR_INVALID_MINALLOCSIZE 195L
		-1,		// #define ERROR_DYNLINK_FROM_INVALID_RING 196L
		-1,		// #define ERROR_IOPL_NOT_ENABLED 197L
		-1,		// #define ERROR_INVALID_SEGDPL 198L
		-1,		// #define ERROR_AUTODATASEG_EXCEEDS_64k 199L
		-1,		// #define ERROR_RING2SEG_MUST_BE_MOVABLE 200L
		-1,		// #define ERROR_RELOC_CHAIN_XEEDS_SEGLIM 201L
		-1,		// #define ERROR_INFLOOP_IN_RELOC_CHAIN 202L
		-1,		// #define ERROR_ENVVAR_NOT_FOUND 203L
		-1,		// #define ERROR_NO_SIGNAL_SENT 205L
		-1,		// #define ERROR_FILENAME_EXCED_RANGE 206L
		-1,		// #define ERROR_RING2_STACK_IN_USE 207L
		-1,		// #define ERROR_META_EXPANSION_TOO_LONG 208L
		-1,		// #define ERROR_INVALID_SIGNAL_NUMBER 209L
		-1,		// #define ERROR_THREAD_1_INACTIVE 210L
		-1,		// #define ERROR_LOCKED 212L
		-1,		// #define ERROR_TOO_MANY_MODULES 214L
		-1,		// #define ERROR_NESTING_NOT_ALLOWED 215L
		-1,		// #define ERROR_EXE_MACHINE_TYPE_MISMATCH 216L
		-1,		// #define ERROR_EXE_CANNOT_MODIFY_SIGNED_BINARY 217L
		-1,		// #define ERROR_EXE_CANNOT_MODIFY_STRONG_SIGNED_BINARY 218L
		-1,		// #define ERROR_BAD_PIPE 230L
		-1,		// #define ERROR_PIPE_BUSY 231L
		-1,		// #define ERROR_NO_DATA 232L
		-1,		// #define ERROR_PIPE_NOT_CONNECTED 233L
		-1,		// #define ERROR_MORE_DATA 234L
		-1,		// #define ERROR_VC_DISCONNECTED 240L
		-1,		// #define ERROR_INVALID_EA_NAME 254L
		-1,		// #define ERROR_EA_LIST_INCONSISTENT 255L
	};

	if ((unsigned)error < sizeof(error_map0)/sizeof(error_map0[0])) {
		if (error_map0[error] >= 0) {
			return error_map0[error];
		}
	}
	{
		char msg[64];
		sprintf(msg, "Unmapped error %d", error);
		Fatal::failedTodo(msg, __FILE__, __LINE__, DR_FUNCTION);
	}
	return DRP_ENOTSUP;
}

void File_sysiface_wnt::throwSysException(File *handle, const String &operation, int err)
{
	int perr = mapWntErrorToPosix(err);
	switch (perr) {
	case DRP_EAGAIN:
		xthrownew(NonblockExcept(handle, operation, perr, err));
		break;

	default:
		xthrownew(IoExcept(handle, operation, perr, err));
		break;
	}
}

void File_sysiface_wnt::throwFileStaticSysException(const String &filename, const String &operation, int err)
{
	int perr = mapWntErrorToPosix(err);
	switch (perr) {
	case DRP_ENOENT:
		xthrownew(FileNotFoundExcept(filename, operation, perr, err));
		break;

	default:
		xthrownew(FileFailedExcept(filename, operation, perr, err));
		break;
	}
}


DR_IO_NS_END
