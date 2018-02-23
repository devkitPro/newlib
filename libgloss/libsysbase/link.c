#include "config.h"
#include <_ansi.h>
#include <_syslist.h>
#include <reent.h>
#include <errno.h>
#include <sys/iosupport.h>

#ifdef REENTRANT_SYSCALLS_PROVIDED
int _DEFUN (_link_r, (r, existing, new),
					struct _reent *r _AND
					const char *existing _AND
					const char *new) {
#else
int _DEFUN (_link,	(existing, new),
					const char *existing _AND
					const char *new) {
	struct _reent *r = _REENT;
#endif
	int ret;
	int sourceDev = FindDevice(existing);
	int destDev = FindDevice(new);

	ret = -1;

	if ( sourceDev == destDev) {
		if (devoptab_list[destDev]->link_r) {
			r->deviceData = devoptab_list[destDev]->deviceData;
			ret = devoptab_list[destDev]->link_r( r, existing, new);
		} else {
			r->_errno = ENOSYS;
		}
	} else {
		r->_errno = EXDEV;
	}

	return ret;
}

