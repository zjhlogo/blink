#include "Shader.h"
#include "ShaderLib.h"
#include "../textures/Texture.h"
#include "../render/GlConfig.h"
#include <InstanceManager.h>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace blink
{
    static InstanceManager<tstring, Shader> s_instanceManager;

    class AutoDeleteShaderObj
    {
    public:
        AutoDeleteShaderObj(uint32 shaderId) :m_shaderId(shaderId) {};
        ~AutoDeleteShaderObj() { if (m_shaderId != 0) glDeleteShader(m_shaderId); };

    private:
        uint32 m_shaderId{};

    };

    Shader * Shader::fromStock(StockShaders stockShader, uint32 preprocessDefine)
    {
        static const char* s_stockVs[static_cast<int>(StockShaders::NumberOfStockShaders)] =
        {
            PHONG_VS,                 // Phong
            WIREFRAME_VS,               // Wireframe
        };

        static const char* s_stockGs[static_cast<int>(StockShaders::NumberOfStockShaders)] =
        {
            nullptr,                    // Phong
            WIREFRAME_GS,               // Wireframe
        };

        static const char* s_stockFs[static_cast<int>(StockShaders::NumberOfStockShaders)] =
        {
            PHONG_FS,                 // Phong
            WIREFRAME_FS,               // Wireframe
        };

        tstring shaderId = makeId(stockShader, preprocessDefine);
        auto exitShader = s_instanceManager.insertInstance(shaderId);
        if (exitShader) return exitShader;

        Shader* shader = new Shader();

        concatShaderSources(shader->m_vertexShaderSources, preprocessDefine, s_stockVs[static_cast<int>(stockShader)]);
        concatShaderSources(shader->m_geometryShaderSources, preprocessDefine, s_stockGs[static_cast<int>(stockShader)]);
        concatShaderSources(shader->m_fragShaderSources, preprocessDefine, s_stockFs[static_cast<int>(stockShader)]);

        if (!shader->reload())
        {
            SAFE_DELETE(shader);
            return nullptr;
        }

        return s_instanceManager.insertInstance(shaderId, shader);
    }

    Shader * Shader::fromBuffer(const tstring & id, const char* vsBuffer, const char* gsBuffer, const char* fsBuffer)
    {
        tstring formatedId = "buffer::" + id;

        auto exitShader = s_instanceManager.insertInstance(formatedId);
        if (exitShader) return exitShader;

        Shader* shader = new Shader();

        concatShaderSources(shader->m_vertexShaderSources, 0, vsBuffer);
        concatShaderSources(shader->m_geometryShaderSources, 0, gsBuffer);
        concatShaderSources(shader->m_fragShaderSources, 0, fsBuffer);

        if (!shader->reload())
        {
            SAFE_DELETE(shader);
            return nullptr;
        }

        return s_instanceManager.insertInstance(formatedId, shader);
    }

    bool Shader::reload()
    {
        destroyProgram();

        m_programId = glCreateProgram();
        if (m_programId == 0) return false;

        // create vertex shader
        if (!m_vertexShaderSources.empty())
        {
            GLuint vertexId = compileShader(GL_VERTEX_SHADER, m_vertexShaderSources);
            AutoDeleteShaderObj vertexShaderObj(vertexId);
            if (vertexId == 0) return false;
            glAttachShader(m_programId, vertexId);
        }

        // create geometry shader
        if (!m_geometryShaderSources.empty())
        {
            GLuint geometryId = compileShader(GL_GEOMETRY_SHADER, m_geometryShaderSources);
            AutoDeleteShaderObj geometryShaderObj(geometryId);
            if (geometryId == 0) return false;
            glAttachShader(m_programId, geometryId);
        }

        // create fragment shader
        if (!m_fragShaderSources.empty())
        {
            GLuint fragmentId = compileShader(GL_FRAGMENT_SHADER, m_fragShaderSources);
            AutoDeleteShaderObj fragmentShaderObj(fragmentId);
            if (fragmentId == 0) return false;
            glAttachShader(m_programId, fragmentId);
        }

        // link program
        glLinkProgram(m_programId);
        if (getProgramErrorLog(m_programId))
        {
            LOGE("Link program failed with error log {0}", m_errorLog);
            return false;
        }

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

        // TODO: optimize it by cache the operation, do not perform it everytime
        int loc = glGetUniformLocation(m_programId, pszName);
        GL_ERROR_CHECK();
        if (loc == -1) return false;

        glUniform1i(loc, slotIndex);

        glActiveTexture(GL_TEXTURE0 + slotIndex);
        glBindTexture(GL_TEXTURE_2D, pTexture->getTextureId());
        GL_ERROR_CHECK();

        return true;
    }

    void Shader::release()
    {
        if (s_instanceManager.removeInstance(this))
        {
            delete this;
        }
    }

    Shader::Shader()
    {

    }

    Shader::~Shader()
    {
        destroyProgram();
    }

    tstring Shader::makeId(StockShaders stockShader, uint32 preprocessDefine)
    {
        static const tstring s_stockShaderIds[static_cast<int>(StockShaders::NumberOfStockShaders)] =
        {
            "stock::Phong",
            "stock::Wireframe",
        };

        return fmt::format("{0}_{1}", s_stockShaderIds[static_cast<int>(stockShader)], preprocessDefine);
    }

    tstring Shader::makePreprocessDefine(uint32 preprocessDefine)
    {
        tstring defineStr = "#version 330 core\n";

        if (preprocessDefine & USE_DIFFUSE_TEXTURE) defineStr += "#define USE_DIFFUSE_TEXTURE\n";

        return defineStr;
    }

    void Shader::concatShaderSources(StringList & shaderSources, uint32 preprocessDefine, const char* shaderSource)
    {
        if (!shaderSource) return;

        tstring defineStr = makePreprocessDefine(preprocessDefine);
        shaderSources.push_back(defineStr);
        shaderSources.push_back(shaderSource);
    }

    void Shader::destroyProgram()
    {
        if (m_programId != 0)
        {
            glDeleteProgram(m_programId);
            m_programId = 0;
        }
    }

    uint32 Shader::compileShader(uint32 shaderType, const StringList& shaderSources)
    {
        if (shaderSources.empty()) return 0;

        int shaderId = glCreateShader(shaderType);
        if (shaderId == 0) return 0;

        std::vector<const char*> shaderSourcesPtr;
        for (const auto& source : shaderSources)
        {
            if (!source.empty())
            {
                shaderSourcesPtr.push_back(source.c_str());
            }
        }

        glShaderSource(shaderId, shaderSourcesPtr.size(), shaderSourcesPtr.data(), nullptr);
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
