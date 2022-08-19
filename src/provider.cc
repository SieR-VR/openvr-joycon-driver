#include "provider.h"
#include "driverlog.h"
#include "jsl_glue.h"

vr::EVRInitError Provider::Init(vr::IVRDriverContext *pDriverContext)
{
    VR_INIT_SERVER_DRIVER_CONTEXT(pDriverContext);
    
    // Init the driver log
    InitDriverLog(vr::VRDriverLog());

    DriverLog("Provider::Init - Initializing JoyShockLibrary");
    JSLGlue::instance.init();

    DriverLog("Provider::Init - Initializing driver");
    
    vr::EVRInitError eError = vr::VRInitError_None;

    m_LeftController = new JoyconDriver(vr::TrackedControllerRole_LeftHand, "joycon_left");
    m_RightController = new JoyconDriver(vr::TrackedControllerRole_RightHand, "joycon_right");

    vr::VRServerDriverHost()->TrackedDeviceAdded("joycon_left", vr::TrackedDeviceClass_Controller, m_LeftController);
    vr::VRServerDriverHost()->TrackedDeviceAdded("joycon_right", vr::TrackedDeviceClass_Controller, m_RightController);

    return eError;
}

void Provider::Cleanup()
{
    CleanupDriverLog();
    VR_CLEANUP_SERVER_DRIVER_CONTEXT();
}

void Provider::RunFrame()
{
    if (m_bIsFirstRunFrame)
    {
        m_bIsFirstRunFrame = false;
    }
}