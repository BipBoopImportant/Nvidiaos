#ifndef MACOS_COMPAT_LINUX_SPINLOCK_H
#define MACOS_COMPAT_LINUX_SPINLOCK_H

#include <DriverKit/IOSimpleLock.h>

/*
 * This header provides a macOS-native implementation for the Linux kernel's
 * spinlock mechanism. It maps the Linux spinlock API to IOSimpleLock from
 * Apple's DriverKit framework.
 */

// Define spinlock_t as a wrapper around IOSimpleLock.
typedef struct {
    IOSimpleLock *lock;
} spinlock_t;

//
// spin_lock_init
//
// Initializes the spinlock. In Linux, this is often a macro. Here, we
// implement it as a function that allocates a new IOSimpleLock.
//
static inline void spin_lock_init(spinlock_t *sl)
{
    sl->lock = IOSimpleLockAlloc();
}

//
// spin_lock
//
// Acquires the spinlock. This is a blocking call that will spin until the
// lock is acquired. Maps directly to IOSimpleLockLock.
//
static inline void spin_lock(spinlock_t *sl)
{
    if (sl && sl->lock) {
        IOSimpleLockLock(sl->lock);
    }
}

//
// spin_unlock
//
// Releases the spinlock. Maps directly to IOSimpleLockUnlock.
//
static inline void spin_unlock(spinlock_t *sl)
{
    if (sl && sl->lock) {
        IOSimpleLockUnlock(sl->lock);
    }
}

//
// spin_lock_destroy (non-standard, but good practice for our implementation)
//
// Frees the allocated IOSimpleLock. This should be called when the spinlock
// is no longer needed.
//
static inline void spin_lock_destroy(spinlock_t *sl)
{
    if (sl && sl->lock) {
        IOSimpleLockFree(sl->lock);
        sl->lock = NULL;
    }
}


#endif // MACOS_COMPAT_LINUX_SPINLOCK_H