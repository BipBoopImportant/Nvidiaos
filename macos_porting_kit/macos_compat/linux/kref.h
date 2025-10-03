#ifndef MACOS_COMPAT_LINUX_KREF_H
#define MACOS_COMPAT_LINUX_KREF_H

#include <macos_compat/linux/atomic.h>

/*
 * This header provides a macOS-native implementation for the Linux kernel's
 * kref reference counting mechanism. It uses our existing atomic operations
 * compatibility layer to ensure thread-safe reference counting.
 */


//
// struct kref - The Linux kernel reference counter structure.
//
struct kref {
    atomic_t refcount;
};

//
// kref_init - Initializes a kref.
//
static inline void kref_init(struct kref *kref)
{
    atomic_set(&kref->refcount, 1);
}

//
// kref_get - Increments the reference count.
//
static inline void kref_get(struct kref *kref)
{
    atomic_inc(&kref->refcount);
}

//
// kref_put - Decrements the reference count and calls a release function
//            when the count reaches zero.
//
// @kref: The reference counter.
// @release: A function to be called to free the object when the refcount is 0.
//
// Returns 1 if the release function was called, 0 otherwise.
//
static inline int kref_put(struct kref *kref, void (*release)(struct kref *kref))
{
    if (atomic_dec_and_test(&kref->refcount)) {
        release(kref);
        return 1;
    }
    return 0;
}


#endif // MACOS_COMPAT_LINUX_KREF_H