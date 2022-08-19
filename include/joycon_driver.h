#ifndef _JOYCON_DRIVER_H_
#define _JOYCON_DRIVER_H_

#include <openvr_driver.h>
#include <string>

struct InputManager
{
    vr::VRInputComponentHandle_t up, down, left, right;
    vr::VRInputComponentHandle_t plus, minus;
    vr::VRInputComponentHandle_t lstick_click, rstick_click;
    vr::VRInputComponentHandle_t l, r, zl, zr, sl, sr;
    vr::VRInputComponentHandle_t a, b, x, y;
    vr::VRInputComponentHandle_t home, capture;

    vr::VRInputComponentHandle_t lstick_x, lstick_y;
    vr::VRInputComponentHandle_t rstick_x, rstick_y;

    InputManager(vr::PropertyContainerHandle_t container, vr::ETrackedControllerRole role);
    void registerCallback(vr::ETrackedControllerRole role);
};

class JoyconDriver : public vr::ITrackedDeviceServerDriver
{
public:
    JoyconDriver(vr::ETrackedControllerRole controllerRole, std::string serialNumber);
    virtual ~JoyconDriver();

    virtual vr::EVRInitError Activate(vr::TrackedDeviceIndex_t unObjectId);
    virtual void Deactivate();
    virtual void EnterStandby();

    virtual void PowerOff();
    virtual void DebugRequest(const char *pchRequest, char *pchResponseBuffer, uint32_t unResponseBufferSize);
    virtual vr::DriverPose_t GetPose();

    void RegisterCallback();

    void RunFrame();
    void ProcessEvent(const vr::VREvent_t &vrEvent);
    void *GetComponent(const char *pchComponentNameAndVersion);
    std::string GetSerialNumber() const { return m_serialNumber; }

    InputManager *m_inputManager;

private:
    vr::TrackedDeviceIndex_t m_controllerId;
    vr::PropertyContainerHandle_t m_containerHandle;
    vr::ETrackedControllerRole m_controllerRole;

    vr::DriverPose_t m_pose;
    std::string m_serialNumber;

    int m_jslHandle;
};

#endif