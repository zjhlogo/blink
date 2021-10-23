/*!
 * \file VulkanShader.cpp
 *
 * \author zjhlogo
 * \date 2020/02/17
 *
 *
 */
#include "VulkanShader.h"

NS_BEGIN

bool VulkanShader::reload()
{
    return false;
}

bool VulkanShader::setUniform(const char* pszName, int value)
{
    return false;
}

bool VulkanShader::setUniform(const char* pszName, float value)
{
    return false;
}

bool VulkanShader::setUniform(const char* pszName, const glm::vec2& v)
{
    return false;
}

bool VulkanShader::setUniform(const char* pszName, const glm::vec3& v)
{
    return false;
}

bool VulkanShader::setUniform(const char* pszName, const glm::vec4& v)
{
    return false;
}

bool VulkanShader::setUniform(const char* pszName, const glm::mat3& mat3)
{
    return false;
}

bool VulkanShader::setUniform(const char* pszName, const glm::mat4& mat4)
{
    return false;
}

bool VulkanShader::setUniform(const char* pszName, const std::vector<glm::mat4>& matList)
{
    return false;
}

bool VulkanShader::setUniform(const char* pszName, int count, const glm::vec3* v)
{
    return false;
}

bool VulkanShader::setUniform(const char* pszName, int count, const glm::vec4* v)
{
    return false;
}

bool VulkanShader::setUniform(const char* pszName, int count, const glm::mat4* v)
{
    return false;
}

bool VulkanShader::setTexture(const char* pszName, VulkanTexture* texture, uint32 slotIndex /*= 0*/)
{
    return false;
}

NS_END
