#include <openvr_driver.h>
#include <string.h>

#include "provider.h"

#define HMD_DLL_EXPORT extern "C" __declspec(dllexport)
#define HMD_DLL_IMPORT extern "C" __declspec(dllimport)

Provider g_provider;

HMD_DLL_EXPORT void *HmdDriverFactory(const char *pInterfaceName, int *pReturnCode)
{
    if (strcmp(pInterfaceName, vr::IServerTrackedDeviceProvider_Version) == 0)
    {
        return &g_provider;
    }

    if (pReturnCode)
        *pReturnCode = vr::VRInitError_Init_InterfaceNotFound;

    return nullptr;
}