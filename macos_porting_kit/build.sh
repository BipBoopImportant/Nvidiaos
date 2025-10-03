#!/bin/bash

# Change to the script's own directory to ensure relative paths work correctly.
cd "$(dirname "$0")"

#
# build.sh - A script to compile the NVIDIA OS-agnostic 'nvKms' core
# into a static library on macOS.
#
# This script replaces the Makefile to bypass environment issues with 'make'.
# It iterates through all source files and compiles them individually.
#

set -e # Exit immediately if a command exits with a non-zero status.

# Compiler and Archiver
CC="clang"
AR="ar"

# Build Target
TARGET="libnvkms.a"

# Compiler Flags
# Note: All paths are relative to the script's location (macos_porting_kit).
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
	-I../open-gpu-kernel-modules/src/nvidia/inc/libraries/" \
	-I../open-gpu-kernel-modules/src/nvidia/inc/kernel/

# Source Files
# This list is derived from open-gpu-kernel-modules/src/nvidia/srcs.mk
SRCS=(
	"../open-gpu-kernel-modules/src/nvidia/generated/g_access_cntr_buffer_nvoc.c"
	"../open-gpu-kernel-modules/src/nvidia/arch/nvalloc/unix/src/os.c"
	# Add other source files here... for now, we'll just test these two.
)

# Object files will be placed in an 'obj' directory
OBJ_DIR="obj"
mkdir -p "$OBJ_DIR"

OBJS=()

echo "Starting compilation..."

for src_file in "${SRCS[@]}"; do
    base_name=$(basename "$src_file" .c)
    obj_file="$OBJ_DIR/$base_name.o"
    echo "Compiling $src_file -> $obj_file"

    # Compile the source file
    $CC $CFLAGS -o "$obj_file" "$src_file"

    OBJS+=("$obj_file")
done

echo "Archiving object files into $TARGET..."

# Create the static library
$AR rcs "$TARGET" "${OBJS[@]}"

echo "Build script finished."