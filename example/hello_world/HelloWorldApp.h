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
#include <guis/IGuiWindow.h>

class EntityCreationSystem;

class HelloWorldApp : public blink::IApp, public IGuiWindow
{
public:
    HelloWorldApp();
    virtual ~HelloWorldApp();

    virtual bool initialize(blink::VulkanRenderModule& renderModule) override;
    virtual void terminate() override;

    virtual void renderUi() override;

private:
    EntityCreationSystem* m_sys{};

};
