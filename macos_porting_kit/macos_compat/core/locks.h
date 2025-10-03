#ifndef MACOS_COMPAT_CORE_LOCKS_H
#define MACOS_COMPAT_CORE_LOCKS_H

#include <DriverKit/IOLib.h>
#include <DriverKit/IOLock.h>
#include <macos_compat/linux/types.h>

/*
 * This header provides a macOS-native implementation for NVIDIA's high-level
 * locking APIs, typically found in <core/locks.h>. It maps these APIs to
 * IOLock from Apple's DriverKit framework.
 */

//
// RM_LOCK_TYPE - An enumeration for different lock types in the RM.
// For our compatibility layer, we can treat them all as a standard IOLock,
// but we define the enum for source compatibility.
//
typedef enum
{
    RM_LOCK_TYPE_MUTEX,
    RM_LOCK_TYPE_SPINLOCK,
} RM_LOCK_TYPE;


//
// rm_lock_t - The basic lock handle.
// We will implement this as a wrapper around a DriverKit IOLock.
//
typedef struct
{
    IOLock *pLock;
} rm_lock;


//
// rmGpuLockInit - Initializes a lock.
//
static inline NV_STATUS rmGpuLockInit(rm_lock *pLock, RM_LOCK_TYPE type)
{
    if (!pLock) return NV_ERR_INVALID_ARGUMENT;
    pLock->pLock = IOLockAlloc();
    return (pLock->pLock != NULL) ? NV_OK : NV_ERR_NO_MEMORY;
}

//
// rmGpuLockDestroy - Destroys a lock.
//
static inline void rmGpuLockDestroy(rm_lock *pLock)
{
    if (pLock && pLock->pLock) {
        IOLockFree(pLock->pLock);
        pLock->pLock = NULL;
    }
}

//
// rmGpuLockAcquire - Acquires a lock.
//
static inline NV_STATUS rmGpuLockAcquire(rm_lock *pLock)
{
    if (!pLock || !pLock->pLock) return NV_ERR_INVALID_STATE;
    IOLockLock(pLock->pLock);
    return NV_OK;
}

//
// rmGpuLockRelease - Releases a lock.
//
static inline void rmGpuLockRelease(rm_lock *pLock)
{
    if (pLock && pLock->pLock) {
        IOLockUnlock(pLock->pLock);
    }
}

#endif // MACOS_COMPAT_CORE_LOCKS_H