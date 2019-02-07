#pragma once
#include <BaseType.h>

namespace blink
{
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
        static Shader* fromStock(StockShaders stockShader, uint32 preprocessDefine);
        static Shader* fromBuffer(const tstring& id, const char* vsBuffer, const char* gsBuffer, const char* fsBuffer);

        bool reload();
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

        uint32 getProgramId() const { return m_programId; };

        void release();

    private:
        Shader();
        ~Shader();

        static tstring makeId(StockShaders stockShader, uint32 preprocessDefine);
        static tstring makePreprocessDefine(uint32 preprocessDefine);
        static void concatShaderSources(StringList& shaderSources, uint32 preprocessDefine, const char* shaderSource);

        void destroyProgram();
        uint32 compileShader(uint32 shaderType, const StringList& shaderSources);
        bool getShaderErrorLog(uint32 shaderId);
        bool getProgramErrorLog(uint32 programId);

    private:
        uint32 m_programId{};

        StringList m_vertexShaderSources;
        StringList m_geometryShaderSources;
        StringList m_fragShaderSources;

        tstring m_errorLog;

    };
}
