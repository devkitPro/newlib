//---------------------------------------------------------------------------------
#ifndef __iosupp_h__
#define __iosupp_h__
//---------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

#include <reent.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/time.h>
#include <stdbool.h>

enum	{
	STD_IN,
	STD_OUT,
	STD_ERR,
	STD_MAX = 35
};


typedef struct {
	unsigned int device;
	unsigned int refcount;
	void *fileStruct;
} __handle;

/* Directory iterator for mantaining state between dir* calls */
typedef struct {
    int device;
    void *dirStruct;
} DIR_ITER;

typedef struct {
	const char *name;
	size_t structSize;
	int (*open_r)(struct _reent *r, void *fileStruct, const char *path, int flags, int mode);
	int (*close_r)(struct _reent *r, void *fd);
	ssize_t (*write_r)(struct _reent *r, void *fd, const char *ptr, size_t len);
	ssize_t (*read_r)(struct _reent *r, void *fd, char *ptr, size_t len);
	off_t (*seek_r)(struct _reent *r, void *fd, off_t pos, int dir);
	int (*fstat_r)(struct _reent *r, void *fd, struct stat *st);
	int (*stat_r)(struct _reent *r, const char *file, struct stat *st);
	int (*link_r)(struct _reent *r, const char *existing, const char  *newLink);
	int (*unlink_r)(struct _reent *r, const char *name);
	int (*chdir_r)(struct _reent *r, const char *name);
	int (*rename_r) (struct _reent *r, const char *oldName, const char *newName);
	int (*mkdir_r) (struct _reent *r, const char *path, int mode);

	size_t dirStateSize;

	DIR_ITER* (*diropen_r)(struct _reent *r, DIR_ITER *dirState, const char *path);
	int (*dirreset_r)(struct _reent *r, DIR_ITER *dirState);
	int (*dirnext_r)(struct _reent *r, DIR_ITER *dirState, char *filename, struct stat *filestat);
	int (*dirclose_r)(struct _reent *r, DIR_ITER *dirState);
	int (*statvfs_r)(struct _reent *r, const char *path, struct statvfs *buf);
	int (*ftruncate_r)(struct _reent *r, void *fd, off_t len);
	int (*fsync_r)(struct _reent *r, void *fd);

	void *deviceData;

	int (*chmod_r)(struct _reent *r, const char *path, mode_t mode);
	int (*fchmod_r)(struct _reent *r, void *fd, mode_t mode);
	int (*rmdir_r)(struct _reent *r, const char *name);
	int (*lstat_r)(struct _reent *r, const char *file, struct stat *st);
	int (*utimes_r)(struct _reent *r, const char *filename, const struct timeval times[2]);

	long (*fpathconf_r)(struct _reent *r, void *fd, int name);
	long (*pathconf_r)(struct _reent *r, const char *path, int name);

	int (*symlink_r)(struct _reent *r, const char *target, const char *linkpath);
	ssize_t (*readlink_r)(struct _reent *r, const char *path, char *buf, size_t bufsiz);

} devoptab_t;

extern const devoptab_t *devoptab_list[];

#ifdef _BUILDING_LIBSYSBASE
#define __SYSCALL(_name) __attribute__((weak)) __syscall_##_name
#define __has_syscall(_name) (&__syscall_##_name)
#else
#define __SYSCALL(_name) __syscall_##_name
#endif

void __SYSCALL(exit) ( int rc );
int  __SYSCALL(gettod_r) (struct _reent *ptr, struct timeval *tp, struct timezone *tz);
void __SYSCALL(lock_init) (_LOCK_T *lock);
void __SYSCALL(lock_acquire) (_LOCK_T *lock);
int  __SYSCALL(lock_try_acquire) (_LOCK_T *lock);
void __SYSCALL(lock_release) (_LOCK_T *lock);
void __SYSCALL(lock_close) (_LOCK_T *lock);
void __SYSCALL(lock_init_recursive) (_LOCK_RECURSIVE_T *lock);
void __SYSCALL(lock_acquire_recursive) (_LOCK_RECURSIVE_T *lock);
int  __SYSCALL(lock_try_acquire_recursive) (_LOCK_RECURSIVE_T *lock);
void __SYSCALL(lock_release_recursive) (_LOCK_RECURSIVE_T *lock);
void __SYSCALL(lock_close_recursive) (_LOCK_RECURSIVE_T *lock);

int  __SYSCALL(cond_signal)(_COND_T *cond);
int  __SYSCALL(cond_broadcast)(_COND_T *cond);
int  __SYSCALL(cond_wait)(_COND_T *cond, _LOCK_T *lock, uint64_t timeout_ns);
int  __SYSCALL(cond_wait_recursive)(_COND_T *cond, _LOCK_RECURSIVE_T *lock, uint64_t timeout_ns);
int  __SYSCALL(thread_create)(struct __pthread_t **thread, void* (*func)(void*), void *arg, void *stack_addr, size_t stack_size);
void*__SYSCALL(thread_join)(struct __pthread_t *thread);
int  __SYSCALL(thread_detach)(struct __pthread_t *thread);
void __SYSCALL(thread_exit)(void *value);
struct __pthread_t *__SYSCALL(thread_self)(void);
int  __SYSCALL(tls_create)(uint32_t *key, void (*destructor)(void*));
int  __SYSCALL(tls_set)(uint32_t key, const void *value);
void*__SYSCALL(tls_get)(uint32_t key);
int  __SYSCALL(tls_delete)(uint32_t key);

struct _reent * __SYSCALL(getreent) ();
int __SYSCALL(clock_gettime) (clockid_t clock_id, struct timespec *tp);
int __SYSCALL(clock_settime) (clockid_t clock_id, const struct timespec *tp);
int __SYSCALL(clock_getres) (clockid_t clock_id, struct timespec *res);
int __SYSCALL(nanosleep) (const struct timespec *req, struct timespec *rem);

void __SYSCALL(malloc_lock) (struct _reent *ptr);
void __SYSCALL(malloc_unlock) (struct _reent *ptr);

int AddDevice( const devoptab_t* device);
int FindDevice(const char* name);
int RemoveDevice(const char* name);
void setDefaultDevice( int device );

const devoptab_t* GetDeviceOpTab (const char *name);

void __release_handle(int fd);
int  __alloc_handle(int device);
__handle *__get_handle(int fd);

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------------
#endif // __iosupp_h__
//---------------------------------------------------------------------------------
