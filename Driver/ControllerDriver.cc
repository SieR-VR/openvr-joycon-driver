#include "ControllerDriver.h"
#include "../DriverLog/DriverLog.h"
#include "../Tools/QuaternionHelper.h"

#include <windows.h>

ControllerDriver::ControllerDriver(vr::ETrackedControllerRole controllerRole, std::string serialNumber)
{
    m_controllerId = vr::k_unTrackedDeviceIndexInvalid;
    m_containerHandle = vr::k_ulInvalidPropertyContainer;

    m_controllerRole = controllerRole;
    m_serialNumber = serialNumber;
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

    m_pose.poseIsValid = true;
    m_pose.result = vr::TrackingResult_Running_OK;
    m_pose.deviceIsConnected = true;

    m_pose.qRotation = QuaternionHelper::HmdQuaternionInit(1, 0, 0, 0);
    m_pose.qWorldFromDriverRotation = QuaternionHelper::HmdQuaternionInit(1, 0, 0, 0);
    m_pose.qDriverFromHeadRotation = QuaternionHelper::HmdQuaternionInit(1, 0, 0, 0);

    m_pose.vecPosition[1] = 1.5;
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
}

void *ControllerDriver::GetComponent(const char *pchComponentNameAndVersion)
{
    return nullptr;
}

vr::DriverPose_t ControllerDriver::GetPose()
{
    return m_pose;
}

void ControllerDriver::RunFrame()
{
    if (m_controllerId != vr::k_unTrackedDeviceIndexInvalid)
    {
        vr::VRServerDriverHost()->TrackedDevicePoseUpdated(m_controllerId, GetPose(), sizeof(vr::DriverPose_t));
    }
}