#include "config.h"
#include <_ansi.h>
#include <_syslist.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>

#include <sys/iosupport.h>

#ifdef REENTRANT_SYSCALLS_PROVIDED
//---------------------------------------------------------------------------------
int _DEFUN (_open_r, (r, file, flags, mode),
			struct _reent * r _AND
			const char *file  _AND
			int   flags _AND
			int   mode) {
//---------------------------------------------------------------------------------
#else
//---------------------------------------------------------------------------------
int _DEFUN (_open, (file, flags, mode),
        const char *file  _AND
        int   flags _AND
        int   mode) {
//---------------------------------------------------------------------------------
	struct _reent *r = _REENT;
#endif
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
