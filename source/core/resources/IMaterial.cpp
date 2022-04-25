
/*********************************************************************
 * \file   IMaterial.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   04/21/2022
 *********************************************************************/
#include "IMaterial.h"
#include "../modules/IResModule.h"

namespace blink
{
    void IMaterial::release()
    {
        //
        getResModule()->releaseMaterial(this);
    }

    bool IMaterial::getInt(int& valueOut, const tstring& name) const
    {
        const auto it = m_uniformMap.find(name);
        if (it == m_uniformMap.end()) return false;

        const auto& variable = it->second;
        if (variable.type != UniformType::Int) return false;

        valueOut = variable.value.iValue;
        return true;
    }

    bool IMaterial::setInt(const tstring& name, int value)
    {
        auto it = m_uniformMap.find(name);
        if (it != m_uniformMap.end())
        {
            auto& variable = it->second;
            if (variable.type != UniformType::Int) return false;

            variable.value.iValue = value;
            return true;
        }

        UniformVariable variable;
        variable.name = name;
        variable.type = UniformType::Int;
        variable.value.iValue = value;
        m_uniformMap.insert({name, variable});

        return true;
    }

    bool IMaterial::getFloat(float& valueOut, const tstring& name) const
    {
        const auto it = m_uniformMap.find(name);
        if (it == m_uniformMap.end()) return false;

        const auto& variable = it->second;
        if (variable.type != UniformType::Float) return false;

        valueOut = variable.value.fValue;
        return true;
    }

    bool IMaterial::setFloat(const tstring& name, float value)
    {
        auto it = m_uniformMap.find(name);
        if (it != m_uniformMap.end())
        {
            auto& variable = it->second;
            if (variable.type != UniformType::Float) return false;

            variable.value.fValue = value;
            return true;
        }

        UniformVariable variable;
        variable.name = name;
        variable.type = UniformType::Float;
        variable.value.fValue = value;
        m_uniformMap.insert({name, variable});

        return true;
    }

    bool IMaterial::getVec2(glm::vec2& valueOut, const tstring& name) const
    {
        const auto it = m_uniformMap.find(name);
        if (it == m_uniformMap.end()) return false;

        const auto& variable = it->second;
        if (variable.type != UniformType::Vec2) return false;

        valueOut = variable.value.vec2Value;
        return true;
    }

    bool IMaterial::setVec2(const tstring& name, const glm::vec2& value)
    {
        auto it = m_uniformMap.find(name);
        if (it != m_uniformMap.end())
        {
            auto& variable = it->second;
            if (variable.type != UniformType::Vec2) return false;

            variable.value.vec2Value = value;
            return true;
        }

        UniformVariable variable;
        variable.name = name;
        variable.type = UniformType::Vec2;
        variable.value.vec2Value = value;
        m_uniformMap.insert({name, variable});

        return true;
    }

    bool IMaterial::getVec3(glm::vec3& valueOut, const tstring& name) const
    {
        const auto it = m_uniformMap.find(name);
        if (it == m_uniformMap.end()) return false;

        const auto& variable = it->second;
        if (variable.type != UniformType::Vec3) return false;

        valueOut = variable.value.vec3Value;
        return true;
    }

    bool IMaterial::setVec3(const tstring& name, const glm::vec3& value)
    {
        auto it = m_uniformMap.find(name);
        if (it != m_uniformMap.end())
        {
            auto& variable = it->second;
            if (variable.type != UniformType::Vec3) return false;

            variable.value.vec3Value = value;
            return true;
        }

        UniformVariable variable;
        variable.name = name;
        variable.type = UniformType::Vec3;
        variable.value.vec3Value = value;
        m_uniformMap.insert({name, variable});

        return true;
    }

    bool IMaterial::getVec4(glm::vec4& valueOut, const tstring& name) const
    {
        const auto it = m_uniformMap.find(name);
        if (it == m_uniformMap.end()) return false;

        const auto& variable = it->second;
        if (variable.type != UniformType::Vec4) return false;

        valueOut = variable.value.vec4Value;
        return true;
    }

    bool IMaterial::setVec4(const tstring& name, const glm::vec4& value)
    {
        auto it = m_uniformMap.find(name);
        if (it != m_uniformMap.end())
        {
            auto& variable = it->second;
            if (variable.type != UniformType::Vec4) return false;

            variable.value.vec4Value = value;
            return true;
        }

        UniformVariable variable;
        variable.name = name;
        variable.type = UniformType::Vec4;
        variable.value.vec4Value = value;
        m_uniformMap.insert({name, variable});

        return true;
    }

    bool IMaterial::getMat3(glm::mat3& valueOut, const tstring& name) const
    {
        const auto it = m_uniformMap.find(name);
        if (it == m_uniformMap.end()) return false;

        const auto& variable = it->second;
        if (variable.type != UniformType::Mat3) return false;

        valueOut = variable.value.mat3Value;
        return true;
    }

    bool IMaterial::setMat3(const tstring& name, const glm::mat3& value)
    {
        auto it = m_uniformMap.find(name);
        if (it != m_uniformMap.end())
        {
            auto& variable = it->second;
            if (variable.type != UniformType::Mat3) return false;

            variable.value.mat3Value = value;
            return true;
        }

        UniformVariable variable;
        variable.name = name;
        variable.type = UniformType::Mat3;
        variable.value.mat3Value = value;
        m_uniformMap.insert({name, variable});

        return true;
    }

    bool IMaterial::getMat4(glm::mat4& valueOut, const tstring& name) const
    {
        const auto it = m_uniformMap.find(name);
        if (it == m_uniformMap.end()) return false;

        const auto& variable = it->second;
        if (variable.type != UniformType::Mat4) return false;

        valueOut = variable.value.mat4Value;
        return true;
    }

    bool IMaterial::setMat4(const tstring& name, const glm::mat4& value)
    {
        auto it = m_uniformMap.find(name);
        if (it != m_uniformMap.end())
        {
            auto& variable = it->second;
            if (variable.type != UniformType::Mat4) return false;

            variable.value.mat4Value = value;
            return true;
        }

        UniformVariable variable;
        variable.name = name;
        variable.type = UniformType::Mat4;
        variable.value.mat4Value = value;
        m_uniformMap.insert({name, variable});

        return true;
    }
} // namespace blink
