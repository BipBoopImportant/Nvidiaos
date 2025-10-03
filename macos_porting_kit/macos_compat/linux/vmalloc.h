#ifndef MACOS_COMPAT_LINUX_VMALLOC_H
#define MACOS_COMPAT_LINUX_VMALLOC_H

#include <macos_compat/linux/slab.h>

/*
 * This header provides a macOS-native implementation for the Linux kernel's
 * vmalloc functions, typically found in <linux/vmalloc.h>.
 */

//
// vmalloc - Allocate a virtually contiguous memory region.
//
// In Linux, this is different from kmalloc in that the underlying physical
// pages are not necessarily contiguous. For the purposes of a DriverKit
// compatibility layer, the distinction is less important, as IOMalloc
// handles the details of the underlying allocation. We can map vmalloc
// directly to our kmalloc implementation.
//
static inline void *vmalloc(unsigned long size)
{
    // For our purposes, vmalloc can be treated the same as kmalloc.
    return kmalloc(size, GFP_KERNEL);
}

//
// vfree - Free a vmalloc'd memory region.
//
static inline void vfree(const void *addr)
{
    kfree(addr);
}

#endif // MACOS_COMPAT_LINUX_VMALLOC_H