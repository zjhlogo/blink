
/*********************************************************************
 * \file   IMaterial.h
 * \brief
 *
 * \author zjhlogo
 * \date   04/21/2022
 *********************************************************************/
#pragma once

#include "IGeometry.h"
#include "IResource.h"

#include <core/types/UniformTypes.h>
#include <foundation/BaseTypesGlm.h>

#include <unordered_map>

namespace blink
{
    class IMaterial : public IResource
    {
    protected:
        union UniformValue
        {
            int iValue;
            float fValue;
            glm::vec2 vec2Value;
            glm::vec3 vec3Value;
            glm::vec4 vec4Value;
            glm::mat3 mat3Value;
            glm::mat4 mat4Value;
        };

        struct UniformVariable
        {
            tstring name;
            UniformType type;
            UniformValue value;
        };

    public:
        virtual void release() override;
        virtual bool recreate() = 0;

        bool getInt(int& valueOut, const tstring& name) const;
        bool setInt(const tstring& name, int value);

        bool getFloat(float& valueOut, const tstring& name) const;
        bool setFloat(const tstring& name, float value);

        bool getVec2(glm::vec2& valueOut, const tstring& name) const;
        bool setVec2(const tstring& name, const glm::vec2& value);

        bool getVec3(glm::vec3& valueOut, const tstring& name) const;
        bool setVec3(const tstring& name, const glm::vec3& value);

        bool getVec4(glm::vec4& valueOut, const tstring& name) const;
        bool setVec4(const tstring& name, const glm::vec4& value);

        bool getMat3(glm::mat3& valueOut, const tstring& name) const;
        bool setMat3(const tstring& name, const glm::mat3& value);

        bool getMat4(glm::mat4& valueOut, const tstring& name) const;
        bool setMat4(const tstring& name, const glm::mat4& value);

    protected:
        std::unordered_map<tstring, UniformVariable> m_uniformMap;
    };
} // namespace blink
