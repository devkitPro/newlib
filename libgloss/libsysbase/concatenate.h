#include <reent.h>
#include <string.h>
#include <errno.h>

#define DIRECTORY_SEPARATOR_CHAR '/'
#define DIRECTORY_SEPARATOR      "/"
#define DIRECTORY_THIS           "."
#define DIRECTORY_PARENT         ".."

int _concatenate_path (struct _reent *r, char *path, const char *extra, int maxLength);
