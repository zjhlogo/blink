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

class ImguiRenderSystem final : public blink::IRenderSystem
{
public:
    ImguiRenderSystem() = default;
    ~ImguiRenderSystem() override;

    ImguiRenderSystem(const ImguiRenderSystem&) = delete;
    ImguiRenderSystem(ImguiRenderSystem&&) = delete;
    ImguiRenderSystem& operator=(const ImguiRenderSystem&) = delete;
    ImguiRenderSystem& operator=(ImguiRenderSystem&&) = delete;

    bool initialize() override;
    void terminate() override;
    void render() override;

    void addWindow(IGuiWindow* window);
    void removeWindow(const IGuiWindow* window);

private:
    VkDescriptorPool m_imguiPool{};

    std::vector<IGuiWindow*> m_allWindows;
};
