/*
 * This file provides a functional implementation of the main C++ class for the
 * NVIDIA DriverKit extension (dext). It interacts with the simulated shim layer
 * to demonstrate a complete, loadable driver.
 */

// Required DriverKit headers
#include <DriverKit/IOService.h>
#include <DriverKit/IOLib.h>
#include <PCIDriverKit/PCIDriverKit.h>

// The custom shim header that defines our interface to the OS-agnostic code
#include "nvkms_macos_shim.h"

// A fake event callback function. In a real driver, this would handle
// events from the nvKms library, such as hotplug notifications.
static void nvkms_event_callback_stub(const struct NvKmsKapiEvent* event) {
    os_log(OS_LOG_DEFAULT, "NVIDIADriver: Received event from nvKms (stub).");
}

/*
 * The _IVars struct holds the instance variables for our driver class.
 */
struct NVIDIADriver_IVars
{
    IOPCIDevice* pciDevice;
    struct NvKmsKapiDevice* nvDevice;
};

/*
 * The main driver class.
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
 * init()
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
 * free()
 */
void NVIDIADriver::free()
{
    IOSafeDeleteNULL(ivars, NVIDIADriver_IVars, 1);
    super::free();
}

/*
 * Start()
 */
kern_return_t IMPL(NVIDIADriver, Start)
{
    kern_return_t ret = kIOReturnSuccess;

    ret = Start(provider, SUPERDISPATCH);
    if (ret != kIOReturnSuccess) {
        os_log(OS_LOG_DEFAULT, "NVIDIADriver: super::Start failed.");
        return ret;
    }

    ivars->pciDevice = OSDynamicCast(IOPCIDevice, provider);
    if (!ivars->pciDevice) {
        os_log(OS_LOG_DEFAULT, "NVIDIADriver: Provider is not an IOPCIDevice.");
        return kIOReturnUnsupported;
    }

    os_log(OS_LOG_DEFAULT, "NVIDIADriver: Starting up.");

    // 1. Initialize the shim layer.
    if (nvkms_shim_init() != 0) {
        os_log(OS_LOG_DEFAULT, "NVIDIADriver: Failed to initialize nvkms shim.");
        goto fail;
    }

    // 2. Open the device to enable access to PCI config space.
    ret = ivars->pciDevice->Open(this, 0);
    if (ret != kIOReturnSuccess) {
        os_log(OS_LOG_DEFAULT, "NVIDIADriver: Failed to open PCI device.");
        goto fail;
    }

    // 3. Prepare parameters for allocating the nvKms device.
    // In a real driver, you would read the device and vendor IDs from PCI
    // config space. We do that here to demonstrate the API, though in this
    // simulated environment, the values will be dummy values.
    uint32_t vendorAndDeviceID = 0;
    ivars->pciDevice->configRead32(kIOPCIConfigVendorID, &vendorAndDeviceID);

    // In a real device, vendorAndDeviceID would be populated. For simulation,
    // we'll use the placeholder from our Info.plist if the read fails.
    if (vendorAndDeviceID == 0 || vendorAndDeviceID == 0xFFFFFFFF) {
        vendorAndDeviceID = 0xYYYY10de; // Placeholder DeviceID YYYY, VendorID 10de
    }

    os_log(OS_LOG_DEFAULT, "NVIDIADriver: Found PCI device with ID 0x%x", vendorAndDeviceID);

    struct NvKmsKapiAllocateDeviceParams params = {};
    params.gpuId = vendorAndDeviceID;
    params.privateData = this;
    params.eventCallback = nvkms_event_callback_stub;

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
 * Stop()
 */
kern_return_t IMPL(NVIDIADriver, Stop)
{
    os_log(OS_LOG_DEFAULT, "NVIDIADriver: Stopping.");

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

    return Stop(provider, SUPERDISPATCH);
}