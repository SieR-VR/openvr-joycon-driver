#include "driverlog.h"
#include "jsl_glue.h"

JSLGlue JSLGlue::instance;

JSLGlue::JSLGlue()
{
    isLeftConnected = false;
    isRightConnected = false;

    onLeftUpdate = [this](JOY_SHOCK_STATE state, IMU_STATE imu) {};
    onRightUpdate = [this](JOY_SHOCK_STATE state, IMU_STATE imu) {};
}

void JSLGlue::init()
{
    int device_count = JslConnectDevices();
    if (!device_count)
    {
        DriverLog("No devices found");
        return;
    }

    int *device_handles = new int[device_count];
    int connected_device_count = JslGetConnectedDeviceHandles(device_handles, device_count);

    for (int i = 0; i < connected_device_count; i++)
    {
        int handle = device_handles[i];
        int type = JslGetControllerType(handle);

        switch (type)
        {
        case JS_TYPE_JOYCON_LEFT:
            isLeftConnected = true;
            leftHandle = handle;
            break;
        case JS_TYPE_JOYCON_RIGHT:
            isRightConnected = true;
            rightHandle = handle;
            break;
        default:
            break;
        }
    }

    delete[] device_handles;

    DriverLog("Left connected: %s", isLeftConnected ? "true" : "false");
    DriverLog("Right connected: %s", isRightConnected ? "true" : "false");
}

void JSLGlue::cleanup()
{
    JslDisconnectAndDisposeAll();
}

void JSLGlue::enable_callback()
{
    JslSetCallback(poll);
}

void JSLGlue::disable_callback()
{
    JslSetCallback(poll_null);
}

void poll(int deviceId, JOY_SHOCK_STATE state, JOY_SHOCK_STATE prev_state, IMU_STATE imu_state, IMU_STATE prev_imu_state, float dt) {
    if (JSLGlue::instance.isLeftConnected && deviceId == JSLGlue::instance.leftHandle) {
        JSLGlue::instance.onLeftUpdate(state, imu_state);
    }
    else if (JSLGlue::instance.isRightConnected && deviceId == JSLGlue::instance.rightHandle) {
        JSLGlue::instance.onRightUpdate(state, imu_state);
    }
}

void poll_null(int deviceId, JOY_SHOCK_STATE state, JOY_SHOCK_STATE prev_state, IMU_STATE imu_state, IMU_STATE prev_imu_state, float dt) {}