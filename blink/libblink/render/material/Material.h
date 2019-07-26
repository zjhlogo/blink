#pragma once
#include <Rtti.h>
#include <map>
#include <memory>

namespace blink
{
    class Shader;
    class Texture;

    class Material
    {
    public:
        class TexInfo
        {
        public:
            tstring name;
            std::shared_ptr<Texture> texture;
            uint32 index{};
        };

        static const int MAX_TEXTURES = 8;

    public:
        RTTI_ROOT(Material);
        Material();
        virtual ~Material();

        virtual std::shared_ptr<Shader> getShader() { return m_shader; };

        void setDiffuseColor(const glm::vec3& diffuseColor) { m_diffuseColor = diffuseColor; }
        const glm::vec3& getDiffuseColor() const { return m_diffuseColor; }

        std::shared_ptr<Texture> setTexture(const tstring& name, const tstring& filePath, uint32 index);
        std::shared_ptr<Texture> setTexture(const tstring& name, std::shared_ptr<Texture> texture, uint32 index);
        std::shared_ptr<Texture> getTexture(uint32 index);
        std::shared_ptr<Texture> getTexture(const tstring& name);

        virtual void setupShaderUniforms(std::shared_ptr<Shader> shader) = 0;
        virtual void setupShaderSampler(std::shared_ptr<Shader> shader);

    protected:
        std::shared_ptr<Shader> m_shader{};
        glm::vec3 m_diffuseColor{ VEC3_ONE };
        TexInfo m_texInfos[MAX_TEXTURES];

    };
}
