#include <sys/iosupport.h>

//---------------------------------------------------------------------------------
__syscalls_t __syscalls = {
//---------------------------------------------------------------------------------
	NULL,	// sbrk
	NULL,	// lock_init
	NULL,	// lock_close
	NULL,	// lock_release
	NULL,	// lock_acquire
	NULL,	// malloc_lock
	NULL,	// malloc_unlock
	NULL,	// exit
	NULL,	// gettod_r
	NULL,	// clock_gettime
	NULL,	// clock_settime
	NULL,	// clock_getres
	NULL	// nanosleep
};

int __libc_lock_init(int *lock,int recursive) {

	if ( __syscalls.lock_init ) {
		return __syscalls.lock_init(lock, recursive);
	}

	return 0;
}

int __libc_lock_close( int *lock ) {

	if ( __syscalls.lock_close) {
		return __syscalls.lock_close(lock);
	}

	return 0;
}

int __libc_lock_release( int *lock ) {

	if ( __syscalls.lock_release) {
		return __syscalls.lock_release(lock);
	}

	return 0;
}

int __libc_lock_acquire( int *lock ) {

	if ( __syscalls.lock_acquire) {
		return __syscalls.lock_acquire(lock);
	}

	return 0;
}

void __malloc_lock( struct _reent *ptr ) {
	if ( __syscalls.malloc_lock) {
		__syscalls.malloc_lock(ptr);
	}
}

void __malloc_unlock( struct _reent *ptr ) {
	if ( __syscalls.malloc_unlock) {
		__syscalls.malloc_unlock(ptr);
	}
}
