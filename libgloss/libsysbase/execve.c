/*
 * Stub version of execve.
 */

#include "config.h"
#include <_ansi.h>
#include <_syslist.h>
#include <errno.h>

//---------------------------------------------------------------------------------
int _execve_r( struct _reent * r, char *name, char **argv, char **env) {
//---------------------------------------------------------------------------------
	r->_errno = ENOSYS;
	return -1;
}

