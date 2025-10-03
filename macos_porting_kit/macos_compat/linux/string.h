#ifndef MACOS_COMPAT_LINUX_STRING_H
#define MACOS_COMPAT_LINUX_STRING_H

#include <string.h>

/*
 * This header provides a macOS-native implementation for the Linux kernel's
 * <linux/string.h> header.
 *
 * For the most part, the standard C library string functions are available
 * in DriverKit and can be used directly. This header simply includes the
 * standard <string.h>. If the full build analysis reveals any Linux-specific
 * string functions, they can be implemented here.
 */

#endif // MACOS_COMPAT_LINUX_STRING_H