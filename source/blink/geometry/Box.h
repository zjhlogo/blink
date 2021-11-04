/**

    @file      Box.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      2.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#pragma once
#include "IGeometry.h"

namespace blink
{
    class Box : public IGeometry
    {
    public:
        Box(VulkanLogicalDevice& logicalDevice, VulkanCommandPool& commandPool);
        virtual ~Box();

        bool create(float width, float height, float depth, int widthSegments = 1, int heightSegments = 1, int depthSegments = 1);

    private:
        float m_width{};
        float m_height{};
        float m_depth{};
        int m_widthSegments{};
        int m_heightSegments{};
        int m_depthSegments{};
    };
} // namespace blink