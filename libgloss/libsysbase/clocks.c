#include <errno.h>
#include <time.h>
#include <sys/iosupport.h>

int clock_gettime(clockid_t clock_id, struct timespec *tp)
{
   if ( __has_syscall(clock_gettime) ) {
      return __syscall_clock_gettime(clock_id, tp);
   } else {
      errno = ENOSYS;
      return -1;
   }
}

int clock_settime(clockid_t clock_id, const struct timespec *tp)
{
   if ( __has_syscall(clock_settime) ) {
      return __syscall_clock_settime(clock_id, tp);
   } else {
      errno = ENOSYS;
      return -1;
   }
}

int clock_getres (clockid_t clock_id, struct timespec *res)
{
   if ( __has_syscall(clock_getres) ) {
      return __syscall_clock_getres(clock_id, res);
   } else {
      errno = ENOSYS;
      return -1;
   }
}

