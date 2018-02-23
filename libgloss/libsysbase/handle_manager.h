#ifndef __HANDLE_MANAGER_H__
#define __HANDLE_MANAGER_H__

#include <sys/iosupport.h>

void __release_handle(int fd);
int  __alloc_handle(int size);
__handle *__get_handle(int fd);

#endif
