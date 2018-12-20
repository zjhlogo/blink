#include "Light.h"

namespace blink
{
    class AmbientLight : public Light
    {
    public:
        RTTI_DEF(AmbientLight, Light);

        AmbientLight();
        virtual ~AmbientLight();

        virtual void setupShaderUniforms(Shader* shader) override;

    };
}
