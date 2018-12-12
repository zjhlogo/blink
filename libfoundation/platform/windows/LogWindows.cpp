#include "../../Log.h"
#include <windows.h>
#include <sstream>
#include <iostream>

namespace blink
{
    void Log::print(const char* location, int line, LogPriority prio, const tstring& strMsg)
    {
        static const char* PRIORITY_MAP[static_cast<int>(LogPriority::NumPriority)] =
        {
            "Info",
            "Debug",
            "Warning",
            "Error",
        };

        std::ostringstream oss;
        oss << location << "(" << line << "): " << PRIORITY_MAP[static_cast<int>(prio)] << ": " << strMsg << std::endl;
        OutputDebugString(oss.str().c_str());
    }
}
