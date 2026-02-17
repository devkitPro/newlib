#include <config.h>
#include <_ansi.h>
#include <_syslist.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef REENTRANT_SYSCALLS_PROVIDED
#include <reent.h>
#endif
#include <errno.h>
#include <sys/lock.h>

void __flockfile(FILE *fp)
{
	__lock_acquire_recursive(*(_LOCK_RECURSIVE_T*)&fp->_lock);
}

void __funlockfile(FILE *fp)
{
	__lock_release_recursive(*(_LOCK_RECURSIVE_T*)&fp->_lock);
}

void flockfile(FILE *fp)
{
	if (!(fp->_flags & __SSTR)) {
		__lock_acquire_recursive(fp->_lock);
	}
}

void funlockfile(FILE *fp)
{
	if (!(fp->_flags & __SSTR)) {
		__lock_release_recursive(fp->_lock);
	}
}

int ftrylockfile(FILE *fp)
{
	if (!(fp->_flags & __SSTR)) {
		return __lock_try_acquire_recursive(fp->_lock);
	}
	return 0;
}