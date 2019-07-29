/*!
 * \file OpenGLShader.h
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 * 
 */
#pragma once
#include <Shader.h>
#include <glad/glad.h>

namespace blink
{
    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const tstring& id);
        virtual ~OpenGLShader();

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

        virtual bool setTexture(const char* pszName, std::shared_ptr<Texture> texture, uint32 slotIndex = 0) override;

    private:
        void destroyProgram();
        GLuint compileShader(uint32 shaderType, const std::vector<tstring>& shaderSources);
        bool getShaderErrorLog(GLuint shaderId);
        bool getProgramErrorLog(GLuint programId);

    private:
        GLuint m_programId{};

        std::vector<tstring> m_vertexShaderSources;
        std::vector<tstring> m_geometryShaderSources;
        std::vector<tstring> m_fragShaderSources;
        tstring m_errorLog;

    };
}
