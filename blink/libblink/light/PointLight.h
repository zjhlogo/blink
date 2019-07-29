#include "Light.h"

namespace blink
{
    class PointLight : public Light
    {
    public:
        PointLight();
        virtual ~PointLight();

        virtual void setupShaderUniforms(std::shared_ptr<Shader> shader, const TransformData& transform) override;

    };
}
