#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/iosupport.h>

void
__assert_func (const char *file,
        int line,
        const char *func,
        const char *failedexpr)
{
  if ( __has_syscall(assert_func) ) {
    __syscall_assert_func(file, line, func, failedexpr);
  }
  abort();
  /* NOTREACHED */
}
