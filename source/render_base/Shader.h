/*!
 * \file Shader.h
 *
 * \author zjhlogo
 * \date 2019/07/26
 *
 *
 */
#pragma once
#include <foundation/BaseTypes.h>
#include <foundation/BaseTypesGlm.h>
#include <vector>

NS_BEGIN

class Texture;

class Shader
{
public:
    enum class StockShaders
    {
        Phong = 0,
        Wireframe,
        NumberOfStockShaders,
    };

    static const uint32 USE_DIFFUSE_TEXTURE = 1 << 0;

public:
    const tstring& getId() const { return m_id; };

    virtual bool reload() = 0;
    virtual bool setUniform(const char* pszName, int value) = 0;
    virtual bool setUniform(const char* pszName, float value) = 0;
    virtual bool setUniform(const char* pszName, const glm::vec2& v) = 0;
    virtual bool setUniform(const char* pszName, const glm::vec3& v) = 0;
    virtual bool setUniform(const char* pszName, const glm::vec4& v) = 0;
    virtual bool setUniform(const char* pszName, const glm::mat3& mat3) = 0;
    virtual bool setUniform(const char* pszName, const glm::mat4& mat4) = 0;
    virtual bool setUniform(const char* pszName, const std::vector<glm::mat4>& matList) = 0;

    virtual bool setUniform(const char* pszName, int count, const glm::vec3* v) = 0;
    virtual bool setUniform(const char* pszName, int count, const glm::vec4* v) = 0;
    virtual bool setUniform(const char* pszName, int count, const glm::mat4* v) = 0;

    template <typename T, int t_size> bool setUniform(const char* pszName, const T (&v)[t_size])
    {
        return setUniform(pszName, t_size, v);
    }

    virtual bool setTexture(const char* pszName, Texture* texture, uint32 slotIndex = 0) = 0;

protected:
    Shader(const tstring& id);
    virtual ~Shader();

protected:
    tstring m_id;
};

NS_END
