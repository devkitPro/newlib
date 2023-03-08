#include "config.h"
#include <_ansi.h>
#include <_syslist.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/iosupport.h>
#include <errno.h>


//---------------------------------------------------------------------------------
int _stat_r(struct _reent *r, const char *file, struct stat *st) {
//---------------------------------------------------------------------------------
	int dev,ret=-1;

	dev = FindDevice(file);

	if(dev!=-1) {
		if (devoptab_list[dev]->stat_r) {
			r->deviceData = devoptab_list[dev]->deviceData;
			ret = devoptab_list[dev]->stat_r(r,file,st);
		} else {
			r->_errno=ENOSYS;
		}
	} else {
		r->_errno = ENODEV;
	}
	return ret;
}

