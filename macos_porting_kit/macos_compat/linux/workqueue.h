#ifndef MACOS_COMPAT_LINUX_WORKQUEUE_H
#define MACOS_COMPAT_LINUX_WORKQUEUE_H

#include <DriverKit/IOLib.h>
#include <DriverKit/IODispatchQueue.h>

/*
 * This header provides a macOS-native implementation for the Linux kernel's
 * work queue mechanism. It maps the Linux work queue API to IODispatchQueue,
 * the standard asynchronous work facility in Apple's DriverKit framework.
 */

// Forward declaration for the work_struct
struct work_struct;

// Define the work function pointer type used by Linux
typedef void (*work_func_t)(struct work_struct *work);

//
// work_struct - The basic Linux work item.
//
// Our native implementation will store the work function and its argument.
// We will also need a global dispatch queue to submit the work to.
//
struct work_struct {
    work_func_t func;
    // In a more complete implementation, you might store data here.
    // For now, the function pointer is the key part.
};

// A global dispatch queue to serve as the system-wide workqueue.
static IODispatchQueue* gSystemWorkQueue = NULL;

// Helper to initialize the global work queue once.
static inline void ensure_system_workqueue_initialized() {
    if (gSystemWorkQueue == NULL) {
        // Create a serial dispatch queue.
        gSystemWorkQueue = IODispatchQueue::create("com.nvidia.system_workqueue");
    }
}

//
// INIT_WORK - Initializes a work_struct with a given function.
//
#define INIT_WORK(work, fn) \
do { \
    (work)->func = (fn); \
} while (0)


//
// schedule_work - Queues a work item to be executed.
//
// This maps to dispatching a block onto our global IODispatchQueue.
//
static inline bool schedule_work(struct work_struct *work)
{
    ensure_system_workqueue_initialized();
    if (!gSystemWorkQueue || !work || !work->func) {
        return false;
    }

    // Capture the work struct by value in the dispatched block.
    struct work_struct captured_work = *work;

    gSystemWorkQueue->dispatchAsync(^{
        captured_work.func(&captured_work);
    });

    return true;
}

//
// flush_scheduled_work - Waits for all currently scheduled work to complete.
//
// This can be mapped to dispatching a synchronous, empty block to the queue.
// When the sync block returns, all previously async blocks are guaranteed
// to have finished.
//
static inline void flush_scheduled_work(void)
{
    if (gSystemWorkQueue) {
        gSystemWorkQueue->dispatchSync(^{
            // This block does nothing. Its purpose is to act as a barrier.
        });
    }
}


#endif // MACOS_COMPAT_LINUX_WORKQUEUE_H