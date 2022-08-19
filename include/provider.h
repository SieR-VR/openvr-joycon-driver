#ifndef _PROVIDER_H_
#define _PROVIDER_H_

#include <openvr_driver.h>

#include "joycon_driver.h"

class Provider : public vr::IServerTrackedDeviceProvider
{
public:
    virtual vr::EVRInitError Init(vr::IVRDriverContext *pDriverContext);
    virtual void Cleanup();
    virtual const char * const *GetInterfaceVersions() { return vr::k_InterfaceVersions; }
    virtual void RunFrame();
    virtual bool ShouldBlockStandbyMode() { return false; }
    virtual void EnterStandby() {}
    virtual void LeaveStandby() {}

    JoyconDriver *m_LeftController;
    JoyconDriver *m_RightController;

private:
    bool m_bIsFirstRunFrame = true;
};

#endif