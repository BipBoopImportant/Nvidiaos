#ifndef MACOS_COMPAT_LINUX_PORT_MEM_H
#define MACOS_COMPAT_LINUX_PORT_MEM_H

#include <DriverKit/IOLib.h>
#include <string.h> // For memcpy, memset

/*
 * This header provides macOS-native implementations for NVIDIA's 'nvport'
 * memory allocation functions. It maps the nvport APIs to the corresponding
 * memory management functions in Apple's DriverKit framework.
 */

//
// portMemAllocNonPaged
//
// The Linux equivalent of this function is typically a wrapper around kmalloc
// or vmalloc with the GFP_KERNEL flag. The macOS DriverKit equivalent is
// IOMalloc.
//
static inline void* portMemAllocNonPaged(size_t size)
{
    // IOMalloc is the standard way to allocate memory in DriverKit.
    return IOMalloc(size);
}

//
// portMemFree
//
// The Linux equivalent is kfree or vfree. The macOS DriverKit equivalent
// is IOFree.
//
static inline void portMemFree(void* p)
{
    if (p) {
        IOFree(p);
    }
}

//
// portMemCopy
//
// This function can be mapped directly to the standard C library's memcpy,
// which is available in DriverKit.
//
static inline void* portMemCopy(void* dest, size_t destSize, const void* src, size_t count)
{
    // It's good practice to ensure the destination buffer is large enough,
    // though the original nvport API may not enforce this.
    if (destSize < count) {
        return NULL;
    }
    return memcpy(dest, src, count);
}

//
// portMemSet
//
// This function can be mapped directly to the standard C library's memset,
// which is available in DriverKit.
//
static inline void* portMemSet(void* dest, int c, size_t count)
{
    return memset(dest, c, count);
}

#endif // MACOS_COMPAT_LINUX_PORT_MEM_H