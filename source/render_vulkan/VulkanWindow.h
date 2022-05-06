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
        VulkanWindow();
        ~VulkanWindow();

        bool create(const glm::ivec2& windowSize);
        void destroy();

        operator GLFWwindow*() { return m_window; };

    private:
        bool createWindow(const glm::ivec2& windowSize);
        void destroyWindow();

    private:
        glm::ivec2 m_deviceSize{};
        GLFWwindow* m_window{};
    };

} // namespace blink
