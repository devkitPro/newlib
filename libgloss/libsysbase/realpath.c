#include <errno.h>
#include <stddef.h>
#include <unistd.h>
#include <limits.h>
#include <reent.h>
#include <sys/iosupport.h>

char *realpath(const char *path, char *resolved_path)
{
        int ret = -1;
        unsigned int dev = FindDevice(path);

	struct _reent *r = _REENT;

        if(dev != -1 && devoptab_list[dev]->pathconf_r) {
                r->deviceData = devoptab_list[dev]->deviceData;
                ret = devoptab_list[dev]->pathconf_r(r, path, resolved_path);
        } else
                r->_errno=ENOSYS;

        return ret;
}

