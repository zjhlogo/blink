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
    class VulkanRenderModule;
    class VulkanCommandBuffer;
    class VulkanUniformBuffer;
    class ISystemBase;

    class IApp
    {
    public:
        IApp(){};
        virtual ~IApp();

        virtual bool initialize(VulkanRenderModule& renderModule) = 0;
        virtual void terminate() = 0;

        virtual void update(float dt);
        virtual void render(VulkanCommandBuffer& commandBuffer, VulkanUniformBuffer& pfub, VulkanUniformBuffer& pmub, VulkanUniformBuffer& piub);

    protected:
        bool addSystem(ISystemBase* sys);
        bool initializeSystems();
        void terminateSystems();
        void destroyAllSystems();

    protected:
        flecs::world m_world;

    private:
        std::vector<ISystemBase*> m_systems;
    };

} // namespace blink
