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
        class TexInfo
        {
        public:
            tstring name;
            Texture* texture{};
            uint32 index{};
        };

        static const int MAX_TEXTURES = 8;

    public:
        RTTI_ROOT(Material);

        virtual Shader* getShader() { return m_shader; };

        void setDiffuseColor(const glm::vec3& diffuseColor) { m_diffuseColor = diffuseColor; }
        const glm::vec3& getDiffuseColor() const { return m_diffuseColor; }

        Texture* setTexture(const tstring& name, const tstring& filePath, uint32 index);
        Texture* getTexture(uint32 index);
        Texture* getTexture(const tstring& name);

        virtual void setupShaderUniforms(Shader* shader) = 0;
        virtual void setupShaderSampler(Shader* shader);

        void release();
        int incReference() { return ++m_reference; };
        int decReference() { return --m_reference; };

    protected:
        Material();
        virtual ~Material();

    protected:
        Shader* m_shader{};
        glm::vec3 m_diffuseColor{ VEC3_ONE };
        TexInfo m_texInfos[MAX_TEXTURES];

    private:
        int m_reference{};

    };
}
