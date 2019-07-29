/*!
 * \file AmbientLight.h
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 * 
 */
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
