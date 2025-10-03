#!/bin/bash

#
# build_dext.sh - A comprehensive script to build the NVIDIA DriverKit Extension.
#
# This script simulates the final build process, compiling the driver skeleton
# and the shim layer, and linking them together. In a real-world scenario, this
# would also link against the compiled nvKms library.
#

# Ensure we are in the script's directory
cd "$(dirname "$0")"
set -e

echo "--- NVIDIA DriverKit Porting Kit Build ---"

# --- Configuration ---
CC="clang"
CXX="clang++"
AR="ar"

TARGET_DEXT="NVIDIADriver.kext/Contents/MacOS/NVIDIADriver"
TARGET_LIB="libnvkms.a"

# --- Compiler Flags ---
# These flags are for compiling the C++ components of our DriverKit extension.
# We include the necessary I/O Kit and DriverKit frameworks.
CXXFLAGS="-c -g -Wall -std=c++17 \
    -I. \
    -framework IOKit \
    -framework DriverKit"

# CFLAGS for the nvKms C code (replicated from the previous build script)
CFLAGS="-c -g -Wall -include ./nvport.h -include ./linux_stubs.h \
	-I. \
	-I../open-gpu-kernel-modules/src/nvidia/ \
	-I../open-gpu-kernel-modules/src/nvidia/inc/ \
	-I../open-gpu-kernel-modules/src/common/sdk/nvidia/inc/ \
	-I../open-gpu-kernel-modules/src/common/uproc/os/libos-v3.1.0/inc/ \
	-I../open-gpu-kernel-modules/src/nvidia/arch/nvalloc/unix/include/ \
	-I../open-gpu-kernel-modules/src/common/nvlink/in-band/inc/ \
	-I../open-gpu-kernel-modules/src/common/nvlink/kernel/nvlink/include/ \
	-I../open-gpu-kernel-modules/src/common/nvswitch/kernel/inc/ \
	-I../open-gpu-kernel-modules/src/nvidia/inc/libraries/ \
	-I../open-gpu-kernel-modules/src/nvidia/inc/kernel/"

# --- Build Steps ---

OBJ_DIR="obj"
mkdir -p "$OBJ_DIR"

#
# Step 1: Compile the nvKms library (simulated)
#
echo "[1/4] Compiling the OS-agnostic nvKms library (simulated)..."
# In a real build, you would uncomment the following lines to build libnvkms.a
# from the NVIDIA source code. This is currently disabled because it will produce
# thousands of errors that need to be resolved by creating more stubs.
#
# ./build.sh
#
# For now, we will create a dummy library from our shim object file to allow linking.
echo "    -> Creating dummy libnvkms.a for linking purposes."
$CXX $CXXFLAGS -o "$OBJ_DIR/nvkms_macos_shim.o" nvkms_macos_shim.cpp
$AR rcs "$TARGET_LIB" "$OBJ_DIR/nvkms_macos_shim.o"
echo "---"


#
# Step 2: Compile the DriverKit Extension C++ source
#
echo "[2/4] Compiling the NVIDIADriver DriverKit extension..."
$CXX $CXXFLAGS -o "$OBJ_DIR/NVIDIADriver.o" NVIDIADriver.cpp
echo "---"

#
# Step 3: Compile the Shim Layer C++ source
#
echo "[3/4] Compiling the macOS shim layer..."
# This was already compiled into the dummy library, but in a real build,
# you would compile it separately and link it.
# $CXX $CXXFLAGS -o "$OBJ_DIR/nvkms_macos_shim.o" nvkms_macos_shim.cpp
echo "    -> (Already compiled into the library)"
echo "---"


#
# Step 4: Link the final DriverKit Extension
#
echo "[4/4] Linking the final NVIDIADriver extension binary..."
mkdir -p "$(dirname "$TARGET_DEXT")"
$CXX -o "$TARGET_DEXT" \
    "$OBJ_DIR/NVIDIADriver.o" \
    "$OBJ_DIR/nvkms_macos_shim.o" \
    -L. -l"nvkms" \
    -framework IOKit -framework DriverKit

echo ""
echo "--- BUILD COMPLETE ---"
echo "The final (dummy) driver binary is located at: $TARGET_DEXT"
echo "NOTE: This is a skeleton and will not function. It demonstrates a successful"
echo "compilation and linking of the porting kit components."