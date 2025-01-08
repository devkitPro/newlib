/*
 * Stub version of getentropy.
 */

#include "config.h"
#include <_ansi.h>
#include <_syslist.h>
#include <errno.h>

//---------------------------------------------------------------------------------
int _getentropy_r(struct _reent *r, void *buf, size_t buflen) {
//---------------------------------------------------------------------------------
	r->_errno = ENOSYS;
	return -1;
}
