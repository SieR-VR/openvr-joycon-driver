#include "ControllerDriver.h"
#include "../DriverLog/DriverLog.h"
#include "../Tools/QuaternionHelper.h"

#include <JSL/JoyShockLibrary.h>

ControllerDriver::ControllerDriver(vr::ETrackedControllerRole controllerRole, std::string serialNumber, int jslHandle)
{
    m_controllerId = vr::k_unTrackedDeviceIndexInvalid;
    m_containerHandle = vr::k_ulInvalidPropertyContainer;

    m_controllerRole = controllerRole;
    m_serialNumber = serialNumber;

    m_jslHandle = jslHandle;
}

ControllerDriver::~ControllerDriver()
{
}

vr::EVRInitError ControllerDriver::Activate(vr::TrackedDeviceIndex_t unObjectId)
{
    m_controllerId = unObjectId;
    m_containerHandle = vr::VRProperties()->TrackedDeviceToPropertyContainer(m_controllerId);

    vr::VRProperties()->SetStringProperty(m_containerHandle, vr::Prop_SerialNumber_String, m_serialNumber.c_str());
    vr::VRProperties()->SetStringProperty(m_containerHandle, vr::Prop_RenderModelName_String, "vr_controller_vive_1_5");

    vr::VRProperties()->SetBoolProperty(m_containerHandle, vr::Prop_NeverTracked_Bool, false);
	vr::VRProperties()->SetBoolProperty(m_containerHandle, vr::Prop_WillDriftInYaw_Bool, false);

    vr::VRProperties()->SetUint64Property(m_containerHandle, vr::Prop_CurrentUniverseId_Uint64, 2);

    vr::VRProperties()->SetInt32Property(m_containerHandle, vr::Prop_ControllerRoleHint_Int32, m_controllerRole);
    vr::VRProperties()->SetStringProperty(m_containerHandle, vr::Prop_InputProfilePath_String, "{JoyCon}/input/JoyCon_profile.json");

    vr::VRDriverInput()->CreateScalarComponent(m_containerHandle, "/input/trigger/value", &m_triggerValueHandle, vr::EVRScalarType::VRScalarType_Absolute, vr::EVRScalarUnits::VRScalarUnits_NormalizedOneSided);
    vr::VRDriverInput()->CreateBooleanComponent(m_containerHandle, "/input/trigger/click", &m_triggerPressedHandle);

    vr::VRDriverInput()->CreateBooleanComponent(m_containerHandle, "/input/system/click", &m_homeButtonHandle);

    vr::VRDriverInput()->CreateBooleanComponent(m_containerHandle, "/input/thumbstick/click", &m_joystickPressedHandle);
    vr::VRDriverInput()->CreateScalarComponent(m_containerHandle, "/input/thumbstick/x", &m_joystickAxisXHandle, vr::EVRScalarType::VRScalarType_Absolute, vr::EVRScalarUnits::VRScalarUnits_NormalizedTwoSided);
    vr::VRDriverInput()->CreateScalarComponent(m_containerHandle, "/input/thumbstick/y", &m_joystickAxisYHandle, vr::EVRScalarType::VRScalarType_Absolute, vr::EVRScalarUnits::VRScalarUnits_NormalizedTwoSided);

    vr::VRDriverInput()->CreateBooleanComponent(m_containerHandle, "/input/a/click", &m_buttonAHandle);
    vr::VRDriverInput()->CreateBooleanComponent(m_containerHandle, "/input/b/click", &m_buttonBHandle);
    vr::VRDriverInput()->CreateBooleanComponent(m_containerHandle, "/input/x/click", &m_buttonXHandle);
    vr::VRDriverInput()->CreateBooleanComponent(m_containerHandle, "/input/y/click", &m_buttonYHandle);

    vr::VRDriverInput()->CreateHapticComponent(m_containerHandle, "/output/haptic", &m_triggerHapticHandle);

    m_pose.poseIsValid = true;
    m_pose.result = vr::TrackingResult_Running_OK;
    m_pose.deviceIsConnected = true;

    m_pose.qRotation = QuaternionHelper::HmdQuaternionInit(1, 0, 0, 0);
    m_pose.qWorldFromDriverRotation = QuaternionHelper::HmdQuaternionInit(1, 0, 0, 0);
    m_pose.qDriverFromHeadRotation = QuaternionHelper::HmdQuaternionInit(1, 0, 0, 0);

    m_pose.vecPosition[0] = m_controllerRole == vr::TrackedControllerRole_RightHand ? 0.25f : -0.25f;
    m_pose.vecPosition[1] = 1.2f;
    m_pose.vecPosition[2] = 0;

    return vr::VRInitError_None;
}

void ControllerDriver::Deactivate()
{
    m_controllerId = vr::k_unTrackedDeviceIndexInvalid;
}

void ControllerDriver::EnterStandby()
{
}

void ControllerDriver::PowerOff()
{
}

void ControllerDriver::DebugRequest(const char *pchRequest, char *pchResponseBuffer, uint32_t unResponseBufferSize)
{
    if (unResponseBufferSize >= 1)
        pchResponseBuffer[0] = 0;
}

void ControllerDriver::ProcessEvent(const vr::VREvent_t &event)
{
    switch (event.eventType)
    {
        case vr::VREvent_Input_HapticVibration:
        {
            if(event.data.hapticVibration.componentHandle == m_triggerHapticHandle)
            {
                int hapticValue = (int) event.data.hapticVibration.fAmplitude * 255;
                JslSetRumble(m_jslHandle, hapticValue, hapticValue);
            }
        }
    }
}

void *ControllerDriver::GetComponent(const char *pchComponentNameAndVersion)
{
    return nullptr;
}

vr::DriverPose_t ControllerDriver::GetPose()
{
    static int i = 0;
    MOTION_STATE motionState = JslGetMotionState(m_jslHandle);

    JOY_SHOCK_STATE joyState = JslGetSimpleState(m_jslHandle);
    bool triggerValue = (joyState.buttons & ((m_controllerRole == vr::TrackedControllerRole_LeftHand) ? 0x00400 : 0x00800)) != 0;

    vr::VRDriverInput()->UpdateScalarComponent(m_triggerValueHandle, (float) triggerValue, 0.0f);
    vr::VRDriverInput()->UpdateBooleanComponent(m_triggerPressedHandle, triggerValue, 0.0f);

    bool homeButtonState = (joyState.buttons & ((m_controllerRole == vr::TrackedControllerRole_LeftHand) ? 0x20000 : 0x10000)) != 0;
    vr::VRDriverInput()->UpdateBooleanComponent(m_homeButtonHandle, homeButtonState, 0.0f);
        
    bool isJoyStickPressed = (joyState.buttons & ((m_controllerRole == vr::TrackedControllerRole_LeftHand) ? 0x00040 : 0x00080)) != 0;
    vr::VRDriverInput()->UpdateBooleanComponent(m_joystickPressedHandle, isJoyStickPressed, 0.0f);

    bool isButtonAPressed = (joyState.buttons & ((m_controllerRole == vr::TrackedControllerRole_LeftHand) ? 0x00008 : 0x02000)) != 0;
    vr::VRDriverInput()->UpdateBooleanComponent(m_buttonAHandle, isButtonAPressed, 0.0f);

    bool isButtonBPressed = (joyState.buttons & ((m_controllerRole == vr::TrackedControllerRole_LeftHand) ? 0x00002 : 0x01000)) != 0;
    vr::VRDriverInput()->UpdateBooleanComponent(m_buttonBHandle, isButtonBPressed, 0.0f);

    bool isButtonXPressed = (joyState.buttons & ((m_controllerRole == vr::TrackedControllerRole_LeftHand) ? 0x00001 : 0x08000)) != 0;
    vr::VRDriverInput()->UpdateBooleanComponent(m_buttonXHandle, isButtonXPressed, 0.0f); 

    bool isButtonYPressed = (joyState.buttons & ((m_controllerRole == vr::TrackedControllerRole_LeftHand) ? 0x00004 : 0x04000)) != 0;
    vr::VRDriverInput()->UpdateBooleanComponent(m_buttonYHandle, isButtonYPressed, 0.0f);

    vr::VRDriverInput()->UpdateScalarComponent(m_joystickAxisXHandle, m_controllerRole == vr::TrackedControllerRole_LeftHand ? joyState.stickLX : joyState.stickRX, 0.0f);
    vr::VRDriverInput()->UpdateScalarComponent(m_joystickAxisYHandle, m_controllerRole == vr::TrackedControllerRole_LeftHand ? joyState.stickLY : joyState.stickRY, 0.0f);


    if(isJoyStickPressed) m_calibrationOffset ={motionState.quatW, -motionState.quatX, -motionState.quatY, -motionState.quatZ};

    m_pose.qRotation = QuaternionHelper::MultipleQuaternion(m_calibrationOffset, {motionState.quatW, motionState.quatX, motionState.quatY, motionState.quatZ});

    return m_pose;
}

void ControllerDriver::RunFrame()
{
    if (m_controllerId != vr::k_unTrackedDeviceIndexInvalid)
    {
        vr::VRServerDriverHost()->TrackedDevicePoseUpdated(m_controllerId, GetPose(), sizeof(vr::DriverPose_t));
    }
}