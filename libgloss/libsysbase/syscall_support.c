#include <sys/iosupport.h>
#include <errno.h>

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

int __libc_cond_init(_COND_T *cond) {

	*cond = __COND_INITIALIZER;

}

int __libc_cond_signal(_COND_T *cond) {

	if ( __has_syscall(cond_signal) ) {
		return __syscall_cond_signal(cond);
	}

	return ENOSYS;

}

int __libc_cond_broadcast(_COND_T *cond) {

	if ( __has_syscall(cond_broadcast) ) {
		return __syscall_cond_broadcast(cond);
	}

	return ENOSYS;

}

int __libc_cond_wait(_COND_T *cond, _LOCK_T *lock, uint64_t timeout_ns) {

	if ( __has_syscall(cond_wait) ) {
		return __syscall_cond_wait(cond, lock, timeout_ns);
	}

	return ENOSYS;

}

int __libc_cond_wait_recursive(_COND_T *cond, _LOCK_RECURSIVE_T *lock, uint64_t timeout_ns) {

	if ( __has_syscall(cond_wait_recursive) ) {
		return __syscall_cond_wait_recursive(cond, lock, timeout_ns);
	}

	return ENOSYS;

}


#ifdef CUSTOM_MALLOC_LOCK

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

#endif
