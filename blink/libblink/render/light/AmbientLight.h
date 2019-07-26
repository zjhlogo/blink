#include "Light.h"

namespace blink
{
    class AmbientLight : public Light
    {
    public:

        AmbientLight();
        virtual ~AmbientLight();

        virtual void setupShaderUniforms(std::shared_ptr<Shader> shader, const TransformData& transform) override;

    };
}
