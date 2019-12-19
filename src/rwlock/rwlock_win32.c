// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// This library
#include <zsainternal/rwlock.h>

// System dependencies
#include <windows.h>
#include <assert.h>

// TODO: static_assert be available with C11. See Issue #482
// static_assert(sizeof(zsa_rwlock_t) == sizeof(SRWLOCK), "Windows SRWLOCK size doesn't match generic zsa_rwlock_t
// size");

void rwlock_init(zsa_rwlock_t *lock)
{
    InitializeSRWLock((SRWLOCK *)lock);
}

void rwlock_deinit(zsa_rwlock_t *lock)
{
    UNREFERENCED_PARAMETER(lock);
    // No work
}

void rwlock_acquire_read(zsa_rwlock_t *lock)
{
    AcquireSRWLockShared((SRWLOCK *)lock);
}

bool rwlock_try_acquire_read(zsa_rwlock_t *lock)
{
    return TryAcquireSRWLockShared((SRWLOCK *)lock);
}

void rwlock_acquire_write(zsa_rwlock_t *lock)
{
    AcquireSRWLockExclusive((SRWLOCK *)lock);
}

bool rwlock_try_acquire_write(zsa_rwlock_t *lock)
{
    return TryAcquireSRWLockExclusive((SRWLOCK *)lock);
}

void rwlock_release_read(zsa_rwlock_t *lock)
{
    ReleaseSRWLockShared((SRWLOCK *)lock);
}

void rwlock_release_write(zsa_rwlock_t *lock)
{
    ReleaseSRWLockExclusive((SRWLOCK *)lock);
}
