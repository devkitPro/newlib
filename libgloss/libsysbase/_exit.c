/* Stub version of _exit.  */

#include <limits.h>
#include "config.h"
#include <_ansi.h>
#include <_syslist.h>
#include <sys/iosupport.h>

void _exit(int rc)
{

	if ( __has_syscall(exit) ) {
		__syscall_exit(rc);
	}

	while(1);
}
