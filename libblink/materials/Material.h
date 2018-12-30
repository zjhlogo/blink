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

        typedef std::map<uint32, TexInfo> TexInfoMap;

    public:
        RTTI_ROOT(Material);

        Material();

        virtual Shader* getShader() { return m_shader; };

        void setDiffuseColor(const glm::vec3& diffuseColor) { m_diffuseColor = diffuseColor; }
        const glm::vec3& getDiffuseColor() const { return m_diffuseColor; }

        bool setTexture(const tstring& name, const tstring& filePath, uint32 index);
        const Texture* getTexture(uint32 index);
        const Texture* getTexture(const tstring& name);

        virtual void setupShaderUniforms(Shader* shader) = 0;
        virtual void setupShaderSampler(Shader* shader);

        void release();
        int incReference() { return ++m_reference; };
        int decReference() { return --m_reference; };

    protected:
        virtual ~Material();

    protected:
        Shader* m_shader{};
        glm::vec3 m_diffuseColor{ VEC3_ONE };
        TexInfoMap m_texInfoMap;

    private:
        int m_reference{};

    };
}
