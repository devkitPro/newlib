#include "config.h"
#include <_ansi.h>
#include <_syslist.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/iosupport.h>
#include <errno.h>

//---------------------------------------------------------------------------------
int _gettimeofday_r( struct _reent *ptr, struct timeval *ptimeval, void *ptimezone) {
//---------------------------------------------------------------------------------
	if ( __has_syscall(gettod_r) ) return __syscall_gettod_r(ptr, ptimeval, ptimezone);

	ptr->_errno = ENOSYS;
	return -1;

}

