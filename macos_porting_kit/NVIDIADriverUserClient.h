#ifndef NVIDIA_DRIVER_USER_CLIENT_H
#define NVIDIA_DRIVER_USER_CLIENT_H

/*
 * This header defines the User Client class for the NVIDIA DriverKit extension.
 * A User Client is the standard mechanism for a user-space application to
 * open a connection to and communicate with a driver.
 */

#include <DriverKit/IOUserClient.h>

class NVIDIADriver; // Forward declaration

class NVIDIADriverUserClient : public IOUserClient
{
public:
    virtual bool init() override;
    virtual kern_return_t Start(IOService* provider) override;
    virtual kern_return_t Stop(IOService* provider) override;
    virtual void free() override;

private:
    NVIDIADriver* m_driver;
};

#endif // NVIDIA_DRIVER_USER_CLIENT_H