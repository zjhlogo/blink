#pragma once
#include <IApp.h>
#include <objects/Scene.h>
#include <objects/Mesh.h>
#include <cameras/TargetCamera.h>
#include <input/MouseComponent.h>

class HelloWorldApp : public blink::IApp
{
    DECLARE_EVENT_HOST();

public:
    RTTI_DEF(HelloWorldApp, blink::IApp);

    HelloWorldApp();
    virtual ~HelloWorldApp();

	virtual bool initialize() override;
	virtual void terminate() override;

    virtual void update(float dt);
    virtual void render();

private:
    void onMouseEvent(blink::MouseEvent& evt);

private:
    blink::Scene* m_rootScene{};
    blink::TargetCamera* m_camera{};
    blink::Mesh* m_cube{};

};
