#include "Light.h"

namespace blink
{
    class PointLight : public Light
    {
    public:
        RTTI_DEF(PointLight, Light);

        virtual void setupShaderUniforms(Shader* shader) override;

    };
}
