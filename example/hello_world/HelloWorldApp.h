/*!
 * \file HelloWorldApp.h
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 *
 */
#pragma once
#include <blink/blink.h>
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
