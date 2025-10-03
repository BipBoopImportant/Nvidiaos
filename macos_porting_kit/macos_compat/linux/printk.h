#ifndef MACOS_COMPAT_LINUX_PRINTK_H
#define MACOS_COMPAT_LINUX_PRINTK_H

#include <DriverKit/IOLib.h>
#include <stdarg.h>
#include <stdio.h>

/*
 * This header provides a macOS-native implementation of the Linux kernel's
 * printk function. It uses os_log, the standard logging facility for
 * DriverKit extensions, to redirect kernel logs to the macOS unified
 * logging system.
 */

static inline int printk(const char *fmt, ...)
{
    va_list args;
    char log_buffer[256];

    va_start(args, fmt);
    vsnprintf(log_buffer, sizeof(log_buffer), fmt, args);
    va_end(args);

    // Use the default os_log handle and log at the default level.
    // In a real driver, you might create a custom log handle.
    os_log(OS_LOG_DEFAULT, "%s", log_buffer);

    // Return the number of characters printed, mimicking the behavior of printk.
    return (int)strlen(log_buffer);
}

#endif // MACOS_COMPAT_LINUX_PRINTK_H