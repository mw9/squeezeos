/* $Id: statfs.h 1763 2006-09-22 01:22:23Z dean $ */
#ifndef _SPARC_STATFS_H
#define _SPARC_STATFS_H

#ifndef __KERNEL_STRICT_NAMES

#include <linux/types.h>

typedef __kernel_fsid_t	fsid_t;

#endif

struct statfs {
	long f_type;
	long f_bsize;
	long f_blocks;
	long f_bfree;
	long f_bavail;
	long f_files;
	long f_ffree;
	__kernel_fsid_t f_fsid;
	long f_namelen;  /* SunOS ignores this field. */
	long f_spare[6];
};

#endif
