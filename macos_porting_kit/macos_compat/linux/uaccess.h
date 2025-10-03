#ifndef MACOS_COMPAT_LINUX_UACCESS_H
#define MACOS_COMPAT_LINUX_UACCESS_H

#include <string.h> // For memcpy
#include <macos_compat/linux/types.h>

/*
 * This header provides a macOS-native implementation for the Linux kernel's
 * user memory access functions, typically found in <linux/uaccess.h>.
 *
 * NOTE: This implementation uses direct memory copies (memcpy). In a real,
 * security-conscious DriverKit driver, communication with user space should
 * be handled through the IOUserClient mechanism, with careful validation of
 * all inputs. This implementation is provided to allow the NVIDIA source
 * code to compile, but it should be reviewed and hardened during the
 * development of the final driver.
 */


//
// copy_to_user - Copy a block of data from the kernel to user space.
//
// @to:   Destination address in user space.
// @from: Source address in kernel space.
// @n:    Number of bytes to copy.
//
// Returns 0 on success, or the number of bytes that could not be copied.
//
static inline unsigned long
copy_to_user(void __user *to, const void *from, unsigned long n)
{
    memcpy(to, from, n);
    return 0; // Assume success for this compatibility implementation
}

//
// copy_from_user - Copy a block of data from user space to the kernel.
//
// @to:   Destination address in kernel space.
// @from: Source address in user space.
// @n:    Number of bytes to copy.
//
// Returns 0 on success, or the number of bytes that could not be copied.
//
static inline unsigned long
copy_from_user(void *to, const void __user *from, unsigned long n)
{
    memcpy(to, from, n);
    return 0; // Assume success for this compatibility implementation
}

#endif // MACOS_COMPAT_LINUX_UACCESS_H