#include <materials/Material.h>

class HatchingMaterial : public blink::Material
{
public:
    RTTI_DEF(HatchingMaterial, blink::Material);

    HatchingMaterial();
    virtual ~HatchingMaterial();

    virtual void setupShaderUniforms(blink::Shader* shader) override;

private:

};
