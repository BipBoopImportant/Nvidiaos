#ifndef MACOS_COMPAT_LINUX_PCI_H
#define MACOS_COMPAT_LINUX_PCI_H

#include <DriverKit/IOLib.h>
#include <PCIDriverKit/PCIDriverKit.h>

/*
 * This header provides a macOS-native implementation for the Linux kernel's
 * PCI subsystem APIs. It maps Linux PCI functions to methods on the
 * IOPCIDevice object provided by Apple's DriverKit framework.
 */

//
// The 'pci_dev' struct in Linux is a complex structure that represents a
// PCI device. For our compatibility layer, we can define it as a simple
// wrapper around the IOPCIDevice object, which is the native representation
// of a PCI device in DriverKit.
//
struct pci_dev {
    IOPCIDevice* pciDevice;
};

//
// pci_get_device - Finds a PCI device from vendor and device IDs.
//
// In DriverKit, device matching is handled by the Info.plist. The driver's
// Start() method is called with the IOPCIDevice as the provider. Therefore,
// we don't need to "find" the device in the same way. This function can be
// a no-op or a simple helper if needed, but the primary mechanism is different.
// For now, we'll stub it.
//
static inline struct pci_dev* pci_get_device(unsigned int vendor, unsigned int device, struct pci_dev *from)
{
    // This is fundamentally different in DriverKit. The system matches the
    // device and starts our driver. We don't search for devices.
    return NULL;
}

//
// pci_enable_device - Enables the PCI device.
//
// This maps to the Open() method on IOPCIDevice.
//
static inline int pci_enable_device(struct pci_dev *dev)
{
    if (!dev || !dev->pciDevice) {
        return -EINVAL;
    }
    // The 'owner' parameter for Open() is the IOService instance. In a real
    // implementation, we would need a pointer to our main driver object here.
    // For now, we pass 'NULL', which may need to be adjusted.
    kern_return_t kr = dev->pciDevice->Open(NULL, 0);
    return (kr == kIOReturnSuccess) ? 0 : -EIO;
}

//
// pci_disable_device - Disables the PCI device.
//
// This maps to the Close() method on IOPCIDevice.
//
static inline void pci_disable_device(struct pci_dev *dev)
{
    if (dev && dev->pciDevice) {
        dev->pciDevice->Close(NULL);
    }
}

//
// pci_read_config_dword - Reads a 32-bit value from PCI config space.
//
// This maps directly to the configRead32 method on IOPCIDevice.
//
static inline int pci_read_config_dword(const struct pci_dev *dev, int where, u32 *val)
{
    if (!dev || !dev->pciDevice || !val) {
        return -EINVAL;
    }
    kern_return_t kr = dev->pciDevice->configRead32(where, val);
    return (kr == kIOReturnSuccess) ? 0 : -EIO;
}

//
// pci_write_config_dword - Writes a 32-bit value to PCI config space.
//
// This maps directly to the configWrite32 method on IOPCIDevice.
//
static inline int pci_write_config_dword(const struct pci_dev *dev, int where, u32 val)
{
    if (!dev || !dev->pciDevice) {
        return -EINVAL;
    }
    kern_return_t kr = dev->pciDevice->configWrite32(where, val);
    return (kr == kIOReturnSuccess) ? 0 : -EIO;
}


// Define common PCI config space offsets for compatibility
#define PCI_VENDOR_ID   0x00
#define PCI_DEVICE_ID   0x02
#define PCI_COMMAND     0x04
#define PCI_STATUS      0x06
#define PCI_REVISION_ID 0x08
#define PCI_CLASS_PROG  0x0a
#define PCI_CLASS_DEVICE 0x0a
#define PCI_CACHE_LINE_SIZE 0x0c
#define PCI_LATENCY_TIMER 0x0d
#define PCI_HEADER_TYPE 0x0e
#define PCI_BIST        0x0f
#define PCI_BASE_ADDRESS_0 0x10
#define PCI_BASE_ADDRESS_1 0x14
#define PCI_BASE_ADDRESS_2 0x18
#define PCI_BASE_ADDRESS_3 0x1c
#define PCI_BASE_ADDRESS_4 0x20
#define PCI_BASE_ADDRESS_5 0x24

#endif // MACOS_COMPAT_LINUX_PCI_H