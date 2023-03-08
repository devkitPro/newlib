/*
 * Stub version of wait.
 */

#include "config.h"
#include <_ansi.h>
#include <_syslist.h>
#include <errno.h>

//---------------------------------------------------------------------------------
int _wait_r(struct _reent *r, int  *status) {
//---------------------------------------------------------------------------------
	r->_errno = ENOSYS;
	return -1;
}

