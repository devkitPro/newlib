#include "config.h"
#include <_ansi.h>
#include <_syslist.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/iosupport.h>
#include <errno.h>


#ifdef REENTRANT_SYSCALLS_PROVIDED
//---------------------------------------------------------------------------------
int _DEFUN (_stat_r,(r,file, st),
			struct _reent * r _AND
			const char *file _AND
			struct stat *st) {
//---------------------------------------------------------------------------------
#else
//---------------------------------------------------------------------------------
int
_DEFUN (_stat, (file, st),
        const char  *file _AND
        struct stat *st)
{
//---------------------------------------------------------------------------------
	struct _reent *r = _REENT;
#endif
	int dev,ret;

	dev = FindDevice(file);

	if(dev!=-1) {
		if (devoptab_list[dev]->stat_r) {
			r->deviceData = devoptab_list[dev]->deviceData;
			ret = devoptab_list[dev]->stat_r(r,file,st);
		} else {
			r->_errno=ENOSYS;
		}
	} else {
		ret = -1;
		r->_errno = ENODEV;
	}
	return ret;
}

