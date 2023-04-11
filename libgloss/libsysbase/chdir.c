#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/iosupport.h>
#include <sys/param.h>

#include "concatenate.h"

/* CWD always start with "/" */
static char _current_working_directory [PATH_MAX] = "/";
static char temp_cwd [PATH_MAX];

int chdir (const char *path) {
	struct _reent *r = _REENT;

	int dev;
	const char *pathPosition;

	/* Make sure the path is short enough */
	if (strnlen (path, PATH_MAX) >= PATH_MAX) {
		r->_errno = ENAMETOOLONG;
		return -1;
	}

	if (strchr (path, ':') != NULL) {
		strncpy (temp_cwd, path, PATH_MAX);
		/* Move path past device name */
		path = strchr (path, ':') + 1;
	} else {
		strncpy (temp_cwd, _current_working_directory, PATH_MAX);
	}

	pathPosition = strchr (temp_cwd , ':');

	if (pathPosition == NULL) {
		pathPosition = temp_cwd;
	} else {
		pathPosition++;
	}

	/* Make sure the path starts in the root directory */
	if (pathPosition[0] != DIRECTORY_SEPARATOR_CHAR) {
		r->_errno = ENOENT;
		return -1;
	}

	/* Concatenate the path to the CWD */
	if (_concatenate_path (r, temp_cwd, path, PATH_MAX) == -1) {
		return -1;
	}

	/* Get device from path name */
	dev = FindDevice(temp_cwd);

	if (dev < 0) {
		r->_errno = ENODEV;
		return -1;
	}

	r->deviceData = devoptab_list[dev]->deviceData;

	if ( devoptab_list[dev]->chdir_r == NULL) {
		r->_errno = ENOSYS;
		return -1;
	}

	/* Try changing directories on the device */
	if (devoptab_list[dev]->chdir_r (r, temp_cwd) == -1) {
		return -1;
	}

	/* Since it worked, set the new CWD and default device */
	setDefaultDevice(dev);
	strncpy (_current_working_directory, temp_cwd, PATH_MAX);

	return 0;
}

char *getcwd(char *buf, size_t size) {

	struct _reent *r = _REENT;

	if (size == 0) {
		if (buf != NULL) {
			r->_errno = EINVAL;
			return NULL;
		}
		buf = malloc(PATH_MAX);
		size = PATH_MAX;
	}

	if (buf == NULL) {
		r->_errno = EINVAL;
		return NULL;
	}

	if ( size < (strnlen (_current_working_directory, PATH_MAX) + 1)) {
		r->_errno = ERANGE;
		return NULL;
	}

	strncpy (buf, _current_working_directory, size);

	return buf;
}
