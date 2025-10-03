/*
 * This is a user-space test client application that demonstrates how to find
 * and connect to the NVIDIADriverUserClient.
 */

#include <iostream>
#include <IOKit/IOKitLib.h>

int main(int argc, const char * argv[]) {
    kern_return_t kr;
    io_service_t service;
    io_connect_t connection;

    std::cout << "--- NVIDIA Driver Test Client ---" << std::endl;

    // Create a matching dictionary to find our driver service.
    // This looks for a service that has the "IOProviderClass" property set to
    // "IOPCIDevice" and is named "NVIDIADriver" (our IOClass).
    CFMutableDictionaryRef matchingDict = IOServiceMatching("NVIDIADriver");
    if (!matchingDict) {
        std::cerr << "Failed to create matching dictionary." << std::endl;
        return -1;
    }

    std::cout << "Searching for NVIDIADriver service..." << std::endl;

    // IOServiceGetMatchingService will wait for a matching service to be published.
    // In a real system, this would happen when the user plugs in the hardware.
    // For our test, the dext would need to be loaded first.
    service = IOServiceGetMatchingService(kIOMasterPortDefault, matchingDict);

    if (service == IO_OBJECT_NULL) {
        std::cerr << "NVIDIADriver service not found." << std::endl;
        return -1;
    }

    std::cout << "Service found. Opening connection to user client..." << std::endl;

    // Open a connection to the user client. This will trigger the Start()
    // method in our NVIDIADriverUserClient class.
    kr = IOServiceOpen(service, mach_task_self(), 0, &connection);

    // We no longer need the service object.
    IOObjectRelease(service);

    if (kr != KERN_SUCCESS) {
        std::cerr << "Failed to open user client connection. Error: " << kr << std::endl;
        return -1;
    }

    std::cout << "Successfully opened connection to user client." << std::endl;

    // In a real client, you would now use IOConnectCallMethod to communicate
    // with the driver.

    // Close the connection. This will trigger the Stop() method in our
    // NVIDIADriverUserClient class.
    kr = IOServiceClose(connection);
    if (kr != KERN_SUCCESS) {
        std::cerr << "Failed to close user client connection. Error: " << kr << std::endl;
    }

    std::cout << "Connection closed." << std::endl;
    std::cout << "Test complete." << std::endl;

    return 0;
}