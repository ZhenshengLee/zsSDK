// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// This library
#include <zsainternal/rwlock.h>

#include <zsainternal/common.h>

// System dependencies
#include <assert.h>
#include <pthread.h>

// TODO: static_assert be available with C11. See Issue #482
// static_assert(sizeof(zsa_rwlock_t) == sizeof(pthread_rwlock_t),
//              "Linux pthread_rwlock_t size doesn't match generic zsa_rwlock_t size");

// If one of the lock synchronization functions fails, retry forever
// rather than return an error code since callers are unlikeley to
// properly test the error condition and crashing should be avoided.

void rwlock_init(zsa_rwlock_t *lock)
{
    int err;
    while ((err = pthread_rwlock_init((pthread_rwlock_t *)lock, NULL)))
    {
        assert(err == 0);
    }
}
void rwlock_deinit(zsa_rwlock_t *lock)
{
    int err;
    while ((err = pthread_rwlock_destroy((pthread_rwlock_t *)lock)))
    {
        assert(err == 0);
    }
}

void rwlock_acquire_read(zsa_rwlock_t *lock)
{
    int err;
    while ((err = pthread_rwlock_rdlock((pthread_rwlock_t *)lock)))
    {
        assert(err == 0);
    }
}

bool rwlock_try_acquire_read(zsa_rwlock_t *lock)
{
    int err = pthread_rwlock_tryrdlock((pthread_rwlock_t *)lock);
    return err == 0;
}

void rwlock_acquire_write(zsa_rwlock_t *lock)
{
    int err;
    while ((err = pthread_rwlock_wrlock((pthread_rwlock_t *)lock)))
    {
        assert(err == 0);
    }
}

bool rwlock_try_acquire_write(zsa_rwlock_t *lock)
{
    int err = pthread_rwlock_trywrlock((pthread_rwlock_t *)lock);
    return err == 0;
}

void rwlock_release_read(zsa_rwlock_t *lock)
{
    pthread_rwlock_unlock((pthread_rwlock_t *)lock);
}

void rwlock_release_write(zsa_rwlock_t *lock)
{
    pthread_rwlock_unlock((pthread_rwlock_t *)lock);
}
