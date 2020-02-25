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
}

bool VulkanShader::setUniform(const char* pszName, int value)
{
}

bool VulkanShader::setUniform(const char* pszName, float value)
{
}

bool VulkanShader::setUniform(const char* pszName, const glm::vec2& v)
{
}

bool VulkanShader::setUniform(const char* pszName, const glm::vec3& v)
{
}

bool VulkanShader::setUniform(const char* pszName, const glm::vec4& v)
{
}

bool VulkanShader::setUniform(const char* pszName, const glm::mat3& mat3)
{
}

bool VulkanShader::setUniform(const char* pszName, const glm::mat4& mat4)
{
}

bool VulkanShader::setUniform(const char* pszName, const std::vector<glm::mat4>& matList)
{
}

bool VulkanShader::setUniform(const char* pszName, int count, const glm::vec3* v)
{
}

bool VulkanShader::setUniform(const char* pszName, int count, const glm::vec4* v)
{
}

bool VulkanShader::setUniform(const char* pszName, int count, const glm::mat4* v)
{
}

bool VulkanShader::setTexture(const char* pszName, Texture* texture, uint32 slotIndex /*= 0*/)
{
}

NS_END
