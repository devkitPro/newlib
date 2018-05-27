#include "config.h"
#include <_ansi.h>
#include <_syslist.h>
#include <stdio.h>
#include <errno.h>

#include <sys/iosupport.h>

#ifdef REENTRANT_SYSCALLS_PROVIDED
//---------------------------------------------------------------------------------
int _close_r( struct _reent *ptr, int fileDesc) {
//---------------------------------------------------------------------------------
#else
//---------------------------------------------------------------------------------
int _close( int fileDesc) {
//---------------------------------------------------------------------------------
	struct _reent *ptr = _REENT;
#endif
	int ret = -1;
	unsigned int dev = 0;

	if(fileDesc!=-1) {

		__handle *handle = __get_handle(fileDesc);

		if ( handle != NULL) {
			dev = handle->device;
			handle->refcount--;
			if (handle->refcount == 0 ) {

				if(devoptab_list[dev]->close_r) {
					ptr->deviceData = devoptab_list[dev]->deviceData;
					ret = devoptab_list[dev]->close_r(ptr,handle->fileStruct);
				}
				else
					ret = 0;

				__release_handle(fileDesc);
			} else {
				ret = 0;
			}

		}
	}
	return ret;
}
