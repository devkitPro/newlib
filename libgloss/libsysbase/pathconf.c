#include <errno.h>
#include <stddef.h>
#include <unistd.h>
#include <limits.h>

long pathconf(const char *path, int name) {
	return fpathconf(-1, name);
}

long fpathconf(int fd, int name)
{
	struct _reent *r = _REENT;

	switch (name) {
		case _PC_LINK_MAX:
			return 1;
		case _PC_MAX_CANON:
			return MAX_CANON;
		case _PC_MAX_INPUT:
			return MAX_INPUT;
		case _PC_NAME_MAX:
			return NAME_MAX;
		case _PC_PATH_MAX:
			return PATH_MAX;
		case _PC_PIPE_BUF:
			return PIPE_BUF;
		case _PC_CHOWN_RESTRICTED:
			break; // chown is not implemented
		case _PC_NO_TRUNC:
			return 1;
		case _PC_VDISABLE:
			break; // termios is not supported
		case _PC_SYNC_IO:
			return 1;
		case _PC_ASYNC_IO:
		case _PC_PRIO_IO:
			break;
		case _PC_FILESIZEBITS:
			return 64;
		case _PC_REC_INCR_XFER_SIZE:
		case _PC_REC_MAX_XFER_SIZE:
		case _PC_REC_MIN_XFER_SIZE:
		case _PC_REC_XFER_ALIGN:
		case _PC_ALLOC_SIZE_MIN:
		case _PC_SYMLINK_MAX:
			break;
		case _PC_2_SYMLINKS:
			return 0;
	}

	r->_errno = EINVAL;
	return -1;
}
