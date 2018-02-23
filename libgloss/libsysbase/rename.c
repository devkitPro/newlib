#include "config.h"
#include <reent.h>
#include <sys/iosupport.h>
#include <errno.h>

#ifdef REENTRANT_SYSCALLS_PROVIDED
int
_DEFUN (_rename_r, (ptr, existing, newName),
     struct _reent *ptr _AND
     _CONST char *existing _AND
     _CONST char *newName)
{
#else
int
_DEFUN(rename, (existing, newName),
       _CONST char *existing _AND
       _CONST char *newName)
{
	struct _reent *ptr = _REENT;
#endif
	struct _reent *r = _REENT;

	int ret;
	int sourceDev = FindDevice(existing);
	int destDev = FindDevice(newName);

	ret = -1;

	if ( sourceDev == destDev) {
		if (devoptab_list[destDev]->rename_r) {
			r->deviceData = devoptab_list[destDev]->deviceData;
			ret = devoptab_list[destDev]->rename_r( r, existing, newName);
		} else {
			r->_errno = ENOSYS;
		}
	} else {
		r->_errno = EXDEV;
	}

	return ret;
}
