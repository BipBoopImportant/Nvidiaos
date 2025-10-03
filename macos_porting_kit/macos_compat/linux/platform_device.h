#ifndef MACOS_COMPAT_LINUX_PLATFORM_DEVICE_H
#define MACOS_COMPAT_LINUX_PLATFORM_DEVICE_H

#include <macos_compat/linux/types.h>

/*
 * This header provides a macOS-native implementation for the Linux kernel's
 * <linux/platform_device.h> header.
 */

//
// struct platform_device - Represents a device that is not on a main bus.
//
// In DriverKit, all devices are represented by IOService subclasses. We
// provide a stub definition here for source code compatibility.
//
struct platform_device {
    const char *name;
    int id;
    // ... other fields as needed by the NVIDIA source ...
};


//
// struct platform_driver - Represents a driver for a platform device.
//
struct platform_driver {
    int (*probe)(struct platform_device *);
    int (*remove)(struct platform_device *);
    // ... other fields ...
};


#endif // MACOS_COMPAT_LINUX_PLATFORM_DEVICE_H