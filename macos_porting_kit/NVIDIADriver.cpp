/*
 * This file provides a skeleton for the main C++ class of the NVIDIA DriverKit
 * extension (dext). It defines the basic structure of a driver that interacts
 * with a PCI device.
 */

// Required DriverKit headers
#include <DriverKit/IOService.h>
#include <DriverKit/IOLib.h>
#include <PCIDriverKit/PCIDriverKit.h>

// The custom shim header that defines our interface to the OS-agnostic code
#include "nvkms_macos_shim.h"

/*
 * The _IVars struct holds the instance variables for our driver class.
 * This is a common pattern in I/O Kit and DriverKit development.
 */
struct NVIDIADriver_IVars
{
    // A pointer to the underlying PCI device provider
    IOPCIDevice* pciDevice;

    // A pointer to the OS-agnostic nvKms device object
    struct NvKmsKapiDevice* nvDevice;
};

/*
 * The main driver class. The name 'NVIDIADriver' must match the
 * 'IOClass' key in the Info.plist file.
 */
class NVIDIADriver : public IOService
{
public:
    virtual bool init() override;
    virtual kern_return_t Start(IOService* provider) override;
    virtual kern_return_t Stop(IOService* provider) override;
    virtual void free() override;

private:
    NVIDIADriver_IVars* ivars;
};

/*
 * init() is called to initialize a new instance of the driver.
 * Memory for instance variables should be allocated here.
 */
bool NVIDIADriver::init()
{
    if (!super::init()) {
        return false;
    }

    ivars = IONewZero(NVIDIADriver_IVars, 1);
    if (!ivars) {
        return false;
    }

    return true;
}

/*
 * free() is called when the driver instance is being destroyed.
 * All memory allocated in init() should be freed here.
 */
void NVIDIADriver::free()
{
    IOSafeDeleteNULL(ivars, NVIDIADriver_IVars, 1);
    super::free();
}

/*
 * Start() is the main entry point for the driver. It is called when the
 * system has matched this driver to a device.
 */
kern_return_t IMPL(NVIDIADriver, Start)
{
    kern_return_t ret = kIOReturnSuccess;

    // Start the superclass. This is required.
    ret = Start(provider, SUPERDISPATCH);
    if (ret != kIOReturnSuccess) {
        os_log(OS_LOG_DEFAULT, "NVIDIADriver: super::Start failed.");
        return ret;
    }

    // Cast the provider to an IOPCIDevice. This will be our main handle
    // for interacting with the hardware (e.g., reading BARs).
    ivars->pciDevice = OSDynamicCast(IOPCIDevice, provider);
    if (!ivars->pciDevice) {
        os_log(OS_LOG_DEFAULT, "NVIDIADriver: Provider is not an IOPCIDevice.");
        return kIOReturnUnsupported;
    }

    os_log(OS_LOG_DEFAULT, "NVIDIADriver: Starting up.");

    // --- Developer TODO ---
    // This is where the core driver initialization logic goes.

    // 1. Initialize the shim layer. This should load the OS-agnostic
    //    nvKms library and resolve its function pointers.
    if (nvkms_shim_init() != 0) {
        os_log(OS_LOG_DEFAULT, "NVIDIADriver: Failed to initialize nvkms shim.");
        goto fail;
    }

    // 2. Open the device. This allows you to access PCI config space.
    ret = ivars->pciDevice->Open(this, 0);
    if (ret != kIOReturnSuccess) {
        os_log(OS_LOG_DEFAULT, "NVIDIADriver: Failed to open PCI device.");
        goto fail;
    }

    // 3. Prepare parameters for allocating the nvKms device. You will need
    //    to get the GPU ID, likely from the PCI device information.
    struct NvKmsKapiAllocateDeviceParams params = {};
    // params.gpuId = ... ; // Get this from the hardware
    // params.privateData = this; // Pass a pointer to this driver instance
    // params.eventCallback = ...; // A static C function to handle callbacks

    // 4. Allocate the nvKms device via the shim.
    ivars->nvDevice = nvkms_shim_allocate_device(&params);
    if (ivars->nvDevice == NULL) {
        os_log(OS_LOG_DEFAULT, "NVIDIADriver: nvkms_shim_allocate_device failed.");
        goto fail_after_open;
    }

    // 5. Grab modesetting ownership.
    if (!nvkms_shim_grab_ownership(ivars->nvDevice)) {
         os_log(OS_LOG_DEFAULT, "NVIDIADriver: nvkms_shim_grab_ownership failed.");
         goto fail_after_alloc;
    }

    // 6. Register the service. This makes it available to user-space clients.
    RegisterService();

    os_log(OS_LOG_DEFAULT, "NVIDIADriver: Started successfully.");

    return kIOReturnSuccess;

fail_after_alloc:
    nvkms_shim_free_device(ivars->nvDevice);
fail_after_open:
    ivars->pciDevice->Close(this);
fail:
    Stop(provider, SUPERDISPATCH);
    return kIOReturnError;
}

/*
 * Stop() is called when the driver is being unloaded.
 * It should reverse everything that was done in Start().
 */
kern_return_t IMPL(NVIDIADriver, Stop)
{
    os_log(OS_LOG_DEFAULT, "NVIDIADriver: Stopping.");

    // --- Developer TODO ---
    // This is where the driver teardown logic goes.

    // 1. Release modesetting ownership.
    if (ivars->nvDevice) {
        nvkms_shim_release_ownership(ivars->nvDevice);
    }

    // 2. Free the nvKms device.
    if (ivars->nvDevice) {
        nvkms_shim_free_device(ivars->nvDevice);
        ivars->nvDevice = NULL;
    }

    // 3. Close the PCI device.
    if (ivars->pciDevice) {
        ivars->pciDevice->Close(this);
    }

    // 4. Tear down the shim layer.
    nvkms_shim_teardown();

    // Stop the superclass.
    return Stop(provider, SUPERDISPATCH);
}