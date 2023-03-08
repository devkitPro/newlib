#include "config.h"
#include <_ansi.h>
#include <_syslist.h>
#include <reent.h>
#include <errno.h>
#include <sys/iosupport.h>

//---------------------------------------------------------------------------------
int _link_r(struct _reent *r, const char *existing, const char *new) {
//---------------------------------------------------------------------------------
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

