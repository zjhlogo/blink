/**

    @file      app.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#pragma once

#include <core/EcsWorld.h>

#include <vector>

namespace blink
{
    class VulkanCommandBuffer;
    class VulkanUniformBuffer;
    class VulkanRenderModule;
    class IRenderSystem;

    class IApp
    {
    public:
        IApp(){};
        virtual ~IApp();

        virtual bool initialize(VulkanRenderModule& renderModule) = 0;
        virtual void terminate() = 0;

        void stepEcsWorld();
        void render(VulkanCommandBuffer& commandBuffer,
                    VulkanUniformBuffer& pfub,
                    VulkanUniformBuffer& pmub,
                    VulkanUniformBuffer& piub);

        EcsWorld& getEcsWorld() { return m_ecsWorld; };

    protected:
        bool addRenderSystem(IRenderSystem* sys);
        bool initializeRenderSystems();
        void terminateRenderSystems();
        void destroyRenderSystems();

    protected:
        EcsWorld m_ecsWorld;
        std::vector<IRenderSystem*> m_renderSystems;
    };

} // namespace blink
