#ifndef MACOS_COMPAT_ASM_BARRIER_H
#define MACOS_COMPAT_ASM_BARRIER_H

/*
 * This header provides a macOS-native implementation for the Linux kernel's
 * memory barrier functions, typically found in <asm/barrier.h>. It uses
 * compiler built-ins to generate the appropriate memory fence instructions.
 */

//
// mb() - full memory barrier
//
// Ensures that all preceding memory operations (reads and writes) are
// completed before any subsequent memory operations are started.
// We map this to the GCC/Clang full synchronization built-in.
//
#define mb()    __sync_synchronize()

//
// rmb() - read memory barrier
//
// Ensures that all preceding memory reads are completed before any
// subsequent memory reads are started. On most modern architectures with
// strong memory models (like x86), this is often a no-op for the compiler,
// but it's good practice to define it as a full barrier for compatibility.
//
#define rmb()   mb()

//
// wmb() - write memory barrier
//
// Ensures that all preceding memory writes are completed before any
// subsequent memory writes are started.
//
#define wmb()   mb()


#endif // MACOS_COMPAT_ASM_BARRIER_H