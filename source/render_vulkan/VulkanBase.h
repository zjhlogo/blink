/*********************************************************************
 * \file   VulkanBase.h
 * \brief
 *
 * \author zjhlogo
 * \date   05/06/2022
 *********************************************************************/
#pragma once
#include <foundation/Log.h>
#include <vulkan/vulkan.h>

#include <cassert>

namespace blink
{
#define VK_CHECK_RESULT(f)                                                                                                                                     \
    {                                                                                                                                                          \
        VkResult res = (f);                                                                                                                                    \
        if (res != VK_SUCCESS)                                                                                                                                 \
        {                                                                                                                                                      \
            LOGE("Fatal : VkResult is {0} in {1}:({2})", res, __FILE__, __LINE__);                                                                             \
            assert(res == VK_SUCCESS);                                                                                                                         \
        }                                                                                                                                                      \
    }

#define VK_CHECK_RESULT_BOOL(f)                                                                                                                                \
    {                                                                                                                                                          \
        VkResult res = (f);                                                                                                                                    \
        if (res != VK_SUCCESS)                                                                                                                                 \
        {                                                                                                                                                      \
            LOGE("Fatal : VkResult is {0} in {1}:({2})", res, __FILE__, __LINE__);                                                                             \
            assert(res == VK_SUCCESS);                                                                                                                         \
            return false;                                                                                                                                      \
        }                                                                                                                                                      \
    }
} // namespace blink
