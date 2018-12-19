#pragma once
#include <IApp.h>
#include <glm/glm.hpp>
#include <render/geometries/BufferGeometry.h>
#include <render/Shader.h>

class HelloWorldApp : public blink::IApp
{
public:
	virtual bool initialize() override;
	virtual void terminate() override;

    virtual void update(float dt);
    virtual void render();

private:
    glm::mat4 m_matModel;
    glm::mat4 m_worldToCamera;
    glm::mat4 m_cameraToClip;

    blink::BufferGeometry* m_boxGeometry{};
    blink::Shader* m_shader{};

};
