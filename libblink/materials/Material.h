#pragma once
#include <Rtti.h>
#include <map>

namespace blink
{
    class Shader;
    class Texture;

    class Material
    {
    public:
        enum class TextureType
        {
            Diffuse,
            Normal,
            NumTextureType,
        };

        typedef std::map<TextureType, const Texture*> textureMap;

    public:
        RTTI_ROOT(Material);

        Material();
        virtual ~Material();

        Shader* getShader() const { return m_shader; };

        void setDiffuseColor(const glm::vec3& diffuseColor) { m_diffuseColor = diffuseColor; }
        const glm::vec3& getDiffuseColor() const { return m_diffuseColor; }

        void setTexture(TextureType type, const Texture* texture);
        const Texture* getTexture(TextureType type);

        virtual void setupShaderUniforms(Shader* shader) = 0;
        virtual void setupShaderSampler(Shader* shader);

    protected:
        Shader* m_shader{};
        glm::vec3 m_diffuseColor{ VEC3_ONE };
        textureMap m_textureMap;

    };
}
