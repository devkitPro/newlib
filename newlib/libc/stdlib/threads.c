/*-
 * Copyright (c) 2019 fincs <fincs@devkitpro.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <threads.h>
#include <sched.h>

#if defined(PTHREAD_MUTEX_INITIALIZER) && defined(PTHREAD_RECURSIVE_MUTEX_INITIALIZER)
#define MTX_DIRECT_INIT
#endif

#if defined(PTHREAD_COND_INITIALIZER)
#define CND_DIRECT_INIT
#endif

static inline int err2thrd(int err)
{
    switch (err) {
        case 0:
            return thrd_success;
        case EBUSY:
            return thrd_busy;
        case ETIMEDOUT:
            return thrd_timedout;
        case ENOMEM:
            return thrd_nomem;
        default:
            return thrd_error;
    }
}

void call_once(once_flag *flag, void (*func)(void))
{
    pthread_once(flag, func);
}

int cnd_broadcast(cnd_t *cond)
{
    int err = pthread_cond_broadcast(cond);
    return err2thrd(err);
}

void cnd_destroy(cnd_t *cond)
{
#ifndef CND_DIRECT_INIT
    pthread_cond_destroy(cond);
#endif
}

int cnd_init(cnd_t *cond)
{
#ifdef CND_DIRECT_INIT
    *cond = PTHREAD_COND_INITIALIZER;
    return thrd_success;
#else
    int err = pthread_cond_init(cond, NULL);
    return err2thrd(err);
#endif
}

int cnd_signal(cnd_t *cond)
{
    int err = pthread_cond_signal(cond);
    return err2thrd(err);
}

int cnd_timedwait(cnd_t *__restrict cond, mtx_t *__restrict mtx, const struct timespec *__restrict abs_time)
{
    int err = pthread_cond_timedwait(cond, mtx, abs_time);
    return err2thrd(err);
}

int cnd_wait(cnd_t *cond, mtx_t *mtx)
{
    int err = pthread_cond_wait(cond, mtx);
    return err2thrd(err);
}

void mtx_destroy(mtx_t *mtx)
{
#ifndef MTX_DIRECT_INIT
    pthread_mutex_destroy(mtx);
#endif
}

int mtx_init(mtx_t *mtx, int type)
{
    if (type & mtx_timed)
        return thrd_error;
#ifdef MTX_DIRECT_INIT
    if (type & mtx_recursive)
        *mtx = PTHREAD_RECURSIVE_MUTEX_INITIALIZER;
    else
        *mtx = PTHREAD_MUTEX_INITIALIZER;
    return thrd_success;
#else
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    if (type & mtx_plain)
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_NORMAL);
    if (type & mtx_recursive)
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    int err = pthread_mutex_init(mtx, &attr);
    pthread_mutexattr_destroy(&attr);
    return err2thrd(err);
#endif
}

int mtx_lock(mtx_t *mtx)
{
    int err = pthread_mutex_lock(mtx);
    return err2thrd(err);
}

int mtx_timedlock(mtx_t *__restrict mtx, const struct timespec *__restrict ts)
{
    return thrd_error;
}

int mtx_trylock(mtx_t *mtx)
{
    int err = pthread_mutex_trylock(mtx);
    return err2thrd(err);
}

int mtx_unlock(mtx_t *mtx)
{
    int err = pthread_mutex_unlock(mtx);
    return err2thrd(err);
}

int thrd_create(thrd_t *thr, thrd_start_t func, void *arg)
{
    int err = pthread_create(thr, NULL, (void*(*)(void*))func, arg);
    return err2thrd(err);
}

thrd_t thrd_current(void)
{
    return pthread_self();
}

int thrd_detach(thrd_t thr)
{
    int err = pthread_detach(thr);
    return err2thrd(err);
}

int thrd_equal(thrd_t thr1, thrd_t thr2)
{
    return pthread_equal(thr1, thr2);
}

void thrd_exit(int res)
{
    pthread_exit((void*)(uintptr_t)res);
}

int thrd_join(thrd_t thr, int *res)
{
    void *value = NULL;
    int err = pthread_join(thr, &value);
    if (!err && res)
        *res = (uintptr_t)value;
    return err2thrd(err);
}

int thrd_sleep(const struct timespec *duration, struct timespec *remaining)
{
    return nanosleep(duration, remaining);
}

void thrd_yield(void)
{
    sched_yield();
}

int tss_create(tss_t *key, tss_dtor_t dtor)
{
    int err = pthread_key_create(key, dtor);
    return err2thrd(err);
}

void tss_delete(tss_t key)
{
    pthread_key_delete(key);
}

void * tss_get(tss_t key)
{
    return pthread_getspecific(key);
}

int tss_set(tss_t key, void *val)
{
    int err = pthread_setspecific(key, val);
    return err2thrd(err);
}
