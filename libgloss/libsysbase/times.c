#include "config.h"
#include <_ansi.h>
#include <_syslist.h>
#include <sys/times.h>
#include <errno.h>

//---------------------------------------------------------------------------------
clock_t _times_r(struct _reent *r, struct tms *ptms) {
//---------------------------------------------------------------------------------
	r->_errno = ENOSYS;
	return (clock_t)-1;
}

