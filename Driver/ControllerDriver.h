#ifndef CONTROLLERDRIVER_H
#define CONTROLLERDRIVER_H

#include <openvr_driver.h>
#include <string>

class ControllerDriver : public vr::ITrackedDeviceServerDriver
{
public:
    ControllerDriver(vr::ETrackedControllerRole controllerRole, std::string serialNumber, int jslHandle);
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

    vr::VRInputComponentHandle_t m_triggerValueHandle;
    vr::VRInputComponentHandle_t m_triggerPressedHandle;

    vr::VRInputComponentHandle_t m_homeButtonHandle;

    vr::VRInputComponentHandle_t m_joystickPressedHandle;
    vr::VRInputComponentHandle_t m_joystickAxisXHandle;
    vr::VRInputComponentHandle_t m_joystickAxisYHandle;

    vr::VRInputComponentHandle_t m_buttonAHandle;
    vr::VRInputComponentHandle_t m_buttonBHandle;
    vr::VRInputComponentHandle_t m_buttonXHandle;
    vr::VRInputComponentHandle_t m_buttonYHandle;

    vr::VRInputComponentHandle_t m_triggerHapticHandle;

    vr::HmdQuaternion_t m_calibrationOffset = { 1, 0, 0, 0 };
    
    vr::DriverPose_t m_pose;
    std::string m_serialNumber;

    int m_jslHandle;
};

#endif