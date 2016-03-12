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

#ifndef dr__pe_error__hxx__
# define dr__pe_error__hxx__

#define	DRP_EPERM		1		/* Not super-user */
#define	DRP_ENOENT		2		/* No such file or directory */
#define	DRP_ESRCH		3		/* No such process */
#define	DRP_EINTR		4		/* Interrupted system call */
#define	DRP_EIO			5		/* I/O error */
#define	DRP_ENXIO		6		/* No such device or address */
#define	DRP_E2BIG		7		/* Arg list too long */
#define	DRP_ENOEXEC		8		/* Exec format error */
#define	DRP_EBADF		9		/* Bad file number */
#define	DRP_ECHILD		10		/* No children */
#define	DRP_EAGAIN		11		/* No more processes */
#define	DRP_ENOMEM		12		/* Not enough core */
#define	DRP_EACCES		13		/* Permission denied */
#define	DRP_EFAULT		14		/* Bad address */
#define	DRP_ENOTBLK		15		/* Block device required */
#define	DRP_EBUSY		16		/* Mount device busy */
#define	DRP_EEXIST		17		/* File exists */
#define	DRP_EXDEV		18		/* Cross-device link */
#define	DRP_ENODEV		19		/* No such device */
#define	DRP_ENOTDIR		20		/* Not a directory */
#define	DRP_EISDIR		21		/* Is a directory */
#define	DRP_EINVAL		22		/* Invalid argument */
#define	DRP_ENFILE		23		/* Too many open files in system */
#define	DRP_EMFILE		24		/* Too many open files */
#define	DRP_ENOTTY		25		/* Not a typewriter */
#define	DRP_ETXTBSY		26		/* Text file busy */
#define	DRP_EFBIG		27		/* File too large */
#define	DRP_ENOSPC		28		/* No space left on device */
#define	DRP_ESPIPE		29		/* Illegal seek */
#define	DRP_EROFS		30		/* Read only file system */
#define	DRP_EMLINK		31		/* Too many links */
#define	DRP_EPIPE		32		/* Broken pipe */
#define	DRP_EDOM		33		/* Math arg out of domain of func */
#define	DRP_ERANGE		34		/* Math result not representable */
#define	DRP_ENOMSG		35		/* No message of desired type */
#define	DRP_EIDRM		36		/* Identifier removed */
#define	DRP_ECHRNG		37		/* Channel number out of range */
#define	DRP_EL2NSYNC		38		/* Level 2 not synchronized */
#define	DRP_EL3HLT		39		/* Level 3 halted */
#define	DRP_EL3RST		40		/* Level 3 reset */
#define	DRP_ELNRNG		41		/* Link number out of range */
#define	DRP_EUNATCH		42		/* Protocol driver not attached */
#define	DRP_ENOCSI		43		/* No CSI structure available */
#define	DRP_EL2HLT		44		/* Level 2 halted */
#define	DRP_EDEADLK		45		/* Deadlock condition */
#define	DRP_ENOLCK		46		/* No record locks available */
#define	DRP_EBADE		50		/* Invalid exchange */
#define	DRP_EBADR		51		/* Invalid request descriptor */
#define	DRP_EXFULL		52		/* Exchange full */
#define	DRP_ENOANO		53		/* No anode */
#define	DRP_EBADRQC		54		/* Invalid request code */
#define	DRP_EBADSLT		55		/* Invalid slot */
#define	DRP_EDEADLOCK		56		/* File locking deadlock error */
#define	DRP_EBFONT		57		/* Bad font file fmt */
#define	DRP_ENOSTR		60		/* Device not a stream */
#define	DRP_ENODATA		61		/* No data (for no delay io) */
#define	DRP_ETIME		62		/* Timer expired */
#define	DRP_ENOSR		63		/* Out of streams resources */
#define	DRP_ENONET		64		/* Machine is not on the network */
#define	DRP_ENOPKG		65		/* Package not installed */
#define	DRP_EREMOTE		66		/* The object is remote */
#define	DRP_ENOLINK		67		/* The link has been severed */
#define	DRP_EADV		68		/* Advertise error */
#define	DRP_ESRMNT		69		/* Srmount error */
#define	DRP_ECOMM		70		/* Communication error on send */
#define	DRP_EPROTO		71		/* Protocol error */
#define	DRP_EMULTIHOP		74		/* Multihop attempted */
#define	DRP_ELBIN		75		/* Inode is remote (not really error) */
#define	DRP_EDOTDOT		76		/* Cross mount point (not really error) */
#define	DRP_EBADMSG		77		/* Trying to read unreadable message */
#define	DRP_EFTYPE		79		/* Inappropriate file type or format */
#define	DRP_ENOTUNIQ		80		/* Given log. name not unique */
#define	DRP_EBADFD		81		/* f.d. invalid for this operation */
#define	DRP_EREMCHG		82		/* Remote address changed */
#define	DRP_ELIBACC		83		/* Can't access a needed shared lib */
#define	DRP_ELIBBAD		84		/* Accessing a corrupted shared lib */
#define	DRP_ELIBSCN		85		/* .lib section in a.out corrupted */
#define	DRP_ELIBMAX		86		/* Attempting to link in too many libs */
#define	DRP_ELIBEXEC		87		/* Attempting to exec a shared library */
#define	DRP_ENOSYS		88		/* Function not implemented */
#define	DRP_ENMFILE		89		/* No more files */
#define	DRP_ENOTEMPTY		90		/* Directory not empty */
#define	DRP_ENAMETOOLONG	91		/* File or path name too long */
#define	DRP_ELOOP		92		/* Too many symbolic links */
#define	DRP_EOPNOTSUPP		95		/* Operation not supported on transport endpoint */
#define	DRP_EPFNOSUPPORT	96		/* Protocol family not supported */
#define	DRP_ECONNRESET		104		/* Connection reset by peer */
#define	DRP_ENOBUFS		105		/* No buffer space available */
#define	DRP_EAFNOSUPPORT	106		/* Address family not supported by protocol family */
#define	DRP_EPROTOTYPE		107		/* Protocol wrong type for socket */
#define	DRP_ENOTSOCK		108		/* Socket operation on non-socket */
#define	DRP_ENOPROTOOPT		109		/* Protocol not available */
#define	DRP_ESHUTDOWN		110		/* Can't send after socket shutdown */
#define	DRP_ECONNREFUSED	111		/* Connection refused */
#define	DRP_EADDRINUSE		112		/* Address already in use */
#define	DRP_ECONNABORTED	113		/* Connection aborted */
#define	DRP_ENETUNREACH		114		/* Network is unreachable */
#define	DRP_ENETDOWN		115		/* Network interface is not configured */
#define	DRP_ETIMEDOUT		116		/* Connection timed out */
#define	DRP_EHOSTDOWN		117		/* Host is down */
#define	DRP_EHOSTUNREACH	118		/* Host is unreachable */
#define	DRP_EINPROGRESS		119		/* Connection already in progress */
#define	DRP_EALREADY		120		/* Socket already connected */
#define	DRP_EDESTADDRREQ	121		/* Destination address required */
#define	DRP_EMSGSIZE		122		/* Message too long */
#define	DRP_EPROTONOSUPPORT	123		/* Unknown protocol */
#define	DRP_ESOCKTNOSUPPORT	124		/* Socket type not supported */
#define	DRP_EADDRNOTAVAIL	125		/* Address not available */
#define	DRP_ENETRESET		126
#define	DRP_EISCONN		127		/* Socket is already connected */
#define	DRP_ENOTCONN		128		/* Socket is not connected */
#define	DRP_ETOOMANYREFS	129
#define	DRP_EPROCLIM		130
#define	DRP_EUSERS		131
#define	DRP_EDQUOT		132
#define	DRP_ESTALE		133
#define	DRP_ENOTSUP		134		/* Not supported */
#define	DRP_ENOMEDIUM		135   		/* No medium (in tape drive) */
#define	DRP_ENOSHARE		136    		/* No such host or network path */
#define	DRP_ECASECLASH		137  		/* Filename exists with different case */
#define	DRP_EILSEQ		138
#define	DRP_EOVERFLOW		139		/* Value too large for defined data type */
#define	DRP_EWOULDBLOCK		DRP_EAGAIN	/* Operation would block */


#endif
