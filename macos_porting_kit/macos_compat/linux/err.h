#ifndef MACOS_COMPAT_LINUX_ERR_H
#define MACOS_COMPAT_LINUX_ERR_H

#include <macos_compat/linux/types.h>
#include <stddef.h> // For NULL

/*
 * This header provides a macOS-native implementation for the Linux kernel's
 * error-handling pointer mechanism, typically found in <linux/err.h>.
 */

// In Linux, error codes are encoded in the upper range of the pointer address
// space. We can replicate this behavior. A valid pointer will not have these
// high bits set.
#define MAX_ERRNO       4095
#define IS_ERR_VALUE(x) ((unsigned long)(x) >= (unsigned long)-MAX_ERRNO)

//
// ERR_PTR - Encodes an error code into a pointer-like value.
//
static inline void *ERR_PTR(long error)
{
    return (void *) error;
}

//
// PTR_ERR - Extracts the error code from an error-encoded pointer.
//
static inline long PTR_ERR(const void *ptr)
{
    return (long) ptr;
}

//
// IS_ERR - Checks if a pointer is an error-encoded value.
//
static inline bool IS_ERR(const void *ptr)
{
    return IS_ERR_VALUE((unsigned long)ptr);
}

//
// IS_ERR_OR_NULL - Checks if a pointer is NULL or an error-encoded value.
//
static inline bool IS_ERR_OR_NULL(const void *ptr)
{
    return (!ptr) || IS_ERR_VALUE((unsigned long)ptr);
}


#endif // MACOS_COMPAT_LINUX_ERR_H