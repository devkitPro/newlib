#include "config.h"
#include <sys/iosupport.h>
#include <sys/errno.h>

#ifdef REENTRANT_SYSCALLS_PROVIDED
//---------------------------------------------------------------------------------
void * _sbrk_r (struct _reent *ptr, ptrdiff_t incr) {
//---------------------------------------------------------------------------------
#else
//---------------------------------------------------------------------------------
caddr_t _sbrk (int incr) {
//---------------------------------------------------------------------------------
	struct _reent *ptr = _REENT;
#endif


	if ( __has_syscall(sbrk_r) ) {
		return __syscall_sbrk_r(ptr, incr);
	} else {

		ptr->_errno = ENOMEM;
		return (caddr_t) -1;

	}

}