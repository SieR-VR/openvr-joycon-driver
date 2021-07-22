#include <openvr_driver.h>
#include <string.h>

#include "ServerDriver.h"

#define HMD_DLL_EXPORT extern "C" __attribute__((visibility("default")))
#define HMD_DLL_IMPORT extern "C"

ServerDriver g_serverDriverNull;

HMD_DLL_EXPORT void *HmdDriverFactory(const char *pInterfaceName, int *pReturnCode)
{
    if (strcmp(pInterfaceName, vr::IServerTrackedDeviceProvider_Version) == 0)
    {
        return &g_serverDriverNull;
    }

    if(pReturnCode)
        *pReturnCode = vr::VRInitError_Init_InterfaceNotFound;

    return nullptr;
}
