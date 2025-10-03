/*
 * This file provides a functional simulation of the nvKms shim layer.
 * It manages a fake NvKmsKapiDevice object and returns realistic-looking
 * data, allowing the main DriverKit driver to be developed and tested
 * without the real nvKms library.
 */

#include "nvkms_macos_shim.h"
#include <stdio.h>
#include <stdlib.h> // For malloc/free
#include <string.h> // For memset

// --- Simulated Data Structures ---
// In a real port, these definitions would be replaced by including the
// actual headers from the NVIDIA source. For our simulation, we define
// them locally.

struct NvKmsKapiDevice {
    uint32_t gpuId;
    bool ownership_grabbed;
};

// A simplified version of the real struct, with just a few key fields.
struct NvKmsKapiDeviceResourcesInfo {
    struct {
        uint32_t minWidthInPixels;
        uint32_t minHeightInPixels;
        uint32_t maxWidthInPixels;
        uint32_t maxHeightInPixels;
        uint32_t maxCursorSizeInPixels;
        bool supportsSyncpts;
    } caps;
};

struct NvKmsKapiAllocateDeviceParams {
    uint32_t gpuId;
    void* privateData;
    void (*eventCallback)(const struct NvKmsKapiEvent* event);
};


// --- Shim Implementation ---

int nvkms_shim_init(void) {
    printf("NVKMS_SHIM: nvkms_shim_init() called.\n");
    return 0; // Success
}

void nvkms_shim_teardown(void) {
    printf("NVKMS_SHIM: nvkms_shim_teardown() called.\n");
}

NvKmsKapiDevice* nvkms_shim_allocate_device(NvKmsKapiAllocateDeviceParams* params) {
    printf("NVKMS_SHIM: Simulating device allocation for GPU ID 0x%x.\n", params->gpuId);

    if (!params) {
        return NULL;
    }

    NvKmsKapiDevice* fake_device = (NvKmsKapiDevice*)malloc(sizeof(NvKmsKapiDevice));
    if (!fake_device) {
        return NULL;
    }

    memset(fake_device, 0, sizeof(NvKmsKapiDevice));
    fake_device->gpuId = params->gpuId;
    fake_device->ownership_grabbed = false;

    printf("NVKMS_SHIM: Fake device created at %p.\n", fake_device);
    return fake_device;
}

void nvkms_shim_free_device(NvKmsKapiDevice* pDevice) {
    printf("NVKMS_SHIM: Freeing fake device at %p.\n", pDevice);
    if (pDevice) {
        free(pDevice);
    }
}

bool nvkms_shim_get_device_resources_info(NvKmsKapiDevice* pDevice, NvKmsKapiDeviceResourcesInfo* pResInfo) {
    printf("NVKMS_SHIM: Populating fake device resources for device %p.\n", pDevice);
    if (!pDevice || !pResInfo) {
        return false;
    }

    // Populate with some plausible-looking dummy data.
    pResInfo->caps.minWidthInPixels = 640;
    pResInfo->caps.minHeightInPixels = 480;
    pResInfo->caps.maxWidthInPixels = 7680;
    pResInfo->caps.maxHeightInPixels = 4320;
    pResInfo->caps.maxCursorSizeInPixels = 256;
    pResInfo->caps.supportsSyncpts = true;

    return true;
}

bool nvkms_shim_grab_ownership(NvKmsKapiDevice* pDevice) {
    printf("NVKMS_SHIM: Grabbing ownership of fake device %p.\n", pDevice);
    if (pDevice) {
        pDevice->ownership_grabbed = true;
        return true;
    }
    return false;
}

void nvkms_shim_release_ownership(NvKmsKapiDevice* pDevice) {
    printf("NVKMS_SHIM: Releasing ownership of fake device %p.\n", pDevice);
    if (pDevice) {
        pDevice->ownership_grabbed = false;
    }
}

bool nvkms_shim_declare_event_interest(NvKmsKapiDevice* pDevice, uint32_t event_mask) {
    printf("NVKMS_SHIM: Declaring event interest with mask 0x%x for fake device %p.\n", event_mask, pDevice);
    return true;
}