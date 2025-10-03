#!/bin/bash

#
# build_dext.sh - A comprehensive script to build the NVIDIA DriverKit Extension
# and its user-space test client.
#

# Ensure we are in the script's directory
cd "$(dirname "$0")"
set -e

echo "--- NVIDIA Driver Full Stack Build ---"

# --- Configuration ---
CXX="clang++"
TARGET_DEXT="NVIDIADriver.kext/Contents/MacOS/NVIDIADriver"
TEST_CLIENT="test_client"

# --- Compiler Flags ---
# Flags for compiling the DriverKit Extension
DEXT_CXXFLAGS="-g -Wall -std=c++17 -fno-exceptions -fno-rtti \
    -I. \
    -framework IOKit \
    -framework DriverKit"

# Flags for compiling the user-space test client
CLIENT_CXXFLAGS="-g -Wall -std=c++17 \
    -framework IOKit \
    -framework CoreFoundation"


# --- Build Steps ---
OBJ_DIR="obj"
rm -rf "$OBJ_DIR"
mkdir -p "$OBJ_DIR"
mkdir -p "$(dirname "$TARGET_DEXT")"


#
# Step 1: Compile the DriverKit Extension
#
echo "[1/2] Compiling the NVIDIADriver DriverKit extension..."

# Note: NVIDIADriverUserClient.cpp includes NVIDIADriver.cpp, so we only need
# to compile the UserClient file to get both implementation parts.
$CXX $DEXT_CXXFLAGS -o "$TARGET_DEXT" NVIDIADriverUserClient.cpp

echo "    -> Driver binary created at $TARGET_DEXT"
echo "---"


#
# Step 2: Compile the User-Space Test Client
#
echo "[2/2] Compiling the user-space test client..."
$CXX $CLIENT_CXXFLAGS -o "$TEST_CLIENT" test_client.cpp

echo "    -> Test client created at $TEST_CLIENT"
echo "---"


echo ""
echo "--- BUILD COMPLETE ---"
echo "The driver and test client have been successfully compiled."
echo "NOTE: The driver is functional in a simulated context and will not control real hardware."