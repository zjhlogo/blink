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

class HelloWorldApp : public NS::IApp
{
public:
    HelloWorldApp();
    virtual ~HelloWorldApp();

    virtual bool initialize(NS::VulkanRenderModule& renderModule) override;
    virtual void terminate() override;

private:
    NS::Mesh* m_mesh{};
    NS::VulkanTexture* m_texture{};
    NS::Material* m_material{};
};
