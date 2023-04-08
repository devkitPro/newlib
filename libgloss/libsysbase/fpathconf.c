#include <errno.h>
#include <stddef.h>
#include <unistd.h>
#include <limits.h>
#include <reent.h>
#include <sys/iosupport.h>

long fpathconf(int fd, int name)
{
        int ret = -1;
        unsigned int dev = 0;

        __handle * handle = __get_handle(fd);
       struct _reent *r = _REENT;

        if ( NULL == handle ) {
                errno = EINVAL;
                return ret;
        }

        dev = handle->device;

        if(devoptab_list[dev]->fpathconf_r) {
                r->deviceData = devoptab_list[dev]->deviceData;
                ret = devoptab_list[dev]->fpathconf_r(r, fd, name);
        } else
                r->_errno=ENOSYS;

        return ret;
}
