#pragma once

#include "BaseTypes.h"
#include <fmt/format.h>

namespace blink
{
    class Log
    {
    public:
        enum class LogPriority
        {
            Info = 0,
            Debug,
            Warning,
            Error,
            NumPriority,
        };

    public:
        static void print(const char* location, int line, LogPriority prio, const tstring& strMsg);

    };
}

#define LOGI(...) (blink::Log::print(__FILE__, __LINE__, blink::Log::LogPriority::Info, fmt::format(__VA_ARGS__)))
#define LOGD(...) (blink::Log::print(__FILE__, __LINE__, blink::Log::LogPriority::Debug, fmt::format(__VA_ARGS__)))
#define LOGW(...) (blink::Log::print(__FILE__, __LINE__, blink::Log::LogPriority::Warning, fmt::format(__VA_ARGS__)))
#define LOGE(...) (blink::Log::print(__FILE__, __LINE__, blink::Log::LogPriority::Error, fmt::format(__VA_ARGS__)))
