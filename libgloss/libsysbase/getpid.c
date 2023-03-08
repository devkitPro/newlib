#include "config.h"
#include <_ansi.h>
#include <_syslist.h>
#include <errno.h>

//---------------------------------------------------------------------------------
int _getpid_r(struct _reent *ptr) {
//---------------------------------------------------------------------------------
	ptr->_errno = ENOSYS;
	return -1;
}

