#ifndef CONTROLLERDRIVER_H
#define CONTROLLERDRIVER_H

#include <openvr_driver.h>
#include <string>

class ControllerDriver : public vr::ITrackedDeviceServerDriver
{
public:
    ControllerDriver(vr::ETrackedControllerRole controllerRole, std::string serialNumber);
    virtual ~ControllerDriver();

    virtual vr::EVRInitError Activate(vr::TrackedDeviceIndex_t unObjectId);
    virtual void Deactivate();
    virtual void EnterStandby();

    virtual void PowerOff();
    virtual void DebugRequest(const char *pchRequest, char *pchResponseBuffer, uint32_t unResponseBufferSize);
    virtual vr::DriverPose_t GetPose();

    void RunFrame();
    void ProcessEvent(const vr::VREvent_t &vrEvent);
    void *GetComponent(const char *pchComponentNameAndVersion);
    std::string GetSerialNumber() const { return m_serialNumber; }
private:
    vr::TrackedDeviceIndex_t m_controllerId;
    vr::PropertyContainerHandle_t m_containerHandle;
    vr::ETrackedControllerRole m_controllerRole;
    
    vr::DriverPose_t m_pose;
    std::string m_serialNumber;
};

#endif