#ifndef MACOS_COMPAT_LINUX_TIMER_H
#define MACOS_COMPAT_LINUX_TIMER_H

#include <DriverKit/IOLib.h>
#include <DriverKit/IOTimerDispatchSource.h>

/*
 * This header provides a macOS-native implementation for the Linux kernel's
 * timer API. It maps the Linux timer API to IOTimerDispatchSource from
 * Apple's DriverKit framework.
 */

// Forward declaration
struct timer_list;

// Define the timer callback function pointer type used by Linux
typedef void (*timer_callback_t)(struct timer_list *t);


//
// timer_list - The Linux kernel timer structure.
//
// Our native implementation will wrap an IOTimerDispatchSource object,
// along with the callback function and its context.
//
struct timer_list {
    IOTimerDispatchSource*  timer;
    timer_callback_t        function;
    unsigned long           data; // 'data' is the argument to the callback in Linux
};


//
// init_timer - Initializes a timer.
//
static inline void init_timer(struct timer_list *timer)
{
    // We need a dispatch queue to run the timer on. For simplicity, we'll
    // use a single global queue for all timers. A real driver might use
    // per-device queues.
    static IODispatchQueue* gTimerQueue = NULL;
    if (gTimerQueue == NULL) {
        gTimerQueue = IODispatchQueue::create("com.nvidia.timer_queue");
    }

    timer->timer = IOTimerDispatchSource::create(gTimerQueue, ^{
        // When the timer fires, call the Linux-style callback function.
        if (timer->function) {
            timer->function(timer);
        }
    });
}


//
// add_timer - Starts or modifies a timer.
//
// Linux's add_timer takes an 'expires' value in jiffies. We will assume
// for this compatibility layer that the caller has converted this to
// nanoseconds. A more complete implementation would handle jiffies conversion.
//
static inline void add_timer(struct timer_list *timer, unsigned long expires_ns)
{
    if (timer && timer->timer) {
        // Set the timer to fire once after the specified interval.
        timer->timer->setTimeout(expires_ns, /* one-shot timer */ 0);
    }
}


//
// del_timer_sync - Deactivates a timer and waits for it to finish executing.
//
// This maps to canceling the IOTimerDispatchSource.
//
static inline int del_timer_sync(struct timer_list *timer)
{
    if (timer && timer->timer) {
        timer->timer->cancel();
        return 1; // Return 1 if the timer was pending
    }
    return 0;
}


//
// destroy_timer (non-standard, but good practice for our implementation)
//
// Frees the allocated IOTimerDispatchSource.
//
static inline void destroy_timer(struct timer_list *timer)
{
    if (timer && timer->timer) {
        timer->timer->release();
        timer->timer = NULL;
    }
}


#endif // MACOS_COMPAT_LINUX_TIMER_H