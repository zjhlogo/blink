/*!
 * \file VulkanRenderModule.h
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 * 
 */
#pragma once
#include <RenderModule.h>

#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

namespace blink
{
    class VulkanRenderModule : public RenderModule
    {
    public:
        VulkanRenderModule();
        virtual ~VulkanRenderModule();

        virtual bool createDevice(const glm::ivec2& deviceSize) override;
        virtual void destroyDevice() override;

        virtual bool gameLoop() override;

    private:
        GLFWwindow* m_window{};

    };
}