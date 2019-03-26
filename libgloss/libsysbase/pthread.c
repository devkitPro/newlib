#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/iosupport.h>

static inline uint64_t
__pthread_abstime_to_ns (const struct timespec *__abstime)
{
	return __abstime ? abstimespec2nsec(CLOCK_REALTIME, __abstime) : UINT64_MAX;
}

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
			__mutex->normal = __LOCK_INITIALIZER;
			break;
		case PTHREAD_MUTEX_RECURSIVE:
			__mutex->recursive = __LOCK_INITIALIZER_RECURSIVE;
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
	__cond->cond = __COND_INITIALIZER;
	return 0;
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
	if (!__key)
		return EINVAL;
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

static pthread_once_t __pthread_cleanup_once;
static pthread_key_t __pthread_cleanup_key;

static void
__pthread_cleanup_dtor (void *arg)
{
	struct _pthread_cleanup_context *cur;
	for (cur = (struct _pthread_cleanup_context *)arg; cur; cur = cur->_previous)
		cur->_routine(cur->_arg);
}

static void
__pthread_cleanup_setup (void)
{
	int err = pthread_key_create(&__pthread_cleanup_key, __pthread_cleanup_dtor);
	if (err)
		abort();
}

void
_pthread_cleanup_push (struct _pthread_cleanup_context *_context, void (*_routine)(void *), void *_arg)
{
	pthread_once(&__pthread_cleanup_once, __pthread_cleanup_setup);
	_context->_routine = _routine;
	_context->_arg = _arg;
	_context->_previous = (struct _pthread_cleanup_context *)pthread_getspecific(__pthread_cleanup_key);
	pthread_setspecific(__pthread_cleanup_key, _context);
}

void
_pthread_cleanup_pop (struct _pthread_cleanup_context *_context, int _execute)
{
	struct _pthread_cleanup_context *cur = (struct _pthread_cleanup_context *)pthread_getspecific(__pthread_cleanup_key);
	if (cur) {
		if (_execute)
			cur->_routine(cur->_arg);
		pthread_setspecific(__pthread_cleanup_key, cur->_previous);
	}
}

//-----------------------------------------------------------------------------
// Barrier
//-----------------------------------------------------------------------------

int
pthread_barrierattr_init (pthread_barrierattr_t *__attr)
{
	if (!__attr)
		return EINVAL;
	return 0;
}

int
pthread_barrierattr_destroy (pthread_barrierattr_t *__attr)
{
	if (!__attr)
		return EINVAL;
	return 0;
}

int
pthread_barrierattr_getpshared (const pthread_barrierattr_t *__attr, int *__pshared)
{
	return ENOSYS;
}

int
pthread_barrierattr_setpshared (pthread_barrierattr_t *__attr, int __pshared)
{
	return ENOSYS;
}

int
pthread_barrier_init (pthread_barrier_t *__barrier, const pthread_barrierattr_t *__attr, unsigned __count)
{
	if (!__barrier || !__count)
		return EINVAL;

	__barrier->lock = __LOCK_INITIALIZER;
	__barrier->cond = __COND_INITIALIZER;
	__barrier->reload = __count;
	__barrier->counter = __count;
	__barrier->cycle = 0;
	return 0;
}

int
pthread_barrier_destroy (pthread_barrier_t *__barrier)
{
	if (!__barrier)
		return EINVAL;
	return 0;
}

int
pthread_barrier_wait (pthread_barrier_t *__barrier)
{
	if (!__barrier)
		return EINVAL;

	__lock_acquire(__barrier->lock);

	int is_last_thread = !--__barrier->counter;
	if (is_last_thread) {
		__barrier->cycle ++;
		__barrier->counter = __barrier->reload;
		__cond_broadcast(__barrier->cond);
	} else {
		unsigned my_cycle = __barrier->cycle;
		do
			__cond_wait(__barrier->cond, __barrier->lock, UINT64_MAX);
		while (__barrier->cycle == my_cycle);
	}

	__lock_release(__barrier->lock);
	return is_last_thread ? PTHREAD_BARRIER_SERIAL_THREAD : 0;
}

//-----------------------------------------------------------------------------
// Read/write lock
//-----------------------------------------------------------------------------

int
pthread_rwlockattr_init (pthread_rwlockattr_t *__attr)
{
	if (!__attr)
		return EINVAL;
	return 0;
}

int
pthread_rwlockattr_destroy (pthread_rwlockattr_t *__attr)
{
	if (!__attr)
		return EINVAL;
	return 0;
}

int
pthread_rwlockattr_getpshared (const pthread_rwlockattr_t *__attr, int *__pshared)
{
	return ENOSYS;
}

int
pthread_rwlockattr_setpshared (pthread_rwlockattr_t *__attr, int __pshared)
{
	return ENOSYS;
}

int
pthread_rwlock_init (pthread_rwlock_t *__rwlock, const pthread_rwlockattr_t *__attr)
{
	if (!__rwlock)
		return EINVAL;
	*__rwlock = PTHREAD_RWLOCK_INITIALIZER;
	return 0;
}

int
pthread_rwlock_destroy (pthread_rwlock_t *__rwlock)
{
	if (!__rwlock)
		return EINVAL;
	return 0;
}

static int
__pthread_rwlock_rdlock_common (pthread_rwlock_t *__rwlock, const struct timespec *__abstime)
{
	int rc = 0;
	__lock_acquire(__rwlock->lock);

	while (__rwlock->cnt_w) {
		rc = __cond_wait(__rwlock->cond_w, __rwlock->lock, __pthread_abstime_to_ns(__abstime));
		if (rc) break;
	}

	if (!rc)
		__rwlock->cnt_r ++;

	__lock_release(__rwlock->lock);
	return rc;
}

int
pthread_rwlock_rdlock (pthread_rwlock_t *__rwlock)
{
	if (!__rwlock)
		return EINVAL;
	return __pthread_rwlock_rdlock_common(__rwlock, NULL);
}

int
pthread_rwlock_tryrdlock (pthread_rwlock_t *__rwlock)
{
	if (!__rwlock)
		return EINVAL;

	__lock_acquire(__rwlock->lock);

	int failed = __rwlock->cnt_w != 0;
	if (!failed)
		__rwlock->cnt_r ++;

	__lock_release(__rwlock->lock);
	return failed ? EBUSY : 0;
}

int
pthread_rwlock_timedrdlock (pthread_rwlock_t *__rwlock, const struct timespec *__abstime)
{
	if (!__rwlock || !__abstime)
		return EINVAL;
	return __pthread_rwlock_rdlock_common(__rwlock, __abstime);
}

int
pthread_rwlock_unlock (pthread_rwlock_t *__rwlock)
{
	if (!__rwlock)
		return EINVAL;

	__lock_acquire(__rwlock->lock);

	if (__rwlock->cnt_w != 2) {
		if (!--__rwlock->cnt_r)
			__cond_broadcast(__rwlock->cond_r);
	} else {
		__rwlock->cnt_w = 0;
		__cond_broadcast(__rwlock->cond_w);
	}

	__lock_release(__rwlock->lock);
	return 0;
}

static int
__pthread_rwlock_wrlock_common (pthread_rwlock_t *__rwlock, const struct timespec *__abstime)
{
	int rc = 0;
	__lock_acquire(__rwlock->lock);

	while (__rwlock->cnt_w) {
		rc = __cond_wait(__rwlock->cond_w, __rwlock->lock, __pthread_abstime_to_ns(__abstime));
		if (rc) break;
	}

	if (!rc) {
		__rwlock->cnt_w = 1;

		while (__rwlock->cnt_r) {
			rc = __cond_wait(__rwlock->cond_r, __rwlock->lock, __pthread_abstime_to_ns(__abstime));
			if (rc) break;
		}

		if (rc) {
			__rwlock->cnt_w = 0;
			__cond_broadcast(__rwlock->cond_w);
		} else
			__rwlock->cnt_w = 2;
	}

	__lock_release(__rwlock->lock);
	return rc;
}

int
pthread_rwlock_wrlock (pthread_rwlock_t *__rwlock)
{
	if (!__rwlock)
		return EINVAL;
	return __pthread_rwlock_wrlock_common(__rwlock, NULL);
}

int
pthread_rwlock_trywrlock (pthread_rwlock_t *__rwlock)
{
	if (!__rwlock)
		return EINVAL;

	__lock_acquire(__rwlock->lock);

	int failed = __rwlock->cnt_w != 0 || __rwlock->cnt_r != 0;
	if (!failed)
		__rwlock->cnt_w = 2;

	__lock_release(__rwlock->lock);
	return failed ? EBUSY : 0;
}

int
pthread_rwlock_timedwrlock (pthread_rwlock_t *__rwlock, const struct timespec *__abstime)
{
	if (!__rwlock || !__abstime)
		return EINVAL;
	return __pthread_rwlock_wrlock_common(__rwlock, __abstime);
}

//-----------------------------------------------------------------------------
// semaphore.h
//-----------------------------------------------------------------------------

int
sem_close(sem_t *__sem)
{
	errno = ENOSYS;
	return -1;
}

int
sem_destroy(sem_t *__sem)
{
	if (!__sem) {
		errno = EINVAL;
		return -1;
	}
	return 0;
}

int
sem_getvalue(sem_t *__sem, int *__sval)
{
	if (!__sem || !__sval) {
		errno = EINVAL;
		return -1;
	}

	__lock_acquire(__sem->lock);
	*__sval = __sem->value;
	__lock_release(__sem->lock);
	return 0;
}

int
sem_init(sem_t *__sem, int __pshared, unsigned int __value)
{
	if (!__sem || __value > SEM_VALUE_MAX) {
		errno = EINVAL;
		return -1;
	}
	if (__pshared) {
		errno = ENOSYS;
		return -1;
	}

	__sem->lock = __LOCK_INITIALIZER;
	__sem->cond = __COND_INITIALIZER;
	__sem->value = __value;
	return 0;
}

sem_t *
sem_open(const char *__name, int __oflag, ...)
{
	errno = ENOSYS;
	return SEM_FAILED;
}

int
sem_post(sem_t *__sem)
{
	if (!__sem) {
		errno = EINVAL;
		return -1;
	}

	__lock_acquire(__sem->lock);
	if (__sem->value++ < 0)
		__cond_signal(__sem->cond);
	__lock_release(__sem->lock);
	return 0;
}

static int
__sem_wait_common(sem_t *__sem, const struct timespec *__abstime)
{
	int rc = 0;
	__lock_acquire(__sem->lock);

	__sem->value --;
	if (__sem->value < 0)
		rc = __cond_wait(__sem->cond, __sem->lock, __pthread_abstime_to_ns(__abstime));
	if (rc) {
		if (__sem->value < 0)
			__sem->value ++;
		else
			rc = 0;
	}

	__lock_release(__sem->lock);

	if (rc) {
		errno = rc;
		return -1;
	}

	return 0;
}

int
sem_timedwait(sem_t *__sem, const struct timespec *__abstime)
{
	if (!__sem || !__abstime) {
		errno = EINVAL;
		return -1;
	}

	return __sem_wait_common(__sem, __abstime);
}

int
sem_trywait(sem_t *__sem)
{
	if (!__sem) {
		errno = EINVAL;
		return -1;
	}

	__lock_acquire(__sem->lock);

	int failed = __sem->value <= 0;
	if (!failed)
		__sem->value --;

	__lock_release(__sem->lock);

	if (failed) {
		errno = EAGAIN;
		return -1;
	}

	return 0;
}

int
sem_unlink(const char *__name)
{
	errno = ENOSYS;
	return -1;
}

int
sem_wait(sem_t *__sem)
{
	if (!__sem) {
		errno = EINVAL;
		return -1;
	}

	return __sem_wait_common(__sem, NULL);
}

//-----------------------------------------------------------------------------
// sched.h
//-----------------------------------------------------------------------------

int __attribute__((weak))
sched_yield (void)
{
	errno = ENOSYS;
	return -1;
}

int __attribute__((weak))
sched_getcpu (void)
{
	errno = ENOSYS;
	return -1;
}
