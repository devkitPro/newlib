#if __ARM_ARCH < 6
#include <sys/cdefs.h>
void __attribute__((weak))
__sync_synchronize (void)
{
  return;
}
#endif
