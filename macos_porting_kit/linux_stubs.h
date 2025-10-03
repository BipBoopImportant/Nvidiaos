#ifndef LINUX_STUBS_H
#define LINUX_STUBS_H

#include <stdio.h>
#include <stdarg.h>

/*
 * This header provides stub implementations for common Linux kernel functions
 * to allow the NVIDIA OS-agnostic source code to compile on macOS.
 *
 * This file is force-included by the build script.
 */

// Stub for the Linux kernel's printk function.
// This simple implementation redirects kernel logs to standard output (printf).
// A more advanced implementation for a real driver would use os_log.
static inline int printk(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int ret = vprintf(fmt, args);
    va_end(args);
    return ret;
}

#endif // LINUX_STUBS_H