/*!
 * \file OpenGLShader.cpp
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 * 
 */
#include "OpenGLShader.h"
#include "Texture.h"
#include "shader_lib/ShaderLib.h"
#include "GlConfig.h"
#include <fmt/format.h>
#include <glm/gtc/type_ptr.hpp>

NS_BEGIN

class AutoDeleteShaderObj
{
public:
    AutoDeleteShaderObj(GLuint shaderId) :m_shaderId(shaderId) {};
    ~AutoDeleteShaderObj() { if (m_shaderId != 0) glDeleteShader(m_shaderId); };

private:
    GLuint m_shaderId{};

};

OpenGLShader::OpenGLShader(const tstring& id)
    :Shader(id)
{

}

OpenGLShader::~OpenGLShader()
{
    destroyProgram();
}

bool OpenGLShader::reload()
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

bool OpenGLShader::setUniform(const char* pszName, int value)
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

bool OpenGLShader::setUniform(const char* pszName, float value)
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

bool OpenGLShader::setUniform(const char* pszName, const glm::vec2& v)
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

bool OpenGLShader::setUniform(const char* pszName, const glm::vec3& v)
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

bool OpenGLShader::setUniform(const char* pszName, const glm::vec4& v)
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

bool OpenGLShader::setUniform(const char* pszName, const glm::mat3& mat3)
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

bool OpenGLShader::setUniform(const char* pszName, const glm::mat4& mat4)
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

bool OpenGLShader::setUniform(const char* pszName, const std::vector<glm::mat4>& matList)
{
    if (matList.empty()) return false;

    // First gets the location of that variable in the shader using its name
    int loc = glGetUniformLocation(m_programId, pszName);
    GL_ERROR_CHECK();
    if (loc == -1) return false;

    const float* pValue = glm::value_ptr(matList[0]);

    // Then passes the matrix to that variable
    glUniformMatrix4fv(loc, static_cast<GLsizei>(matList.size()), GL_FALSE, pValue);
    GL_ERROR_CHECK();

    return true;
}

bool OpenGLShader::setUniform(const char* pszName, int count, const glm::vec3* v)
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

bool OpenGLShader::setUniform(const char* pszName, int count, const glm::vec4* v)
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

bool OpenGLShader::setUniform(const char* pszName, int count, const glm::mat4* v)
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

bool OpenGLShader::setTexture(const char * pszName, Texture* texture, uint32 slotIndex/* = 0 */)
{
    if (!pszName || !texture) return false;

//  // TODO: optimize it by cache the operation, do not perform it everytime
//  int loc = glGetUniformLocation(m_programId, pszName);
//  GL_ERROR_CHECK();
//  if (loc == -1) return false;
// 
//  glUniform1i(loc, slotIndex);
// 
//  glActiveTexture(GL_TEXTURE0 + slotIndex);
//  glBindTexture(GL_TEXTURE_2D, texture->getTextureId());
//  GL_ERROR_CHECK();

    return true;
}

void OpenGLShader::destroyProgram()
{
    if (m_programId != 0)
    {
        glDeleteProgram(m_programId);
        m_programId = 0;
    }
}

GLuint OpenGLShader::compileShader(uint32 shaderType, const std::vector<tstring>& shaderSources)
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

    glShaderSource(shaderId, static_cast<GLsizei>(shaderSourcesPtr.size()), shaderSourcesPtr.data(), nullptr);
    glCompileShader(shaderId);

    if (getShaderErrorLog(shaderId))
    {
        LOGE("Compile shader failed with error log {0}", m_errorLog);
        glDeleteShader(shaderId);
        return 0;
    }

    return shaderId;
}

bool OpenGLShader::getShaderErrorLog(GLuint shaderId)
{
    GLint compileStatus = 0;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus == GL_TRUE) return false;

    GLint infoLength = 0;
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLength);
    if (infoLength <= 1) return false;

    std::vector<uint8> buffer;
    buffer.resize(infoLength);

    GLsizei charWritten = 0;
    glGetShaderInfoLog(shaderId, infoLength, &charWritten, (char*)buffer.data());

    m_errorLog.clear();
    m_errorLog.append((char*)buffer.data(), infoLength);

    return true;
}

bool OpenGLShader::getProgramErrorLog(GLuint programId)
{
    GLint linkStatus = 0;
    glGetProgramiv(programId, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_TRUE) return false;

    GLint infoLength = 0;
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLength);
    if (infoLength <= 1) return false;

    std::vector<uint8> buffer;
    buffer.resize(infoLength);

    GLsizei charWritten = 0;
    glGetProgramInfoLog(programId, infoLength, &charWritten, (char*)buffer.data());

    m_errorLog.clear();
    m_errorLog.append((char*)buffer.data(), infoLength);

    return true;
}

NS_END
