#include "ServerDriver.h"
#include "../DriverLog/DriverLog.h"

vr::EVRInitError ServerDriver::Init(vr::IVRDriverContext *pDriverContext)
{
    VR_INIT_SERVER_DRIVER_CONTEXT(pDriverContext);
    InitDriverLog(vr::VRDriverLog());

    return vr::VRInitError_None;
}

void ServerDriver::Cleanup()
{
    CleanupDriverLog();

}

void ServerDriver::RunFrame()
{
}