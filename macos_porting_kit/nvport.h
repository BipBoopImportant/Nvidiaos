#ifndef NVPORT_H
#define NVPORT_H

/*
 * This is a platform-specific header for the macOS port of the NVIDIA driver.
 * It provides definitions for macros that are expected by the OS-agnostic
 * source code but are specific to the operating system and build environment.
 *
 * This file is included via the CFLAGS in the build script.
 */

// Define the build type. For now, we'll define it as a non-checked (release) build.
#define PORT_IS_CHECKED_BUILD 0

// Define a breakpoint macro. On macOS, this can be __builtin_debugtrap() or an
// assembly instruction. For a stub, we can make it a no-op.
#define PORT_BREAKPOINT_CHECKED()

// Define coverage macros as no-ops for now.
#define PORT_COVERAGE_PUSH_OFF()
#define PORT_COVERAGE_POP()

#endif // NVPORT_H