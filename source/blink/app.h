/**

    @file      app.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#pragma once

#include <flecs/flecs.h>

#include <vector>

namespace blink
{
    class VulkanCommandBuffer;
    class VulkanUniformBuffer;
    class VulkanRenderModule;
    class ILogicalSystem;
    class IRenderSystem;

    class IApp
    {
    public:
        IApp(){};
        virtual ~IApp();

        virtual bool initialize(VulkanRenderModule& renderModule) = 0;
        virtual void terminate() = 0;

        void executeLogicalSystems();
        void executeRenderSystems(VulkanCommandBuffer& commandBuffer, VulkanUniformBuffer& pfub, VulkanUniformBuffer& pmub, VulkanUniformBuffer& piub);

        const flecs::world& getWorld() const { return m_world; };

    protected:
        bool addLogicalSystem(ILogicalSystem* sys);
        bool initializeLogicalSystems();
        void terminateLogicalSystems();
        void destroyLogicalSystems();

        bool addRenderSystem(IRenderSystem* sys);
        bool initializeRenderSystems();
        void terminateRenderSystems();
        void destroyRenderSystems();

    protected:
        flecs::world m_world;
        std::vector<ILogicalSystem*> m_logicalSystems;
        std::vector<IRenderSystem*> m_renderSystems;
    };

} // namespace blink
