#include <materials/Material.h>

class MandelbrotMaterial : public blink::Material
{
public:
    RTTI_DEF(MandelbrotMaterial, blink::Material);

    MandelbrotMaterial();
    virtual ~MandelbrotMaterial();

    virtual void setupShaderUniforms(blink::Shader* shader) override;

private:
    float m_maxIterations{ 50.0f };
    float m_zoom{ 0.0025f };
    float m_escapeRadius{ 4.0f };
    glm::vec2 m_centerPos{ -0.64f, 0.0f };
    glm::vec3 m_colorScale{ 0.025f, 0.08f, 0.12f };

};
