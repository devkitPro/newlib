#include "config.h"
#include <_ansi.h>
#include <_syslist.h>
#include <errno.h>

#ifdef REENTRANT_SYSCALLS_PROVIDED
//---------------------------------------------------------------------------------
int _DEFUN(	_getpid_r,(ptr),
						struct _reent *ptr) {
//---------------------------------------------------------------------------------
#else
//---------------------------------------------------------------------------------
int _DEFUN (_getpid, (),
						_NOARGS) {
//---------------------------------------------------------------------------------
	struct _reent *ptr = _REENT;
#endif
	ptr->_errno = ENOSYS;
	return -1;
}

