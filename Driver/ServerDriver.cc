#include "ServerDriver.h"
#include "../DriverLog/DriverLog.h"

#include <JSL/JoyShockLibrary.h>

vr::EVRInitError ServerDriver::Init(vr::IVRDriverContext *pDriverContext)
{
    VR_INIT_SERVER_DRIVER_CONTEXT(pDriverContext);
    InitDriverLog(vr::VRDriverLog());

    numberOfJslControllers = JslConnectDevices();
    JslDeviceHandles = new int[numberOfJslControllers];

    int getHandleResult = JslGetConnectedDeviceHandles(JslDeviceHandles, numberOfJslControllers);

    for(int i = 0; i < numberOfJslControllers; i++)
    {
        switch (JslGetControllerType(JslDeviceHandles[i]))
        {
            case JS_TYPE_JOYCON_LEFT:
            {
                m_LeftController = new ControllerDriver(vr::TrackedControllerRole_LeftHand, "Left_JoyCon", JslDeviceHandles[i]);
                DriverLog("Succesfully created Left Controller Driver");
                break;
            }
            case JS_TYPE_JOYCON_RIGHT:
            {
                m_RightController = new ControllerDriver(vr::TrackedControllerRole_RightHand, "Right_JoyCon", JslDeviceHandles[i]);
                DriverLog("Succesfully created Right Controller Driver");
                break;
            }
            default:
            {
                DriverLog("Skipping Other Controller: %d", JslGetControllerType(JslDeviceHandles[i]));
                break;
            }
        }
    }

    if(m_LeftController == NULL || m_RightController == NULL)
    {
        DriverLog("Failed to create Controller Driver");
        return vr::VRInitError_Driver_Failed;
    }

    vr::VRServerDriverHost()->TrackedDeviceAdded("Left_JoyCon", vr::TrackedDeviceClass_Controller, m_LeftController);
    vr::VRServerDriverHost()->TrackedDeviceAdded("Right_JoyCon", vr::TrackedDeviceClass_Controller, m_RightController);
    return vr::VRInitError_None;
}

void ServerDriver::Cleanup()
{
    CleanupDriverLog();

    delete m_LeftController;
    m_LeftController = NULL;
}

void ServerDriver::RunFrame()
{
    if(m_LeftController)
        m_LeftController->RunFrame();
    
    if(m_RightController)
        m_RightController->RunFrame();
}