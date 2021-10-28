/*!
 * \file blink.h
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 *
 */
#pragma once
#include <foundation/BaseTypes.h>
#include <flecs/flecs.h>

NS_BEGIN

class VulkanRenderModule;
class VulkanCommandBuffer;

class IApp
{
public:
    virtual ~IApp();

    virtual bool initialize(VulkanRenderModule& renderModule) = 0;
    virtual void terminate() = 0;

    virtual void update(float dt);
    virtual void render(VulkanCommandBuffer& commandBuffer);

protected:
    flecs::world m_world;

};

int run(IApp& app);

NS_END
