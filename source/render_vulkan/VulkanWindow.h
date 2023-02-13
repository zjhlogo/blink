/*!
 * \file VulkanWindow.h
 *
 * \author zjhlogo
 * \date 2020/04/11
 *
 *
 */
#pragma once
#include <foundation/BaseTypesGlm.h>

struct GLFWwindow;

namespace blink
{
    class VulkanContext;

    class VulkanWindow
    {
        friend class VulkanContext;

    public:
        VulkanWindow() = default;
        ~VulkanWindow();

        VulkanWindow(const VulkanWindow&) = delete;
        VulkanWindow(VulkanWindow&&) = delete;
        VulkanWindow& operator=(const VulkanWindow&) = delete;
        VulkanWindow& operator=(VulkanWindow&&) = delete;

        operator GLFWwindow*() { return m_window; }
        const glm::ivec2& getFrameBufferSize() const { return m_frameBufferSize; }

        bool create(const glm::ivec2& windowSize);
        void destroy();
        void updateFrameBufferSize();

    private:
        bool createWindow(const glm::ivec2& windowSize);
        void destroyWindow();

    private:
        GLFWwindow* m_window{};
        glm::ivec2 m_deviceSize{};
        glm::ivec2 m_frameBufferSize{};
    };
} // namespace blink
