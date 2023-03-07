#include <sys/iosupport.h>

void __libc_lock_init(_LOCK_T *lock) {

	if ( __has_syscall(lock_init) ) {
		__syscall_lock_init(lock);
	}
}

void __libc_lock_acquire(_LOCK_T *lock ) {

	if ( __has_syscall(lock_acquire) ) {
		__syscall_lock_acquire(lock);
	}
}

int __libc_lock_try_acquire(_LOCK_T *lock ) {

	if ( __has_syscall(lock_acquire) ) {
		return __syscall_lock_try_acquire(lock);
	} else {
		return 0;
	}
}

void __libc_lock_release(_LOCK_T *lock ) {

	if ( __has_syscall(lock_release) ) {
		__syscall_lock_release(lock);
	}
}

void __libc_lock_close(_LOCK_T *lock ) {

	if ( __has_syscall(lock_close) ) {
		__syscall_lock_close(lock);
	}
}

void __libc_lock_init_recursive(_LOCK_RECURSIVE_T *lock) {

	if ( __has_syscall(lock_init_recursive) ) {
		__syscall_lock_init_recursive(lock);
	}

}

void __libc_lock_acquire_recursive(_LOCK_RECURSIVE_T *lock ) {

	if ( __has_syscall(lock_acquire_recursive) ) {
		__syscall_lock_acquire_recursive(lock);
	}
}

int __libc_lock_try_acquire_recursive(_LOCK_RECURSIVE_T *lock ) {

	if ( __has_syscall(lock_acquire_recursive) ) {
		return __syscall_lock_try_acquire_recursive(lock);
	} else {
		return 0;
	}
}

void __libc_lock_release_recursive(_LOCK_RECURSIVE_T *lock ) {

	if ( __has_syscall(lock_release_recursive) ) {
		__syscall_lock_release_recursive(lock);
	}
}

void __libc_lock_close_recursive(_LOCK_RECURSIVE_T *lock ) {

	if ( __has_syscall(lock_close_recursive) ) {
		__syscall_lock_close_recursive(lock);
	}
}
