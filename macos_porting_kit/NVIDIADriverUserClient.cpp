/*
 * This file provides the implementation for the User Client class of the
 * NVIDIA DriverKit extension.
 */

#include "NVIDIADriverUserClient.h"
#include "NVIDIADriver.cpp" // Include the driver's implementation to get its class definition
#include <DriverKit/IOLib.h>

/*
 * init()
 */
bool NVIDIADriverUserClient::init()
{
    os_log(OS_LOG_DEFAULT, "NVIDIADriverUserClient: init()");
    if (!super::init()) {
        return false;
    }
    m_driver = NULL;
    return true;
}

/*
 * free()
 */
void NVIDIADriverUserClient::free()
{
    os_log(OS_LOG_DEFAULT, "NVIDIADriverUserClient: free()");
    super::free();
}

/*
 * Start() is called when a user-space client opens a connection to the driver.
 */
kern_return_t IMPL(NVIDIADriverUserClient, Start)
{
    os_log(OS_LOG_DEFAULT, "NVIDIADriverUserClient: Start()");

    kern_return_t ret = Start(provider, SUPERDISPATCH);
    if (ret != kIOReturnSuccess) {
        os_log(OS_LOG_DEFAULT, "NVIDIADriverUserClient: super::Start failed.");
        return ret;
    }

    // Get a handle to the main driver object.
    m_driver = OSDynamicCast(NVIDIADriver, provider);
    if (!m_driver) {
        os_log(OS_LOG_DEFAULT, "NVIDIADriverUserClient: Provider is not an NVIDIADriver.");
        return kIOReturnBadArgument;
    }

    // Retain the provider until Stop() is called. This keeps the driver
    // loaded while a client is connected.
    m_driver->retain();

    os_log(OS_LOG_DEFAULT, "NVIDIADriverUserClient: Successfully started and connected to driver.");

    return kIOReturnSuccess;
}

/*
 * Stop() is called when the user-space client closes its connection.
 */
kern_return_t IMPL(NVIDIADriverUserClient, Stop)
{
    os_log(OS_LOG_DEFAULT, "NVIDIADriverUserClient: Stop()");

    // Release the provider that we retained in Start().
    if (m_driver) {
        m_driver->release();
        m_driver = NULL;
    }

    return Stop(provider, SUPERDISPATCH);
}