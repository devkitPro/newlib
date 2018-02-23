#include "config.h"
#include <_ansi.h>
#include <_syslist.h>
#include <errno.h>

#include <sys/iosupport.h>

#include <sys/iosupport.h>

//---------------------------------------------------------------------------------
#ifdef REENTRANT_SYSCALLS_PROVIDED
//---------------------------------------------------------------------------------
_off_t _DEFUN (_lseek_r, (r, fileDesc, pos, dir),
           struct _reent * r _AND
           int   fileDesc  _AND
		   _off_t   pos   _AND
           int   dir) {
//---------------------------------------------------------------------------------
#else
//---------------------------------------------------------------------------------
_off_t _DEFUN (_lseek,(fileDesc, pos, dir),
           int   fileDesc  _AND
		   _off_t   pos   _AND
           int   dir) {
//---------------------------------------------------------------------------------
	struct _reent *r = _REENT;
//---------------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------------
	_off_t ret = -1;
	unsigned int dev = 0;

	__handle * handle;

	if(fileDesc!=-1) {

		handle = __get_handle(fileDesc);

		if ( NULL == handle ) return ret;

		dev = handle->device;

		if(devoptab_list[dev]->seek_r) {
			r->deviceData = devoptab_list[dev]->deviceData;
			ret = devoptab_list[dev]->seek_r( r, handle->fileStruct, pos, dir);
		} else
			r->_errno=ENOSYS;

	}
	return ret;

}
