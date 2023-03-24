#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/iosupport.h>

int
pthread_atfork (void (*prepare)(void), void (*parent)(void), void (*child)(void))
{
	return ENOSYS; // Unsupported
}

//-----------------------------------------------------------------------------
// Mutex attributes
//-----------------------------------------------------------------------------

int
pthread_mutexattr_init (pthread_mutexattr_t *__attr)
{
	if (!__attr)
		return EINVAL;
	__attr->type = PTHREAD_MUTEX_NORMAL;
	return 0;
}

int
pthread_mutexattr_destroy (pthread_mutexattr_t *__attr)
{
	if (!__attr)
		return EINVAL;
	return 0;
}

int
pthread_mutexattr_getpshared (const pthread_mutexattr_t *__attr, int *__pshared)
{
	return ENOSYS;
}

int
pthread_mutexattr_setpshared (pthread_mutexattr_t *__attr, int __pshared)
{
	return ENOSYS;
}

int
pthread_mutexattr_gettype (const pthread_mutexattr_t *__attr, int *__kind)
{
	if (!__attr || !__kind)
		return EINVAL;
	*__kind = __attr->type;
	return 0;
}

int
pthread_mutexattr_settype (pthread_mutexattr_t *__attr, int __kind)
{
	if (!__attr)
		return EINVAL;
	__attr->type = __kind;
	return 0;
}

//-----------------------------------------------------------------------------
// Mutex
//-----------------------------------------------------------------------------

int
pthread_mutex_init (pthread_mutex_t *__mutex, const pthread_mutexattr_t *__attr)
{
	pthread_mutexattr_t __default;
	if (!__attr) {
		pthread_mutexattr_init(&__default);
		__attr = &__default;
	}
	if (!__mutex)
		return EINVAL;

	__mutex->type = __attr->type;
	switch (__mutex->type) {
		case PTHREAD_MUTEX_NORMAL:
		case PTHREAD_MUTEX_ERRORCHECK:
		case PTHREAD_MUTEX_DEFAULT:
			__lock_init(__mutex->normal);
			break;
		case PTHREAD_MUTEX_RECURSIVE:
			__lock_init_recursive(__mutex->recursive);
			break;
		default:
			return EINVAL;
	}

	return 0;
}

int
pthread_mutex_destroy (pthread_mutex_t *__mutex)
{
	if (!__mutex)
		return EINVAL;
	return 0;
}

int
pthread_mutex_lock (pthread_mutex_t *__mutex)
{
	if (!__mutex)
		return EINVAL;

	switch (__mutex->type) {
		case PTHREAD_MUTEX_NORMAL:
		case PTHREAD_MUTEX_ERRORCHECK: // todo: error check
		case PTHREAD_MUTEX_DEFAULT:
			__lock_acquire(__mutex->normal);
			break;
		case PTHREAD_MUTEX_RECURSIVE:
			__lock_acquire_recursive(__mutex->recursive);
			break;
		default:
			return EINVAL;
	}

	return 0;
}

int
pthread_mutex_trylock (pthread_mutex_t *__mutex)
{
	if (!__mutex)
		return EINVAL;

	int rc = 0;
	switch (__mutex->type) {
		case PTHREAD_MUTEX_NORMAL:
		case PTHREAD_MUTEX_ERRORCHECK: // todo: error check
		case PTHREAD_MUTEX_DEFAULT:
			rc = __lock_try_acquire(__mutex->normal);
			break;
		case PTHREAD_MUTEX_RECURSIVE:
			rc = __lock_try_acquire_recursive(__mutex->recursive);
			break;
		default:
			return EINVAL;
	}

	if (rc)
		return EBUSY;
	return 0;
}

int
pthread_mutex_unlock (pthread_mutex_t *__mutex)
{
	if (!__mutex)
		return EINVAL;

	switch (__mutex->type) {
		case PTHREAD_MUTEX_NORMAL:
		case PTHREAD_MUTEX_ERRORCHECK: // todo: error check
		case PTHREAD_MUTEX_DEFAULT:
			__lock_release(__mutex->normal);
			break;
		case PTHREAD_MUTEX_RECURSIVE:
			__lock_release_recursive(__mutex->recursive);
			break;
		default:
			return EINVAL;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// Condition variable attributes
//-----------------------------------------------------------------------------

int
pthread_condattr_init (pthread_condattr_t *__attr)
{
	if (!__attr)
		return EINVAL;
	__attr->clock_id = CLOCK_REALTIME;
	return 0;
}

int
pthread_condattr_destroy (pthread_condattr_t *__attr)
{
	if (!__attr)
		return EINVAL;
	return 0;
}

int
pthread_condattr_getclock (const pthread_condattr_t *__restrict __attr, clockid_t *__restrict __clock_id)
{
	if (!__attr || !__clock_id)
		return EINVAL;
	*__clock_id = __attr->clock_id;
	return 0;
}

int
pthread_condattr_setclock (pthread_condattr_t *__attr, clockid_t __clock_id)
{
	if (!__attr)
		return EINVAL;
	__attr->clock_id = __clock_id;
	return 0;
}

int
pthread_condattr_getpshared (const pthread_condattr_t *__attr, int *__pshared)
{
	return ENOSYS;
}

int
pthread_condattr_setpshared (pthread_condattr_t *__attr, int __pshared)
{
	return ENOSYS;
}

//-----------------------------------------------------------------------------
// Condition variable
//-----------------------------------------------------------------------------

int
pthread_cond_init (pthread_cond_t *__cond, const pthread_condattr_t *__attr)
{
	pthread_condattr_t __default;
	if (!__attr) {
		pthread_condattr_init(&__default);
		__attr = &__default;
	}
	if (!__cond)
		return EINVAL;

	__cond->clock_id = __attr->clock_id;
	return __cond_init(__cond->cond);
}

int
pthread_cond_destroy (pthread_cond_t *__cond)
{
	if (!__cond)
		return EINVAL;
	return 0;
}

int
pthread_cond_signal (pthread_cond_t *__cond)
{
	if (!__cond)
		return EINVAL;
	return __cond_signal(__cond->cond);
}

int
pthread_cond_broadcast (pthread_cond_t *__cond)
{
	if (!__cond)
		return EINVAL;
	return __cond_broadcast(__cond->cond);
}

static int
__pthread_cond_wait_common (pthread_cond_t *__cond, pthread_mutex_t *__mutex, uint64_t timeout_ns)
{
	switch (__mutex->type) {
		case PTHREAD_MUTEX_NORMAL:
		case PTHREAD_MUTEX_ERRORCHECK: // todo: error check
		case PTHREAD_MUTEX_DEFAULT:
			return __cond_wait(__cond->cond, __mutex->normal, timeout_ns);
		case PTHREAD_MUTEX_RECURSIVE:
			return __cond_wait_recursive(__cond->cond, __mutex->recursive, timeout_ns);
		default:
			return EINVAL;
	}
}

int
pthread_cond_wait (pthread_cond_t *__cond, pthread_mutex_t *__mutex)
{
	if (!__cond || !__mutex)
		return EINVAL;
	return __pthread_cond_wait_common(__cond, __mutex, UINT64_MAX);
}

int
pthread_cond_timedwait (pthread_cond_t *__cond, pthread_mutex_t *__mutex, const struct timespec *__abstime)
{
	if (!__cond || !__mutex || !__abstime)
		return EINVAL;
	return __pthread_cond_wait_common(__cond, __mutex, abstimespec2nsec(__cond->clock_id, __abstime));
}

//-----------------------------------------------------------------------------
// Thread attributes
//-----------------------------------------------------------------------------

int
pthread_attr_setschedparam (pthread_attr_t *__attr, const struct sched_param *__param)
{
	if (!__attr || !__param)
		return EINVAL;
	__attr->schedparam = *__param;
	return 0;
}

int
pthread_attr_getschedparam (const pthread_attr_t *__attr, struct sched_param *__param)
{
	if (!__attr || !__param)
		return EINVAL;
	*__param = __attr->schedparam;
	return 0;
}

int
pthread_attr_init (pthread_attr_t *__attr)
{
	if (!__attr)
		return EINVAL;
	memset(__attr, 0, sizeof(*__attr));
	return 0;
}

int
pthread_attr_destroy (pthread_attr_t *__attr)
{
	if (!__attr)
		return EINVAL;
	return 0;
}

int
pthread_attr_setstack (pthread_attr_t *attr, void *__stackaddr, size_t __stacksize)
{
	if (!attr || !__stackaddr || !__stacksize)
		return EINVAL;
	attr->stackaddr = __stackaddr;
	attr->stacksize = __stacksize;
	return 0;
}

int
pthread_attr_getstack (const pthread_attr_t *attr, void **__stackaddr, size_t *__stacksize)
{
	if (!attr || !__stackaddr || !__stacksize)
		return EINVAL;
	*__stackaddr = attr->stackaddr;
	*__stacksize = attr->stacksize;
	return 0;
}

int
pthread_attr_getstacksize (const pthread_attr_t *__attr, size_t *__stacksize)
{
	if (!__attr || !__stacksize)
		return EINVAL;
	*__stacksize = __attr->stacksize;
	return 0;
}

int
pthread_attr_setstacksize (pthread_attr_t *__attr, size_t __stacksize)
{
	if (!__attr || !__stacksize)
		return EINVAL;
	__attr->stacksize = __stacksize;
	return 0;
}

int
pthread_attr_getstackaddr (const pthread_attr_t *__attr, void **__stackaddr)
{
	if (!__attr || !__stackaddr)
		return EINVAL;
	*__stackaddr = __attr->stackaddr;
	return 0;
}

int
pthread_attr_setstackaddr (pthread_attr_t  *__attr, void *__stackaddr)
{
	if (!__attr || !__stackaddr)
		return EINVAL;
	__attr->stackaddr = __stackaddr;
	return 0;
}

int
pthread_attr_getdetachstate (const pthread_attr_t *__attr, int *__detachstate)
{
	if (!__attr || !__detachstate)
		return EINVAL;
	*__detachstate = __attr->detachstate;
	return 0;
}

int
pthread_attr_setdetachstate (pthread_attr_t *__attr, int __detachstate)
{
	if (!__attr)
		return EINVAL;
	__attr->detachstate = __detachstate;
	return 0;
}

int
pthread_attr_getguardsize (const pthread_attr_t *__attr, size_t *__guardsize)
{
	return ENOSYS; // Unsupported
}

int
pthread_attr_setguardsize (pthread_attr_t *__attr, size_t __guardsize)
{
	return ENOSYS; // Unsupported
}

//-----------------------------------------------------------------------------
// Thread
//-----------------------------------------------------------------------------

int
pthread_create (pthread_t *__pthread, const pthread_attr_t *__attr, void *(*__start_routine)(void *), void *__arg)
{
	pthread_attr_t __default;
	if (!__attr) {
		pthread_attr_init(&__default);
		__attr = &__default;
	}
	if (!__pthread || !__start_routine)
		return EINVAL;
	if (!__has_syscall(thread_create))
		return ENOSYS;
	return __syscall_thread_create(__pthread, __start_routine, __arg, __attr->stackaddr, __attr->stacksize);
}

int
pthread_join (pthread_t __pthread, void **__value_ptr)
{
	if (!__has_syscall(thread_join))
		return ENOSYS;
	void* value = __syscall_thread_join(__pthread);
	if (__value_ptr)
		*__value_ptr = value;
	return 0;
}

int
pthread_detach (pthread_t __pthread)
{
	if (!__has_syscall(thread_detach))
		return ENOSYS;
	return __syscall_thread_detach(__pthread);
}

void
pthread_exit (void *__value_ptr)
{
	if (__has_syscall(thread_exit))
		__syscall_thread_exit(__value_ptr);
	for (;;);
}

pthread_t
pthread_self (void)
{
	if (__has_syscall(thread_self))
		return __syscall_thread_self();
	return NULL;
}

int
pthread_equal (pthread_t __t1, pthread_t __t2)
{
	return __t1 == __t2;
}

int
pthread_getcpuclockid (pthread_t thread, clockid_t *clock_id)
{
	if (!thread || !clock_id)
		return EINVAL;
	*clock_id = CLOCK_REALTIME;
	return 0;
}

int
pthread_setconcurrency (int new_level)
{
	return ENOSYS; // Unsupported
}

int
pthread_getconcurrency (void)
{
	return ENOSYS; // Unsupported
}

//-----------------------------------------------------------------------------
// Run-once
//-----------------------------------------------------------------------------

__LOCK_INIT(static, __pthread_once_lock)
__COND_INIT(static, __pthread_once_cv)

int
pthread_once (pthread_once_t *__once_control, void (*__init_routine)(void))
{
	if (!__once_control || !__init_routine)
		return EINVAL;

	__lock_acquire(__pthread_once_lock);

	if (__once_control->status == 0) {
		__once_control->status = 1;
		__lock_release(__pthread_once_lock);
		__init_routine();
		__lock_acquire(__pthread_once_lock);
		__once_control->status = 2;
		__cond_broadcast(__pthread_once_cv);
	} else {
		while (__once_control->status == 1)
			__cond_wait(__pthread_once_cv, __pthread_once_lock, UINT64_MAX);
	}

	__lock_release(__pthread_once_lock);
	return 0;
}

//-----------------------------------------------------------------------------
// Thread-specific keys (TLS)
//-----------------------------------------------------------------------------

int
pthread_key_create (pthread_key_t *__key, void (*__destructor)(void *))
{
	if (!__has_syscall(tls_create))
		return ENOSYS;
	return __syscall_tls_create(__key, __destructor);
}

int
pthread_setspecific (pthread_key_t __key, const void *__value)
{
	if (!__has_syscall(tls_set))
		return ENOSYS;
	return __syscall_tls_set(__key, __value);
}

void *
pthread_getspecific (pthread_key_t __key)
{
	if (__has_syscall(tls_get))
		return __syscall_tls_get(__key);
	return NULL;
}

int
pthread_key_delete (pthread_key_t __key)
{
	if (!__has_syscall(tls_delete))
		return ENOSYS;
	return __syscall_tls_delete(__key);
}

//-----------------------------------------------------------------------------
// Cancel
//-----------------------------------------------------------------------------

int
pthread_cancel (pthread_t __pthread)
{
	return ENOSYS; // Unsupported
}

int
pthread_setcancelstate (int __state, int *__oldstate)
{
	return ENOSYS; // Unsupported
}

int
pthread_setcanceltype (int __type, int *__oldtype)
{
	return ENOSYS; // Unsupported
}

void
pthread_testcancel (void)
{
	// Unsupported
}

//-----------------------------------------------------------------------------
// Cleanup
//-----------------------------------------------------------------------------

void
_pthread_cleanup_push (struct _pthread_cleanup_context *_context, void (*_routine)(void *), void *_arg)
{
	// Unsupported
}

void
_pthread_cleanup_pop (struct _pthread_cleanup_context *_context, int _execute)
{
	// Unsupported
}
