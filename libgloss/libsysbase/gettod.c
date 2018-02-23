#include "config.h"
#include <_ansi.h>
#include <_syslist.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/iosupport.h>
#include <errno.h>

#ifdef REENTRANT_SYSCALLS_PROVIDED
//---------------------------------------------------------------------------------
int
_DEFUN(_gettimeofday_r,(ptr,ptimeval,ptimezone),
			struct _reent *ptr _AND
			struct timeval *ptimeval _AND
			void *ptimezone)
{
//---------------------------------------------------------------------------------
#else
//---------------------------------------------------------------------------------
int
_DEFUN (_gettimeofday, (ptimeval, ptimezone),
        struct timeval  *ptimeval  _AND
        void *ptimezone)
{
//---------------------------------------------------------------------------------
	struct _reent *ptr = _REENT;
#endif

	if ( __syscalls.gettod_r ) return __syscalls.gettod_r(ptr, ptimeval, ptimezone);

	ptr->_errno = ENOSYS;
	return -1;

}

