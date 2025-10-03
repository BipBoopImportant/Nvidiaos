#ifndef MACOS_COMPAT_LINUX_MODULE_H
#define MACOS_COMPAT_LINUX_MODULE_H

/*
 * This header provides a macOS-native implementation for the Linux kernel's
 * <linux/module.h> header. It defines the standard module macros as no-ops,
 * as the DriverKit extension lifecycle is handled by the `Start` and `Stop`
 * methods of the IOService subclass, not by module_init/module_exit.
 */

// Define module metadata macros as no-ops. The equivalent information
// is stored in the Info.plist file for a macOS driver.
#define MODULE_LICENSE(license)
#define MODULE_AUTHOR(author)
#define MODULE_DESCRIPTION(desc)
#define MODULE_VERSION(version)
#define MODULE_FIRMWARE(firmware)

// Define module lifecycle macros as no-ops.
#define module_init(init_fn)
#define module_exit(exit_fn)

#endif // MACOS_COMPAT_LINUX_MODULE_H