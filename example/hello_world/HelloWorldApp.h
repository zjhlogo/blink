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

namespace blink
{
    class Plane;
    class Box;
    class SphereUv;
    class Mesh;
    class VulkanTexture;
    class Material;
}

class HelloWorldApp : public blink::IApp
{
public:
    HelloWorldApp();
    virtual ~HelloWorldApp();

    virtual bool initialize(blink::VulkanRenderModule& renderModule) override;
    virtual void terminate() override;

private:
    blink::Plane* m_plane{};
    blink::Box* m_box{};
    blink::SphereUv* m_sphereUv{};
    blink::Mesh* m_mesh{};
    blink::VulkanTexture* m_texture{};
    blink::Material* m_material{};
};
