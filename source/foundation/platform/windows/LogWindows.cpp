/*!
 * \file LogWindows.cpp
 *
 * \author zjhlogo
 * \date 2019/11/15
 *
 * 
 */
#include "../../Log.h"
#include <windows.h>
#include <sstream>

NS_BEGIN

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

NS_END
