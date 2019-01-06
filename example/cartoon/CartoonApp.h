#pragma once
#include <IApp.h>
#include <objects/Scene.h>
#include <objects/Mesh.h>
#include <cameras/Camera.h>

class CartoonApp : public blink::IApp
{
public:
    RTTI_DEF(CartoonApp, blink::IApp);

    CartoonApp();
    virtual ~CartoonApp();

	virtual bool initialize() override;
	virtual void terminate() override;

    virtual void update(float dt);
    virtual void render();

private:
    blink::Scene* m_rootScene{};
    blink::Camera* m_camera{};

};
