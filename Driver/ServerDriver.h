#ifndef SERVERDRIVER_H
#define SERVERDRIVER_H

#include <openvr_driver.h>

#include "ControllerDriver.h"

class ServerDriver : public vr::IServerTrackedDeviceProvider
{
public:
    virtual vr::EVRInitError Init(vr::IVRDriverContext *pDriverContext);
    virtual void Cleanup();
    virtual const char * const *GetInterfaceVersions() { return vr::k_InterfaceVersions; }
    virtual void RunFrame();
    virtual bool ShouldBlockStandbyMode() { return false; }
    virtual void EnterStandby() {}
    virtual void LeaveStandby() {}
private:
    int numberOfJslControllers;
    int *JslDeviceHandles;

    ControllerDriver *m_LeftController;
    ControllerDriver *m_RightController;
};

#endif