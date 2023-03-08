#include "config.h"
#include <_ansi.h>
#include <_syslist.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>

#include <sys/iosupport.h>

//---------------------------------------------------------------------------------
int _open_r(struct _reent *r, const char *file, int flags, int mode) {
//---------------------------------------------------------------------------------
	__handle *handle;
	int dev, fd, ret;

	dev = FindDevice(file);

	fd = -1;
	if(dev!=-1) {
		if (devoptab_list[dev]->open_r) {
			fd = __alloc_handle(dev);

			if ( -1 != fd ) {
				handle = __get_handle(fd);

				r->deviceData = devoptab_list[dev]->deviceData;

				ret = devoptab_list[dev]->open_r(r, handle->fileStruct, file, flags, mode);

				if ( ret == -1 ) {
					__release_handle(fd);
					fd = -1;
				}
			} else {
				r->_errno = ENOSR;
			}
		} else {
			r->_errno=ENOSYS;
		}
	} else {
		r->_errno = ENOSYS;
	}

	return fd;
}
