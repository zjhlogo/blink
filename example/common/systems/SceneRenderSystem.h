/*********************************************************************
 * \file   SceneRenderSystem.h
 * \brief
 *
 * \author zjhlogo
 * \date   02/07/2022
 *********************************************************************/
#pragma once

#include <blink/app.h>
#include <blink/base/IRenderSystem.h>

class SceneRenderSystem : public blink::IRenderSystem
{
public:
    SceneRenderSystem(blink::IApp* app);
    ~SceneRenderSystem();

    virtual bool initialize() override;
    virtual void terminate() override;
    virtual void render(blink::VulkanCommandBuffer& commandBuffer,
                        blink::VulkanUniformBuffer& pfub,
                        blink::VulkanUniformBuffer& pmub,
                        blink::VulkanUniformBuffer& piub) override;

private:
    blink::IApp* m_app{};

};
