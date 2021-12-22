#include "pch.h"
#include "Log.h"

winrt::Windows::Foundation::Diagnostics::LoggingChannel& GetLogger()
{
    static winrt::Windows::Foundation::Diagnostics::LoggingChannel logger{ L"debug" };
    return logger;
}
