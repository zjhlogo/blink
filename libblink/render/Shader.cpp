#include "Shader.h"
#include "textures/Texture.h"
#include "GlConfig.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace blink
{
    class AutoDeleteShaderObj
    {
    public:
        AutoDeleteShaderObj(uint32 shaderId) :m_shaderId(shaderId) {};
        ~AutoDeleteShaderObj() { if (m_shaderId != 0) glDeleteShader(m_shaderId); };

    private:
        uint32 m_shaderId{};

    };

    bool Shader::loadFromFile(const tstring& filePath)
    {
        destroyProgram();

        m_programId = glCreateProgram();
        if (m_programId == 0) return false;

        // create vertex shader
        GLuint vertexId = compileShader(GL_VERTEX_SHADER, m_vertexShaderData);
        AutoDeleteShaderObj vertexShaderObj(vertexId);
        if (vertexId == 0) return false;

        // create fragment shader
        GLuint fragmentId = compileShader(GL_FRAGMENT_SHADER, m_fragShaderData);
        AutoDeleteShaderObj fragmentShaderObj(fragmentId);
        if (fragmentId == 0) return false;

        // attach shader
        glAttachShader(m_programId, vertexId);
        glAttachShader(m_programId, fragmentId);

        //// bind attribute
        //int numAttrs = m_pVertAttributes->getNumAttributeItems();
        //for (int i = 0; i < numAttrs; ++i)
        //{
        //    const VertexAttributes::AttributeItem* pAttrItem = m_pVertAttributes->getAttributeItem(i);
        //    glBindAttribLocation(m_programId, i, pAttrItem->m_name.c_str());
        //}

        // link program
        glLinkProgram(m_programId);
        if (getProgramErrorLog(m_programId))
        {
            LOGE("Link program failed with error log {0}", m_errorLog);
            return false;
        }

        //// bind texture slot
        //glUseProgram(m_programId);
        //for (auto textureInfo : m_textureInfoMap)
        //{
        //    TextureInfo& info = textureInfo.second;
        //    GLint loc = glGetUniformLocation(m_programId, info.m_name.c_str());
        //    GL_ERROR_CHECK();

        //    glUniform1i(loc, info.m_slotId);
        //    GL_ERROR_CHECK();
        //}
        //glUseProgram(0);

        return true;
    }

    bool Shader::setUniform(const char* pszName, int value)
    {
        // First gets the location of that variable in the shader using its name
        int loc = glGetUniformLocation(m_programId, pszName);
        GL_ERROR_CHECK();
        if (loc == -1) return false;

        // Then passes the value to that variable
        glUniform1i(loc, value);
        GL_ERROR_CHECK();

        return true;
    }

    bool Shader::setUniform(const char* pszName, float value)
    {
        // First gets the location of that variable in the shader using its name
        int loc = glGetUniformLocation(m_programId, pszName);
        GL_ERROR_CHECK();
        if (loc == -1) return false;

        // Then passes the matrix to that variable
        glUniform1f(loc, value);
        GL_ERROR_CHECK();

        return true;
    }

    bool Shader::setUniform(const char* pszName, const glm::vec2& v)
    {
        // First gets the location of that variable in the shader using its name
        int loc = glGetUniformLocation(m_programId, pszName);
        GL_ERROR_CHECK();
        if (loc == -1) return false;

        // Then passes the matrix to that variable
        glUniform2f(loc, v.x, v.y);
        GL_ERROR_CHECK();

        return true;
    }

    bool Shader::setUniform(const char* pszName, const glm::vec3& v)
    {
        // First gets the location of that variable in the shader using its name
        int loc = glGetUniformLocation(m_programId, pszName);
        GL_ERROR_CHECK();
        if (loc == -1) return false;

        // Then passes the matrix to that variable
        glUniform3f(loc, v.x, v.y, v.z);
        GL_ERROR_CHECK();

        return true;
    }

    bool Shader::setUniform(const char* pszName, const glm::vec4& v)
    {
        // First gets the location of that variable in the shader using its name
        int loc = glGetUniformLocation(m_programId, pszName);
        GL_ERROR_CHECK();
        if (loc == -1) return false;

        // Then passes the matrix to that variable
        glUniform4f(loc, v.x, v.y, v.z, v.w);
        GL_ERROR_CHECK();

        return true;
    }

    bool Shader::setUniform(const char* pszName, const glm::mat3& mat3)
    {
        // First gets the location of that variable in the shader using its name
        int loc = glGetUniformLocation(m_programId, pszName);
        GL_ERROR_CHECK();
        if (loc == -1) return false;

        const float* pValue = glm::value_ptr(mat3);

        // Then passes the matrix to that variable
        glUniformMatrix3fv(loc, 1, GL_FALSE, pValue);
        GL_ERROR_CHECK();

        return true;
    }

    bool Shader::setUniform(const char* pszName, const glm::mat4& mat4)
    {
        // First gets the location of that variable in the shader using its name
        int loc = glGetUniformLocation(m_programId, pszName);
        GL_ERROR_CHECK();
        if (loc == -1) return false;

        const float* pValue = glm::value_ptr(mat4);

        // Then passes the matrix to that variable
        glUniformMatrix4fv(loc, 1, GL_FALSE, pValue);
        GL_ERROR_CHECK();

        return true;
    }

    bool Shader::setUniform(const char* pszName, const Mat4List& matList)
    {
        if (matList.empty()) return false;

        // First gets the location of that variable in the shader using its name
        int loc = glGetUniformLocation(m_programId, pszName);
        GL_ERROR_CHECK();
        if (loc == -1) return false;

        const float* pValue = glm::value_ptr(matList[0]);

        // Then passes the matrix to that variable
        glUniformMatrix4fv(loc, matList.size(), GL_FALSE, pValue);
        GL_ERROR_CHECK();

        return true;
    }

    bool Shader::setUniform(const char* pszName, int count, const glm::vec3* v)
    {
        // First gets the location of that variable in the shader using its name
        int loc = glGetUniformLocation(m_programId, pszName);
        GL_ERROR_CHECK();
        if (loc == -1) return false;

        const auto* pValue = glm::value_ptr(*v);

        // Then passes the matrix to that variable
        glUniform3fv(loc, count, pValue);
        GL_ERROR_CHECK();

        return true;
    }

    bool Shader::setUniform(const char* pszName, int count, const glm::vec4* v)
    {
        // First gets the location of that variable in the shader using its name
        int loc = glGetUniformLocation(m_programId, pszName);
        GL_ERROR_CHECK();
        if (loc == -1) return false;

        const auto* pValue = glm::value_ptr(*v);

        // Then passes the matrix to that variable
        glUniform4fv(loc, count, pValue);
        GL_ERROR_CHECK();

        return true;
    }

    bool Shader::setUniform(const char* pszName, int count, const glm::mat4* v)
    {
        // First gets the location of that variable in the shader using its name
        int loc = glGetUniformLocation(m_programId, pszName);
        GL_ERROR_CHECK();
        if (loc == -1) return false;

        const auto* pValue = glm::value_ptr(*v);

        // Then passes the matrix to that variable
        glUniformMatrix4fv(loc, count, GL_FALSE, pValue);
        GL_ERROR_CHECK();

        return true;
    }

    bool Shader::setTexture(const char * pszName, const Texture * pTexture, uint32 slotIndex/* = 0 */)
    {
        if (!pszName || !pTexture) return false;

        glActiveTexture(GL_TEXTURE0 + slotIndex);
        glBindTexture(GL_TEXTURE_2D, pTexture->getTextureId());
        GL_ERROR_CHECK();

        return true;
    }

    Shader::Shader()
    {

    }

    Shader::~Shader()
    {
        destroyProgram();
    }

    void Shader::destroyProgram()
    {
        if (m_programId != 0)
        {
            glDeleteProgram(m_programId);
            m_programId = 0;
        }
    }

    uint32 Shader::compileShader(uint32 shaderType, const tstring & shaderData)
    {
        if (shaderData.length() <= 0) return 0;

        int shaderId = glCreateShader(shaderType);
        if (shaderId == 0) return 0;

        const char* pszShader = shaderData.c_str();
        glShaderSource(shaderId, 1, &pszShader, nullptr);
        glCompileShader(shaderId);

        if (getShaderErrorLog(shaderId))
        {
            LOGE("Compile shader failed with error log {0}", m_errorLog);
            glDeleteShader(shaderId);
            return 0;
        }

        return shaderId;
    }

    bool Shader::getShaderErrorLog(uint32 shaderId)
    {
        GLint compileStatus = 0;
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus);
        if (compileStatus == GL_TRUE) return false;

        GLint infoLength = 0;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLength);
        if (infoLength <= 1) return false;

        BufferData buffer;
        buffer.resize(infoLength);

        GLsizei charWritten = 0;
        glGetShaderInfoLog(shaderId, infoLength, &charWritten, (char*)buffer.data());

        m_errorLog.clear();
        m_errorLog.append((char*)buffer.data(), infoLength);

        return true;
    }

    bool Shader::getProgramErrorLog(uint32 programId)
    {
        GLint linkStatus = 0;
        glGetProgramiv(programId, GL_LINK_STATUS, &linkStatus);
        if (linkStatus == GL_TRUE) return false;

        GLint infoLength = 0;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLength);
        if (infoLength <= 1) return false;

        BufferData buffer;
        buffer.resize(infoLength);

        GLsizei charWritten = 0;
        glGetProgramInfoLog(programId, infoLength, &charWritten, (char*)buffer.data());

        m_errorLog.clear();
        m_errorLog.append((char*)buffer.data(), infoLength);

        return true;
    }
}
