#ifndef MACOS_COMPAT_LINUX_ATOMIC_H
#define MACOS_COMPAT_LINUX_ATOMIC_H

/*
 * This header provides a macOS-native implementation for the Linux kernel's
 * atomic integer operations. It uses the compiler's built-in atomic functions
 * (which are standard in modern C/C++) to map the Linux API to efficient,
 * hardware-level atomic instructions.
 */

// Define atomic_t as a volatile integer. The volatility ensures the compiler
// doesn't optimize away reads/writes. The atomicity is handled by the
// functions that operate on it.
typedef struct {
    volatile int counter;
} atomic_t;

//
// atomic_set - atomically sets the value of an atomic_t
//
static inline void atomic_set(atomic_t *v, int i)
{
    // Use a built-in for a memory barrier, ensuring the write is visible.
    __sync_lock_test_and_set(&v->counter, i);
}

//
// atomic_read - atomically reads the value of an atomic_t
//
static inline int atomic_read(const atomic_t *v)
{
    // A simple volatile read is sufficient on most modern architectures
    // for atomicity of a single aligned integer.
    return v->counter;
}

//
// atomic_inc - atomically increments an atomic_t
//
static inline void atomic_inc(atomic_t *v)
{
    // Use the built-in fetch-and-add primitive.
    __sync_fetch_and_add(&v->counter, 1);
}

//
// atomic_dec - atomically decrements an atomic_t
//
static inline void atomic_dec(atomic_t *v)
{
    // Use the built-in fetch-and-subtract primitive.
    __sync_fetch_and_sub(&v->counter, 1);
}

//
// atomic_add - atomically adds a value to an atomic_t
//
static inline void atomic_add(int i, atomic_t *v)
{
    __sync_fetch_and_add(&v->counter, i);
}

//
// atomic_sub - atomically subtracts a value from an atomic_t
//
static inline void atomic_sub(int i, atomic_t *v)
{
    __sync_fetch_and_sub(&v->counter, i);
}

//
// atomic_inc_and_test - atomically increments and checks if the new value is zero
//
static inline int atomic_inc_and_test(atomic_t *v)
{
    return __sync_add_and_fetch(&v->counter, 1) == 0;
}

//
// atomic_dec_and_test - atomically decrements and checks if the new value is zero
//
static inline int atomic_dec_and_test(atomic_t *v)
{
    return __sync_sub_and_fetch(&v->counter, 1) == 0;
}

//
// atomic_sub_and_test - atomically subtracts and checks if the new value is zero
//
static inline int atomic_sub_and_test(int i, atomic_t *v)
{
    return __sync_sub_and_fetch(&v->counter, i) == 0;
}

#endif // MACOS_COMPAT_LINUX_ATOMIC_H