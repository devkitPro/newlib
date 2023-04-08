#include <unistd.h>
#include <reent.h>
#include <errno.h>
#include <sys/iosupport.h>

ssize_t readlink_r(struct _reent *r, const char *path, char *buf, size_t bufsiz) {
        int ret=-1;
        int device = FindDevice(path);

        if ( device != -1 && devoptab_list[device]->readlink_r) {

                r->deviceData = devoptab_list[device]->deviceData;
                ret = devoptab_list[device]->readlink_r(r, path, buf, bufsiz);

        } else {
                r->_errno = ENOSYS;
        }

        return ret;
}

ssize_t readlink(const char *path, char *buf, size_t bufsiz) {
	return(readlink_r(_REENT, path, buf, bufsiz));
}
