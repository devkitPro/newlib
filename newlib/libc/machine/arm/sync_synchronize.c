#if __ARM_ARCH < 6
#include <sys/cdefs.h>
void __attribute__((weak))
__sync_synchronize (void)
{
  __warn_references (__sync_synchronize,
		     "legacy compatible __sync_synchronize used. Not suitable for multi-threaded applications");
  return;
}
#endif
