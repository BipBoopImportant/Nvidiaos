/*
 * This file provides a stub implementation of the nvKms shim layer.
 * It allows the main DriverKit driver to be compiled and linked before the
 * full nvKms library is available.
 *
 * The real implementation of these functions will call into the compiled
 * nvKms static library.
 */

#include "nvkms_macos_shim.h"
#include <stdio.h> // For printf

// --- Shim Initialization ---

int nvkms_shim_init(void) {
    printf("NVKMS_SHIM: nvkms_shim_init() called.\n");
    // In a real implementation, this would dlopen the library and
    // resolve function pointers.
    return 0; // Success
}

void nvkms_shim_teardown(void) {
    printf("NVKMS_SHIM: nvkms_shim_teardown() called.\n");
    // In a real implementation, this would dlclose the library.
}


// --- Device Management ---

NvKmsKapiDevice* nvkms_shim_allocate_device(NvKmsKapiAllocateDeviceParams* params) {
    printf("NVKMS_SHIM: nvkms_shim_allocate_device() called.\n");
    // This is a stub, so we return NULL. The real implementation would
    // call the actual nvKms function.
    return NULL;
}

void nvkms_shim_free_device(NvKmsKapiDevice* pDevice) {
    printf("NVKMS_SHIM: nvkms_shim_free_device() called.\n");
    // No-op in the stub.
}

bool nvkms_shim_get_device_resources_info(NvKmsKapiDevice* pDevice, NvKmsKapiDeviceResourcesInfo* pResInfo) {
    printf("NVKMS_SHIM: nvkms_shim_get_device_resources_info() called.\n");
    // Return false as we can't provide real info in the stub.
    return false;
}


// --- Modesetting and Display Management ---

bool nvkms_shim_grab_ownership(NvKmsKapiDevice* pDevice) {
    printf("NVKMS_SHIM: nvkms_shim_grab_ownership() called.\n");
    // Return true to allow the calling code to proceed, even though
    // no real ownership is being grabbed in the stub.
    return true;
}

void nvkms_shim_release_ownership(NvKmsKapiDevice* pDevice) {
    printf("NVKMS_SHIM: nvkms_shim_release_ownership() called.\n");
    // No-op in the stub.
}

bool nvkms_shim_declare_event_interest(NvKmsKapiDevice* pDevice, uint32_t event_mask) {
    printf("NVKMS_SHIM: nvkms_shim_declare_event_interest() called with mask 0x%x.\n", event_mask);
    return true;
}