/*!
 * \file VulkanShader.h
 *
 * \author zjhlogo
 * \date 2020/02/17
 *
 *
 */
#include <foundation/BaseTypesGlm.h>
#include <vulkan/vulkan.hpp>

namespace blink
{
    class VulkanTexture;

    class VulkanShader
    {
    public:
        bool reload();
        bool setUniform(const char* pszName, int value);
        bool setUniform(const char* pszName, float value);
        bool setUniform(const char* pszName, const glm::vec2& v);
        bool setUniform(const char* pszName, const glm::vec3& v);
        bool setUniform(const char* pszName, const glm::vec4& v);
        bool setUniform(const char* pszName, const glm::mat3& mat3);
        bool setUniform(const char* pszName, const glm::mat4& mat4);
        bool setUniform(const char* pszName, const std::vector<glm::mat4>& matList);

        bool setUniform(const char* pszName, int count, const glm::vec3* v);
        bool setUniform(const char* pszName, int count, const glm::vec4* v);
        bool setUniform(const char* pszName, int count, const glm::mat4* v);

        bool setTexture(const char* pszName, VulkanTexture* texture, uint32 slotIndex = 0);

    private:
        vk::PipelineShaderStageCreateInfo m_shaderCreateInfo;
    };

} // namespace blink
