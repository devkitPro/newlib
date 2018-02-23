/*
 * Stub version of wait.
 */

#include "config.h"
#include <_ansi.h>
#include <_syslist.h>
#include <errno.h>

//---------------------------------------------------------------------------------
#ifdef REENTRANT_SYSCALLS_PROVIDED
//---------------------------------------------------------------------------------
int
_DEFUN (_wait_r, (r, status),
		struct _reent *r _AND
		int  *status) {

#else
//---------------------------------------------------------------------------------
int
_DEFUN (_wait, (status),
	int  *status) {
	struct _reent *r = _REENT;
#endif
//---------------------------------------------------------------------------------
	r->_errno = ENOSYS;
	return -1;
}

