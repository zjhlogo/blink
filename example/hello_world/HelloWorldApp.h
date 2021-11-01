/**

    @file      HelloWorldApp.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#pragma once

#include <blink/app.h>
#include <blink/geometry/Mesh.h>
#include <blink/material/Material.h>
#include <render_vulkan/VulkanTexture.h>

class HelloWorldApp : public blink::IApp
{
public:
    HelloWorldApp();
    virtual ~HelloWorldApp();

    virtual bool initialize(blink::VulkanRenderModule& renderModule) override;
    virtual void terminate() override;

private:
    blink::Mesh* m_mesh{};
    blink::VulkanTexture* m_texture{};
    blink::Material* m_material{};
};
