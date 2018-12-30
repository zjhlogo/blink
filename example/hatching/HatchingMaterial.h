#include <materials/Material.h>

class HatchingMaterial : public blink::Material
{
public:
    RTTI_DEF(HatchingMaterial, blink::Material);

    HatchingMaterial();

    virtual void setupShaderUniforms(blink::Shader* shader) override;

protected:
    virtual ~HatchingMaterial();

};
