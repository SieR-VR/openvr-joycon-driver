#ifndef _JSL_GLUE_H_
#define _JSL_GLUE_H_

#include <functional>
#include <JSL/JoyShockLibrary.h>

class JSLGlue
{
public:
    JSLGlue();

    void init();
    void cleanup();

    void enable_callback();
    void disable_callback();

    bool isLeftConnected, isRightConnected;
    int leftHandle, rightHandle;
    std::function<void(JOY_SHOCK_STATE, IMU_STATE)> onLeftUpdate, onRightUpdate;

public:
    static JSLGlue instance;
};

#endif