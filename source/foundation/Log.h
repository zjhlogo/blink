/*!
 * \file Log.h
 *
 * \author zjhlogo
 * \date 2019/11/15
 *
 *
 */
#pragma once

#include "BaseTypes.h"

#include <fmt/format.h>

NS_BEGIN

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

NS_END

#define LOGI(...) (NS::Log::print(__FILE__, __LINE__, NS::Log::LogPriority::Info, fmt::format(__VA_ARGS__)))
#define LOGD(...) (NS::Log::print(__FILE__, __LINE__, NS::Log::LogPriority::Debug, fmt::format(__VA_ARGS__)))
#define LOGW(...) (NS::Log::print(__FILE__, __LINE__, NS::Log::LogPriority::Warning, fmt::format(__VA_ARGS__)))
#define LOGE(...) (NS::Log::print(__FILE__, __LINE__, NS::Log::LogPriority::Error, fmt::format(__VA_ARGS__)))
