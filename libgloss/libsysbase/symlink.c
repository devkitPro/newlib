#include <unistd.h>
#include <reent.h>
#include <errno.h>
#include <sys/iosupport.h>

int symlink_r(struct _reent *r, const char *target, const char *linkpath) {
	int ret=-1;
 	int targetDevice = FindDevice(target);
 	int linkDevice = FindDevice(linkpath);

	if (targetDevice != linkDevice) {
		return -1;
	}

	if ( targetDevice != -1 && devoptab_list[targetDevice]->symlink_r) {

		r->deviceData = devoptab_list[targetDevice]->deviceData;
		ret = devoptab_list[targetDevice]->symlink_r(r, target, linkpath );

	} else {
		r->_errno = ENOSYS;
	}

	return ret;
}

int symlink(const char *target, const char *linkpath) {
	return (symlink_r(_REENT, target, linkpath));
}
