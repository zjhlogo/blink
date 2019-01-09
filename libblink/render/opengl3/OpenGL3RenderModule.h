#pragma once
#include "../RenderModule.h"

namespace blink
{
    class Mesh;
    class Camera;
    class Light;

    class OpenGL3RenderModule : public RenderModule
    {
    public:
        RTTI_DEF(OpenGL3RenderModule, RenderModule);

        OpenGL3RenderModule();
        virtual ~OpenGL3RenderModule();

        virtual bool initialize(Entity* pEntity) override;
        virtual void terminate() override;

        virtual void render(Object* rootObj, Camera* camera) override;

    private:
        void renderObject(Mesh* mesh, Camera* camera, const std::vector<Light*>& lights);

    };
}
