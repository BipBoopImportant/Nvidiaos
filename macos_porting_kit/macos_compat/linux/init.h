#ifndef MACOS_COMPAT_LINUX_INIT_H
#define MACOS_COMPAT_LINUX_INIT_H

/*
 * This header provides a macOS-native implementation for the Linux kernel's
 * <linux/init.h> header. This header defines macros used to mark
 * initialization and exit code.
 */

//
// In Linux, these macros are used to place code in special linker sections,
// allowing the kernel to free initialization code after boot. In DriverKit,
// the driver's lifecycle is different, so we define these as no-ops to
// allow the source code to compile without modification.
//
#define __init
#define __initdata
#define __exit
#define __exitdata

#endif // MACOS_COMPAT_LINUX_INIT_H