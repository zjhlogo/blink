/*!
 * \file VulkanShader.h
 *
 * \author zjhlogo
 * \date 2020/02/17
 *
 *
 */
#include <render_base/Shader.h>
#include <vulkan/vulkan.hpp>

NS_BEGIN

class VulkanShader : public Shader
{
public:
    virtual bool reload() override;
    virtual bool setUniform(const char* pszName, int value) override;
    virtual bool setUniform(const char* pszName, float value) override;
    virtual bool setUniform(const char* pszName, const glm::vec2& v) override;
    virtual bool setUniform(const char* pszName, const glm::vec3& v) override;
    virtual bool setUniform(const char* pszName, const glm::vec4& v) override;
    virtual bool setUniform(const char* pszName, const glm::mat3& mat3) override;
    virtual bool setUniform(const char* pszName, const glm::mat4& mat4) override;
    virtual bool setUniform(const char* pszName, const std::vector<glm::mat4>& matList) override;

    virtual bool setUniform(const char* pszName, int count, const glm::vec3* v) override;
    virtual bool setUniform(const char* pszName, int count, const glm::vec4* v) override;
    virtual bool setUniform(const char* pszName, int count, const glm::mat4* v) override;

    virtual bool setTexture(const char* pszName, Texture* texture, uint32 slotIndex = 0) override;

private:
    vk::PipelineShaderStageCreateInfo m_shaderCreateInfo;
};

NS_END
