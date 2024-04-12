#include <stdlib.h>
#include <unistd.h>
#include <sys/iosupport.h>

void abort(void) {
  do {

    if ( __has_syscall(abort) ) {
      __syscall_abort();
    } else {
     _exit (1);
    }
  } while(1);
}

