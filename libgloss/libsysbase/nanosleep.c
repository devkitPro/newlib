#include <errno.h>
#include <time.h>
#include <sys/iosupport.h>

int nanosleep(const struct timespec *req, struct timespec *rem)
{
   if ( __has_syscall(nanosleep) ) {
      return __syscall_nanosleep(req, rem);
   } else {
      *rem = *req;
      errno = ENOSYS;
      return -1;
   }
}
