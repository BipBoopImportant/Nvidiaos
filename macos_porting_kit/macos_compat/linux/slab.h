#ifndef MACOS_COMPAT_LINUX_SLAB_H
#define MACOS_COMPAT_LINUX_SLAB_H

#include <macos_compat/linux/port_mem.h>

/*
 * This header provides a macOS-native implementation for the Linux kernel's
 * slab allocator functions, typically found in <linux/slab.h>.
 */

// Define the GFP (Get Free Pages) flags as no-ops. These flags control
// the behavior of the Linux memory allocator (e.g., whether it can sleep).
// The DriverKit IOMalloc has a simpler model, so we define these to allow
// the source to compile.
#define GFP_KERNEL      0
#define GFP_ATOMIC      0

//
// kmalloc - Allocate kernel memory.
//
// This is the primary memory allocation function in the Linux kernel. We
// map it directly to our portMemAllocNonPaged function, which uses IOMalloc.
// The 'flags' argument is ignored in our implementation.
//
static inline void *kmalloc(size_t size, unsigned int flags)
{
    return portMemAllocNonPaged(size);
}

//
// kfree - Free kernel memory.
//
// This maps directly to our portMemFree function.
//
static inline void kfree(const void *p)
{
    portMemFree((void *)p);
}

//
// kzalloc - Allocate kernel memory and zero it.
//
static inline void *kzalloc(size_t size, unsigned int flags)
{
    void *p = kmalloc(size, flags);
    if (p) {
        memset(p, 0, size);
    }
    return p;
}


#endif // MACOS_COMPAT_LINUX_SLAB_H