#pragma once
#include <IApp.h>
#include <render/Scene.h>
#include <render/cameras/PerspectiveCamera.h>
#include <render/objects/Mesh.h>

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
    blink::PerspectiveCamera* m_camera{};
    blink::Mesh* m_cube{};

};
