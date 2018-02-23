/*
 * Stub version of fork.
 */

#include "config.h"
#include <_ansi.h>
#include <_syslist.h>
#include <errno.h>
#ifdef REENTRANT_SYSCALLS_PROVIDED
//---------------------------------------------------------------------------------
int
_DEFUN(_fork_r,(r),
	   struct _reent * r) {
//---------------------------------------------------------------------------------
#else
//---------------------------------------------------------------------------------
int
_DEFUN (_fork, (),
        _NOARGS) {
//---------------------------------------------------------------------------------
	struct _reent *r = _REENT;
#endif
	r->_errno = ENOSYS;
	return -1;
}
