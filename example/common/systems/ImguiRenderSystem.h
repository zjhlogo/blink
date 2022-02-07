/*********************************************************************
 * \file   ImguiRenderSystem.h
 * \brief
 *
 * \author zjhlogo
 * \date   02/07/2022
 *********************************************************************/
#pragma once

#include "../guis/IGuiWindow.h"

#include <blink/base/IRenderSystem.h>
#include <vulkan/vulkan.h>

#include <vector>

namespace blink
{
    class VulkanRenderModule;
}

class ImguiRenderSystem : public blink::IRenderSystem
{
public:
    ImguiRenderSystem(blink::VulkanRenderModule& renderModule);
    virtual ~ImguiRenderSystem();

    virtual bool initialize() override;
    virtual void terminate() override;
    virtual void render(blink::VulkanCommandBuffer& commandBuffer,
                        blink::VulkanUniformBuffer& pfub,
                        blink::VulkanUniformBuffer& pmub,
                        blink::VulkanUniformBuffer& piub) override;

    void addWindow(IGuiWindow* window);
    void removeWindow(IGuiWindow* window);

private:
    blink::VulkanRenderModule& m_renderModule;
    VkDescriptorPool m_imguiPool{};

    std::vector<IGuiWindow*> m_allWindows;
};
