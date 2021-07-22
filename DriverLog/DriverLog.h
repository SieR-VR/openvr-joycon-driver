#ifndef DRIVERLOG_H
#define DRIVERLOG_H

#include <openvr_driver.h>

extern void DriverLog(const char* format, ...);
extern void DebugDriverLog(const char* format, ...);

extern bool InitDriverLog(vr::IVRDriverLog *pDriverLog);
extern void CleanupDriverLog();

#endif