#include "config.h"
#include <_ansi.h>
#include <_syslist.h>
#include <reent.h>
#include <errno.h>
#include <sys/iosupport.h>

int	_rmdir_r (struct _reent *ptr, const char *name) {
	int	dev,ret=-1;

	dev	= FindDevice(name);
	if(dev!=-1) {
		if(devoptab_list[dev]->rmdir_r) {
			ptr->deviceData = devoptab_list[dev]->deviceData;
			ret = devoptab_list[dev]->rmdir_r(ptr,name);
		} else {
			ptr->_errno = ENOSYS;
		}
	} else {
		ptr->_errno =	ENODEV;
	}

	return ret;
}

#ifndef _REENT_ONLY

int
rmdir (const char *filename)
{
  return _rmdir_r (_REENT, filename);
}

#endif
