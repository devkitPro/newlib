#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#define DIRECTORY_SEPARATOR_CHAR '/'
static const char DIRECTORY_THIS[] = ".";
static const char DIRECTORY_PARENT[] = "..";

/* Based on chdir.c */
static inline int _resolve_path(struct _reent *r, char *path, const char *extra, int maxLength) {
	char *pathEnd;
	int pathLength;
	const char *extraEnd;
	int extraSize;

	pathLength = strnlen (path, maxLength);

	/* assumes path ends in a directory separator */
	if (pathLength >= maxLength) {
		r->_errno = ENAMETOOLONG;
		return -1;
	}
	pathEnd = path + pathLength;
	if (pathEnd[-1] != DIRECTORY_SEPARATOR_CHAR) {
		pathEnd[0] = DIRECTORY_SEPARATOR_CHAR;
		pathEnd += 1;
	}

	extraEnd = extra;

	/* If the extra bit starts with a slash, start at root */
	if (extra[0] == DIRECTORY_SEPARATOR_CHAR) {
		pathEnd = strchr (path, DIRECTORY_SEPARATOR_CHAR) + 1;
		pathEnd[0] = '\0';
	}
	do {
		/* Advance past any separators in extra */
		while (extra[0] == DIRECTORY_SEPARATOR_CHAR) {
			extra += 1;
		}

		/* Grab the next directory name from extra */
		extraEnd = strchr (extra, DIRECTORY_SEPARATOR_CHAR);
		if (extraEnd == NULL) {
			extraEnd = strrchr (extra, '\0');
		} else {
			extraEnd += 1;
		}

		extraSize = (extraEnd - extra);
		if (extraSize == 0) {
			break;
		}

		if ((strncmp (extra, DIRECTORY_THIS, sizeof(DIRECTORY_THIS) - 1) == 0)
			&& ((extra[sizeof(DIRECTORY_THIS)-1] == DIRECTORY_SEPARATOR_CHAR)
				||(extra[sizeof(DIRECTORY_THIS)-1] == '\0')))
		{
			/* Don't copy anything */
		} else 	if ((strncmp (extra, DIRECTORY_PARENT, sizeof(DIRECTORY_PARENT) - 1) == 0)
			&& ((extra[sizeof(DIRECTORY_PARENT)-1] == DIRECTORY_SEPARATOR_CHAR)
				||(extra[sizeof(DIRECTORY_PARENT)-1] == '\0')))
		{
			/* Go up one level of in the path */
			if (pathEnd[-1] == DIRECTORY_SEPARATOR_CHAR) {
				// Remove trailing separator
				pathEnd[-1] = '\0';
			}
			pathEnd = strrchr (path, DIRECTORY_SEPARATOR_CHAR);
			if (pathEnd == NULL) {
				/* Can't go up any higher, return false */
				r->_errno = ENOENT;
				return -1;
			}
			pathLength = pathEnd - path;
			pathEnd += 1;
		} else {
			pathLength += extraSize;
			if (pathLength >= maxLength) {
				r->_errno = ENAMETOOLONG;
				return -1;
			}
			/* Copy the next part over */
			strncpy (pathEnd, extra, extraSize);
			pathEnd += extraSize;
		}
		pathEnd[0] = '\0';
		extra += extraSize;
	} while (extraSize != 0);

	return 0;
}

char *realpath(const char *__restrict path, char *__restrict resolved)
{
	struct _reent *r = _REENT;

	char stack[PATH_MAX] = {0};
	const char *pathPosition = NULL;
	int len = 0;

	if (!path) {
		errno = ENOENT;
		return NULL;
	}

	len = strnlen(path, PATH_MAX);
	if (!len) {
		r->_errno = ENOENT;
		return NULL;	
	}
	if (len >= PATH_MAX) {
		r->_errno = ENAMETOOLONG;
		return NULL;
	}

	if (strchr (path, ':') != NULL) {
		strncpy(stack, path, PATH_MAX-1);
		/* Move path past device name */
		path = strchr(path, ':') + 1;
	} else {
		getcwd(stack, PATH_MAX);
	}

	pathPosition = strchr(stack, ':');

	if (pathPosition == NULL) {
		pathPosition = stack;
	} else {
		pathPosition++;
	}

	/* Make sure the path starts in the root directory */
	if (pathPosition[0] != DIRECTORY_SEPARATOR_CHAR) {
		r->_errno = ENOENT;
		return NULL;
	}

	if (_resolve_path(r, stack, path, PATH_MAX) == -1) {
		return NULL;
	}

	if (resolved) {
		strncpy(resolved, stack, PATH_MAX);
		return resolved;
	}

	return strndup(stack, sizeof(stack));
}
