/**

    @file      Plane.h
    @brief     
    @details   ~
    @author    zjhlogo
    @date      4.11.2021
    @copyright © zjhlogo, 2021. All right reserved.

**/
#pragma once

#include "IGeometry.h"

namespace blink
{
    class Plane : public IGeometry
    {
    public:
        Plane(VulkanLogicalDevice& logicalDevice, VulkanCommandPool& commandPool);
        virtual ~Plane();

        bool create(float width, float height, uint32 widthSegments = 1, uint32 heightSegments = 1);

    };
} // namespace blink
