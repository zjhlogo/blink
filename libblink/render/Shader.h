#pragma once
#include <BaseType.h>

namespace blink
{
    class Texture;

    class Shader
    {
    public:
        bool loadFromFile(const tstring& filePath);
        bool setUniform(const char* pszName, int value);
        bool setUniform(const char* pszName, float value);
        bool setUniform(const char* pszName, const glm::vec2& v);
        bool setUniform(const char* pszName, const glm::vec3& v);
        bool setUniform(const char* pszName, const glm::vec4& v);
        bool setUniform(const char* pszName, const glm::mat3& mat3);
        bool setUniform(const char* pszName, const glm::mat4& mat4);
        bool setUniform(const char* pszName, const Mat4List& matList);

        bool setUniform(const char* pszName, int count, const glm::vec3* v);
        bool setUniform(const char* pszName, int count, const glm::vec4* v);
        bool setUniform(const char* pszName, int count, const glm::mat4* v);

        template<typename T, int t_size>
        bool setUniform(const char* pszName, const T(&v)[t_size]) { return setUniform(pszName, t_size, v); };

        bool setTexture(const char* pszName, const Texture* pTexture, uint32 slotIndex = 0);

    private:
        Shader();
        ~Shader();

        void destroyProgram();
        uint32 compileShader(uint32 shaderType, const tstring& shaderData);
        bool getShaderErrorLog(uint32 shaderId);
        bool getProgramErrorLog(uint32 programId);

    private:
        uint32 m_programId{};

        tstring m_vertexShaderData;
        tstring m_fragShaderData;

        tstring m_errorLog;

    };
}
