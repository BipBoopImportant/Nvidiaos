#ifndef NVKMS_MACOS_SHIM_H
#define NVKMS_MACOS_SHIM_H

/*
 * This header file defines the API contract for the "shim" or "wrapper" layer
 * that connects the macOS DriverKit driver to the OS-agnostic nvKms library.
 *
 * The functions declared here are C-style functions that your macOS driver will
 * call. The implementation of these functions (in a corresponding .cpp file)
 * will be responsible for loading the nvKms library (which you will need to
 * compile as a static or dynamic library) and calling the appropriate functions
 * within it.
 *
 * This approach decouples the macOS-specific driver logic from the core,
 * cross-platform NVIDIA driver logic.
 */

#include <stdint.h>

// Forward-declare the opaque structures that will be used.
// The actual definitions of these are in the NVIDIA source.
// For the shim, we only need to pass pointers around.
struct NvKmsKapiDevice;
struct NvKmsKapiDeviceResourcesInfo;
struct NvKmsKapiAllocateDeviceParams;
struct NvKmsKapiEvent;

// --- Shim Initialization ---

/**
 * @brief Initializes the nvKms shim layer.
 *
 * This function should be called once when the driver starts. It is responsible
 * for loading the nvKms library (e.g., using dlopen if it's a dynamic library)
 * and resolving the function pointers to the core nvKms functions.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int nvkms_shim_init(void);

/**
 * @brief Deinitializes the nvKms shim layer.
 *
 * This function should be called when the driver is stopping. It should unload
 * the nvKms library.
 */
void nvkms_shim_teardown(void);


// --- Device Management ---

/**
 * @brief Allocates and initializes an NvKmsKapiDevice instance.
 *
 * This is the first step in interacting with a GPU. It corresponds to the
 * `nvKms->allocateDevice()` call in the Linux driver.
 *
 * @param params Parameters for device allocation, including the GPU ID.
 * @return A pointer to the allocated NvKmsKapiDevice, or NULL on failure.
 */
NvKmsKapiDevice* nvkms_shim_allocate_device(NvKmsKapiAllocateDeviceParams* params);

/**
 * @brief Frees an NvKmsKapiDevice instance.
 *
 * @param pDevice The device to free.
 */
void nvkms_shim_free_device(NvKmsKapiDevice* pDevice);

/**
 * @brief Retrieves information about the resources available on a device.
 *
 * This function is used to get details about the GPU's capabilities, such as
 * supported resolutions, memory size, etc. It corresponds to the
 * `nvKms->getDeviceResourcesInfo()` call.
 *
 * @param pDevice The device to query.
 * @param pResInfo A pointer to a structure to be filled with resource info.
 * @return true on success, false on failure.
 */
bool nvkms_shim_get_device_resources_info(NvKmsKapiDevice* pDevice, NvKmsKapiDeviceResourcesInfo* pResInfo);


// --- Modesetting and Display Management ---

/**
 * @brief Takes exclusive ownership of modesetting for a device.
 *
 * Corresponds to `nvKms->grabOwnership()`. This must be called before any
 * modesetting operations can be performed.
 *
 * @param pDevice The device to take ownership of.
 * @return true on success, false on failure.
 */
bool nvkms_shim_grab_ownership(NvKmsKapiDevice* pDevice);

/**
 * @brief Releases modesetting ownership.
 *
 * @param pDevice The device to release ownership of.
 */
void nvkms_shim_release_ownership(NvKmsKapiDevice* pDevice);

/**
 * @brief Registers interest in specific types of events from the nvKms library.
 *
 * Events include things like display connection/disconnection. Corresponds to
 * `nvKms->declareEventInterest()`.
 *
 * @param pDevice The device.
 * @param event_mask A bitmask of events to subscribe to.
 * @return true on success, false on failure.
 */
bool nvkms_shim_declare_event_interest(NvKmsKapiDevice* pDevice, uint32_t event_mask);


#endif // NVKMS_MACOS_SHIM_H