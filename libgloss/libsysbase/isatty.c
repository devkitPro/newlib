#include "config.h"
#include <_ansi.h>
#include <_syslist.h>
#include <errno.h>

#ifdef REENTRANT_SYSCALLS_PROVIDED
//---------------------------------------------------------------------------------
int _DEFUN(_isatty_r,(ptr,file),
		   struct _reent *ptr _AND
           int file) {
//---------------------------------------------------------------------------------
#else
//---------------------------------------------------------------------------------
int _DEFUN(_isatty,(file),
           int file) {
//---------------------------------------------------------------------------------
	struct _reent *ptr = _REENT;
#endif
	return 0;
}
