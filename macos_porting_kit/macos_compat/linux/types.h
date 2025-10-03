#ifndef MACOS_COMPAT_LINUX_TYPES_H
#define MACOS_COMPAT_LINUX_TYPES_H

#include <stdint.h>
#include <stdbool.h>

/*
 * This header provides definitions for the standard integer types used
 * throughout the Linux kernel source code.
 */

typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;

typedef int8_t    s8;
typedef int16_t   s16;
typedef int32_t   s32;
typedef int64_t   s64;

// The Linux kernel has its own bool type, but we can map it to the C99/C++ standard.
// Note: In older kernels, 'bool' might be a typedef for an int, so careful
// function signature matching may be required.

#ifndef __cplusplus
#ifndef bool
// Don't redefine if it's already defined (e.g. by stdbool.h)
#endif
#endif

#endif // MACOS_COMPAT_LINUX_TYPES_H