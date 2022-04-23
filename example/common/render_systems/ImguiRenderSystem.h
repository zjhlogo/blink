/*********************************************************************
 * \file   ImguiRenderSystem.h
 * \brief
 *
 * \author zjhlogo
 * \date   02/07/2022
 *********************************************************************/
#pragma once

#include "../guis/IGuiWindow.h"

#include <core/IRenderSystem.h>
#include <vulkan/vulkan.h>

#include <vector>

class ImguiRenderSystem : public blink::IRenderSystem
{
public:
    ImguiRenderSystem();
    virtual ~ImguiRenderSystem();

    virtual bool initialize() override;
    virtual void terminate() override;
    virtual void render(blink::IRenderData& renderData) override;

    void addWindow(IGuiWindow* window);
    void removeWindow(IGuiWindow* window);

private:
    VkDescriptorPool m_imguiPool{};

    std::vector<IGuiWindow*> m_allWindows;
};
