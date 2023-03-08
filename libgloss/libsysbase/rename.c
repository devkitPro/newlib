#include "config.h"
#include <reent.h>
#include <sys/iosupport.h>
#include <errno.h>

//---------------------------------------------------------------------------------
int _rename_r( struct _reent *ptr, const char *existing, const char *newName) {
//---------------------------------------------------------------------------------
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
