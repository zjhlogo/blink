/**

    @file      app.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      31.10.2021
    @copyright © zjhlogo, 2021. All right reserved.

**/
#pragma once

#include <flecs/flecs.h>
#include <foundation/BaseTypes.h>

#include <vector>

NS_BEGIN

class VulkanRenderModule;
class VulkanCommandBuffer;
class ISystemBase;

class IApp
{
public:
    IApp(){};
    virtual ~IApp();

    virtual bool initialize(VulkanRenderModule& renderModule) = 0;
    bool postInitialize();

    void preTerminate();
    virtual void terminate() = 0;

    virtual void update(float dt);
    virtual void render(VulkanCommandBuffer& commandBuffer);

protected:
    bool addSystem(ISystemBase* sys);
    void destroyAllSystems();

private:
    std::vector<ISystemBase*> m_systems;

protected:
    flecs::world m_world;
};

NS_END
