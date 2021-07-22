#include "DriverLog.h"

#include <stdio.h>
#include <stdarg.h>

static vr::IVRDriverLog *s_pLogFile = nullptr;

bool InitDriverLog(vr::IVRDriverLog *pDriverLog)
{
    if(s_pLogFile)
        return false;

    s_pLogFile = pDriverLog;
    return true;
}

void CleanupDriverLog()
{
    s_pLogFile = nullptr;
}

static void DriverLogVarArgs(const char *pFormat, va_list args)
{
    char buf[1024];
    vsnprintf(buf, sizeof(buf), pFormat, args);

    if(s_pLogFile)
        s_pLogFile->Log(buf);
}

void DriverLog(const char *pFormat, ...)
{
    va_list args;
    va_start(args, pFormat);

    DriverLogVarArgs(pFormat, args);

    va_end(args);
}

void DebugDriverLog(const char *pFormat, ...)
{
    va_list args;
    va_start(args, pFormat);

    DriverLogVarArgs(pFormat, args);

    va_end(args);
}