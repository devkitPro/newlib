/*
 * Stub version of execve.
 */

#include "config.h"
#include <_ansi.h>
#include <_syslist.h>
#include <errno.h>

#ifdef REENTRANT_SYSCALLS_PROVIDED
//---------------------------------------------------------------------------------
int
_DEFUN (_execve_r, (r, name, argv, env),
		struct _reent * r _AND
        char  *name  _AND
        char **argv  _AND
        char **env) {
//---------------------------------------------------------------------------------
#else
//---------------------------------------------------------------------------------
int
_DEFUN (_execve, (name, argv, env),
        char  *name  _AND
        char **argv  _AND
        char **env) {
//---------------------------------------------------------------------------------
	struct _reent *r = _REENT;
#endif
	r->_errno = ENOSYS;
	return -1;
}

