/**

    @file      SphereUv.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      5.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#pragma once

#include "IGeometry.h"

namespace blink
{
    class SphereUv : public IGeometry
    {
    public:
        SphereUv(VulkanLogicalDevice& logicalDevice, VulkanCommandPool& commandPool);
        virtual ~SphereUv();

        bool create(const glm::vec3& center = glm::zero<glm::vec3>(), float radius = 0.5f, uint16 rings = 3, uint16 section = 3);
    };
} // namespace blink
