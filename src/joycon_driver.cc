#include "joycon_driver.h"
#include "driverlog.h"
#include "jsl_glue.h"

JoyconDriver::JoyconDriver(
    vr::ETrackedControllerRole controllerRole,
    std::string serialNumber
) :
    m_controllerId(vr::k_unTrackedDeviceIndexInvalid),
    m_containerHandle(vr::k_ulInvalidPropertyContainer),
    m_controllerRole(controllerRole),
    m_serialNumber(serialNumber)
{
}

JoyconDriver::~JoyconDriver()
{
}

vr::EVRInitError JoyconDriver::Activate(vr::TrackedDeviceIndex_t unObjectId) 
{
    m_controllerId = unObjectId;
    m_containerHandle = vr::VRProperties()->TrackedDeviceToPropertyContainer(m_controllerId);

    vr::VRProperties()->SetStringProperty(m_containerHandle, vr::Prop_SerialNumber_String, m_serialNumber.c_str());
    vr::VRProperties()->SetStringProperty(m_containerHandle, vr::Prop_RenderModelName_String, "vr_controller_vive_1_5");

    vr::VRProperties()->SetBoolProperty(m_containerHandle, vr::Prop_NeverTracked_Bool, false);
	vr::VRProperties()->SetBoolProperty(m_containerHandle, vr::Prop_WillDriftInYaw_Bool, false);

    vr::VRProperties()->SetUint64Property(m_containerHandle, vr::Prop_CurrentUniverseId_Uint64, 2);
    vr::VRProperties()->SetInt32Property(m_containerHandle, vr::Prop_ControllerRoleHint_Int32, m_controllerRole);

    m_pose.poseIsValid = true;
    m_pose.result = vr::TrackingResult_Running_OK;
    m_pose.deviceIsConnected = true;

    m_pose.qRotation = { 1, 0, 0, 0 };
    m_pose.qWorldFromDriverRotation = { 1, 0, 0, 0 };
    m_pose.qDriverFromHeadRotation = { 1, 0, 0, 0 };

    m_pose.vecPosition[0] = m_controllerRole == vr::TrackedControllerRole_RightHand ? 0.25f : -0.25f;
    m_pose.vecPosition[1] = 1.2f;
    m_pose.vecPosition[2] = 0;

    return vr::VRInitError_None;
}

void JoyconDriver::Deactivate()
{
    m_controllerId = vr::k_unTrackedDeviceIndexInvalid;
}

void JoyconDriver::EnterStandby()
{
}

void JoyconDriver::PowerOff()
{
}

void JoyconDriver::DebugRequest(const char *pchRequest, char *pchResponseBuffer, uint32_t unResponseBufferSize)
{
    if (unResponseBufferSize >= 1)
        pchResponseBuffer[0] = 0;
}

vr::DriverPose_t JoyconDriver::GetPose()
{
    return m_pose;
}

void JoyconDriver::RunFrame()
{
    if (m_controllerId == vr::k_unTrackedDeviceIndexInvalid)
        return;

    vr::VRServerDriverHost()->TrackedDevicePoseUpdated(m_controllerId, GetPose(), sizeof(vr::DriverPose_t));
}

void JoyconDriver::ProcessEvent(const vr::VREvent_t &vrEvent)
{
}

void *JoyconDriver::GetComponent(const char *pchComponentNameAndVersion)
{
    return nullptr;
}