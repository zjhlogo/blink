/*!
 * \file Material.h
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 * 
 */
#pragma once
#include <BaseTypes.h>
#include <BaseTypesGlm.h>

NS_BEGIN

class Shader;
class Texture;

class Material
{
public:
    class TexInfo
    {
    public:
        tstring name;
        Texture* texture;
        uint32 index{};
    };

    static const int MAX_TEXTURES = 8;

public:
    Material();
    virtual ~Material();

    virtual Shader* getShader() { return m_shader; };

    void setDiffuseColor(const glm::vec3& diffuseColor) { m_diffuseColor = diffuseColor; }
    const glm::vec3& getDiffuseColor() const { return m_diffuseColor; }

    Texture* setTexture(const tstring& name, const tstring& filePath, uint32 index);
    Texture* setTexture(const tstring& name, Texture* texture, uint32 index);
    Texture* getTexture(uint32 index);
    Texture* getTexture(const tstring& name);

    virtual void setupShaderUniforms(Shader* shader) = 0;
    virtual void setupShaderSampler(Shader* shader);

protected:
    Shader* m_shader{};
    glm::vec3 m_diffuseColor{ VEC3_ONE };
    TexInfo m_texInfos[MAX_TEXTURES];

};

NS_END
