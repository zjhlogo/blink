#pragma once
#include <IApp.h>
#include <objects/Scene.h>
#include <objects/Mesh.h>
#include <cameras/PerspectiveCamera.h>

class HatchingApp : public blink::IApp
{
public:
    RTTI_DEF(HatchingApp, blink::IApp);

    HatchingApp();
    virtual ~HatchingApp();

    virtual bool initialize() override;
    virtual void terminate() override;

    virtual void update(float dt);
    virtual void render();

private:
    blink::Scene* m_rootScene{};
    blink::PerspectiveCamera* m_camera{};

    blink::Mesh* m_cube{};
    blink::Mesh* m_sphere{};

};
