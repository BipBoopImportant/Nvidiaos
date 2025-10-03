#ifndef MACOS_COMPAT_LINUX_DELAY_H
#define MACOS_COMPAT_LINUX_DELAY_H

#include <DriverKit/IOLib.h>

/*
 * This header provides a macOS-native implementation for the Linux kernel's
 * delay and sleep functions, typically found in <linux/delay.h>.
 */


//
// udelay - Busy-waits for a specified number of microseconds.
//
// This is used for very short delays where sleeping is not appropriate.
// It maps directly to IODelay.
//
static inline void udelay(unsigned long usecs)
{
    IODelay(usecs);
}

//
// mdelay - Busy-waits for a specified number of milliseconds.
//
static inline void mdelay(unsigned long msecs)
{
    IODelay(msecs * 1000);
}

//
// msleep - Sleeps for a specified number of milliseconds.
//
// This is an interruptible sleep. It maps to IOSleep.
//
static inline void msleep(unsigned int msecs)
{
    // IOSleep takes microseconds
    IOSleep(msecs * 1000);
}

#endif // MACOS_COMPAT_LINUX_DELAY_H