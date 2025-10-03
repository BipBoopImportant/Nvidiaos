#ifndef MACOS_COMPAT_LINUX_MUTEX_H
#define MACOS_COMPAT_LINUX_MUTEX_H

#include <DriverKit/IOLib.h>
#include <DriverKit/IOLock.h>

/*
 * This header provides a macOS-native implementation for the Linux kernel's
 * mutex locking mechanism. It maps the Linux mutex API to IOLock from
 * Apple's DriverKit framework.
 */

//
// struct mutex - The Linux mutex structure.
//
// We implement this as a wrapper around a DriverKit IOLock.
//
struct mutex {
    IOLock *lock;
};

//
// mutex_init - Initializes a mutex.
//
static inline void mutex_init(struct mutex *lock)
{
    lock->lock = IOLockAlloc();
}

//
// mutex_lock - Acquires the mutex.
//
// This is a sleeping lock, which means the thread will be put to sleep
// if the lock is not available. This maps directly to IOLockLock.
//
static inline void mutex_lock(struct mutex *lock)
{
    if (lock && lock->lock) {
        IOLockLock(lock->lock);
    }
}

//
// mutex_unlock - Releases the mutex.
//
static inline void mutex_unlock(struct mutex *lock)
{
    if (lock && lock->lock) {
        IOLockUnlock(lock->lock);
    }
}

//
// mutex_destroy (non-standard, but good practice for our implementation)
//
// Frees the allocated IOLock.
//
static inline void mutex_destroy(struct mutex *lock)
{
    if (lock && lock->lock) {
        IOLockFree(lock->lock);
        lock->lock = NULL;
    }
}

#endif // MACOS_COMPAT_LINUX_MUTEX_H