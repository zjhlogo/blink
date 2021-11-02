/**

    @file      Mesh.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#pragma once

#include "IGeometry.h"

#include <vulkan/vulkan.h>

#include <vector>

namespace blink
{
    class Mesh : public IGeometry
    {
    public:
        Mesh(VulkanLogicalDevice& logicalDevice, VulkanCommandPool& commandPool);
        virtual ~Mesh() {};

        bool loadFromFile(const tstring& filePath);

    };

} // namespace blink
