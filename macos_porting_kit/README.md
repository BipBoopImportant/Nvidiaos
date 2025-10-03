# NVIDIA Open-Source Driver macOS Porting Kit

## 1. Introduction

This porting kit is designed to provide a foundational starting point for the ambitious project of porting NVIDIA's open-source Linux GPU driver to macOS using Apple's modern **DriverKit** framework.

Directly compiling the Linux kernel modules on macOS is not possible due to fundamental differences in operating system architecture. This kit, therefore, provides a well-researched architectural blueprint and a set of skeleton files to guide a developer through the complex but feasible process of creating a new macOS-native driver that leverages NVIDIA's existing OS-agnostic code.

## 2. Architectural Overview

Our analysis of the NVIDIA open-gpu-kernel-modules repository revealed a critical architectural feature: the code is split into two distinct parts:

1.  **The OS-Agnostic Core (`nvKms`)**: Located primarily in the `src/` directory, this is a substantial library containing the core logic for controlling the GPU, managing resources, and performing modesetting. It is designed to be platform-independent.
2.  **The OS-Specific Kernel Interface**: Located in the `kernel-open/` directory, this layer acts as a "translation" or "adaptation" layer between the `nvKms` core and a specific operating system. The `nvidia-drm` module, for example, is the interface to the Linux kernel's Direct Rendering Manager (DRM).

**The Porting Strategy:**

The most effective strategy is not to port the Linux kernel interface, but to **write a new, native macOS kernel interface** that communicates with the same OS-agnostic `nvKms` core. This new interface will be a DriverKit extension (dext).

This kit is built around this strategy, providing the essential components to create this new macOS interface.

## 3. Components of this Kit

This kit contains three key files that form the skeleton of your new DriverKit driver:

*   **`Info.plist`**: This is the standard macOS property list file required for all drivers. It is pre-configured for a PCI-based DriverKit extension.
    *   **Action Required**: You must modify the `IOPCIMatch` key from `0xYYYY10de` to match the specific Device ID of your target NVIDIA GPU. `10de` is NVIDIA's Vendor ID. For example, for a device ID of `0x2204`, the string would be `0x220410de`.

*   **`nvkms_macos_shim.h`**: This C++ header file defines the API contract for a "shim" layer. This shim acts as the bridge between your new Swift/Objective-C/C++ macOS driver code and the C-based `nvKms` library. It contains commented function prototypes for essential tasks like initializing the device, managing resources, and handling modesetting.

*   **`NVIDIADriver.cpp`**: This is the C++ skeleton for your main driver class. It inherits from `IOService` and demonstrates the basic structure of a DriverKit driver, including the `init`, `Start`, and `Stop` methods. It includes placeholder comments (`// --- Developer TODO ---`) guiding you on where to integrate the shim and interact with the PCI device.

## 4. Developer's Roadmap

Here is a high-level, step-by-step guide to using this kit to build the driver:

### Step 1: Compile the OS-Agnostic Core (`nvKms`)

Your first major task is to compile the code in the `src/` directory of the NVIDIA repository into a library that your macOS driver can link against.

*   **Action**: Create a new build target (e.g., a static library `.a` or a dynamic library `.dylib`) for the `nvKms` code.
*   **Challenge**: You will need to adapt the existing `Makefile`-based build system. This will likely involve creating an Xcode project or a custom build script. You may also need to resolve minor platform differences, such as header file paths or missing symbols that need to be stubbed out.

### Step 2: Implement the Shim Layer

Create a `nvkms_macos_shim.cpp` file to implement the functions declared in `nvkms_macos_shim.h`.

*   **Action**: Write C++ functions that call the corresponding functions in the `nvKms` library you compiled in Step 1.
*   **Goal**: This layer will completely isolate your native macOS code from the complexities of the `nvKms` library's internals.

### Step 3: Flesh out the DriverKit Extension

Complete the implementation of the `NVIDIADriver.cpp` skeleton.

*   **Action**:
    1.  Implement the logic to read the PCI Vendor and Device IDs from the `IOPCIDevice` provider.
    2.  Use the shim to initialize `nvKms`, allocate a device, and manage its lifecycle.
    3.  Map the GPU's memory (BARs) into the driver's address space using the `PCIDriverKit` APIs. This is how you will send commands to the hardware.

### Step 4: Implement a User Client

For a user-space application (like the Metal driver) to communicate with your dext, you must create a User Client class.

*   **Action**: Create a new class that inherits from `IOUserClient`. This class will handle requests from user-space, such as "submit this command buffer" or "allocate this memory." It will then use the main `NVIDIADriver` class to pass these requests to the `nvKms` core via the shim.

### Step 5: Build the Metal User-Mode Driver

This is the final and most significant piece of the puzzle. macOS graphics drivers are split between the dext (which manages the hardware) and a user-mode plugin that implements the graphics API.

*   **Action**: You will need to create a new user-mode driver bundle that implements the **Metal** API. This driver will be loaded by applications that use Metal.
*   **How it works**: Your Metal driver will translate Metal API calls (e.g., creating command encoders, committing command buffers) into a series of commands that it sends to your dext via the User Client interface you created in Step 4. The dext will then forward these commands to the `nvKms` library, which will ultimately program the GPU hardware.

## 5. Conclusion

This project is a significant undertaking, requiring deep knowledge of both macOS driver development and GPU architecture. This porting kit is intended to give you a significant head start by providing a clear architectural path and the foundational code to begin your work. Good luck.