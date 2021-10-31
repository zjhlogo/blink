/**

    @file      HelloWorldApp.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      31.10.2021
    @copyright © zjhlogo, 2021. All right reserved.

**/
#pragma once

#include <blink/app.h>
#include <blink/geometry/Mesh.h>
#include <blink/material/Material.h>

class HelloWorldApp : public NS::IApp
{
public:
    HelloWorldApp();
    virtual ~HelloWorldApp();

    virtual bool initialize(NS::VulkanRenderModule& renderModule) override;
    virtual void terminate() override;

private:
    NS::Mesh* m_mesh{};
    NS::Material* m_material{};
};
