#ifndef MACOS_COMPAT_LINUX_KERNEL_H
#define MACOS_COMPAT_LINUX_KERNEL_H

/*
 * This header provides a macOS-native implementation for the Linux kernel's
 * <linux/kernel.h> header. This is a central header in Linux that provides
 * many basic types, macros, and function prototypes.
 */

#include <macos_compat/linux/types.h>
#include <macos_compat/linux/printk.h>
#include <stddef.h> // For offsetof

// Define standard min/max macros if they aren't already.
#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif

// Define Linux log level strings. Our printk implementation ignores these,
// but they need to be defined to allow the source code to compile.
#define KERN_EMERG   ""
#define KERN_ALERT   ""
#define KERN_CRIT    ""
#define KERN_ERR     ""
#define KERN_WARNING ""
#define KERN_NOTICE  ""
#define KERN_INFO    ""
#define KERN_DEBUG   ""

//
// DIV_ROUND_UP - Integer division that rounds up
//
#define DIV_ROUND_UP(n, d) (((n) + (d) - 1) / (d))

//
// container_of - A macro to get a pointer to a containing structure from a
// pointer to one of its members. This is a very common idiom in the Linux kernel.
//
#define container_of(ptr, type, member) ({ \
    const typeof( ((type *)0)->member ) *__mptr = (ptr); \
    (type *)( (char *)__mptr - offsetof(type, member) ); \
})


#endif // MACOS_COMPAT_LINUX_KERNEL_H