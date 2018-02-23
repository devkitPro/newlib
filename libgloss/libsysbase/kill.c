/*
 * Stub version of kill.
 */

#include "config.h"
#include <_ansi.h>
#include <_syslist.h>
#include <errno.h>

#ifdef REENTRANT_SYSCALLS_PROVIDED
int _DEFUN(_kill_r,(ptr,pid,sig),
		   struct _reent *ptr _AND
           int			 pid  _AND
           int			 sig) {
#else
int _DEFUN(_kill,(pid,sig),
           int pid  _AND
           int sig)	{
	struct _reent *ptr = _REENT;
#endif
  ptr->_errno = ENOSYS;
  return -1;
}
