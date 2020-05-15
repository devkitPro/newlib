#include <sys/iosupport.h>

int __libc_lock_init(int *lock,int recursive) {

	if ( __has_syscall(lock_init) ) {
		return __syscall_lock_init(lock, recursive);
	}

	return 0;
}

int __libc_lock_close( int *lock ) {

	if ( __has_syscall(lock_close)) {
		return __syscall_lock_close(lock);
	}

	return 0;
}

int __libc_lock_release( int *lock ) {

	if ( __has_syscall(lock_release)) {
		return __syscall_lock_release(lock);
	}

	return 0;
}

int __libc_lock_acquire( int *lock ) {

	if ( __has_syscall(lock_acquire)) {
		return __syscall_lock_acquire(lock);
	}

	return 0;
}

void __malloc_lock( struct _reent *ptr ) {
	if ( __has_syscall(malloc_lock) ){
		__syscall_malloc_lock(ptr);
	}
}

void __malloc_unlock( struct _reent *ptr ) {
	if ( __has_syscall(malloc_unlock)) {
		__syscall_malloc_unlock(ptr);
	}
}
