/**

    @file      IRenderSystem.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#pragma once

#include <foundation/BaseTypesGlm.h>

namespace blink
{
    class VulkanCommandBuffer;
    class VulkanUniformBuffer;

    class IRenderSystem
    {
    public:
        virtual bool initialize() = 0;
        virtual void terminate() = 0;
        virtual void render(VulkanCommandBuffer& commandBuffer, VulkanUniformBuffer& pfub, VulkanUniformBuffer& pmub, VulkanUniformBuffer& piub) = 0;

    };

} // namespace blink
