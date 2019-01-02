#pragma once
#include <IApp.h>
#include <objects/Scene.h>
#include <objects/Mesh.h>
#include <cameras/Camera.h>

class HelloWorldApp : public blink::IApp
{
public:
    RTTI_DEF(HelloWorldApp, blink::IApp);

    HelloWorldApp();
    virtual ~HelloWorldApp();

	virtual bool initialize() override;
	virtual void terminate() override;

    virtual void update(float dt);
    virtual void render();

private:
    blink::Scene* m_rootScene{};
    blink::Camera* m_camera{};
    blink::Mesh* m_cube{};

};
