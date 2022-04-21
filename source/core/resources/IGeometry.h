
/*********************************************************************
 * \file   IGeometry.h
 * \brief
 *
 * \author zjhlogo
 * \date   04/21/2022
 *********************************************************************/
#pragma once

#include "IResource.h"

#include <core/base/Color.h>
#include <foundation/BaseTypesGlm.h>

#include <vector>

namespace blink
{
    enum class PrimitiveTopology
    {
        LineList,
        TriangleList
    };

    class IGeometry : public IResource
    {
        friend class IResModule;

    public:
        virtual void release() override;

        virtual bool uploadData(const std::vector<uint16>& indices,
                                const std::vector<glm::vec3>& positions,
                                const std::vector<Color>& colors) = 0;

        virtual bool uploadData(const std::vector<uint16>& indices,
                                const std::vector<glm::vec3>& positions,
                                const std::vector<glm::vec3>& normals,
                                const std::vector<glm::vec2>& uv0s) = 0;

        virtual bool uploadData(const void* data,
                                std::size_t dataSize,
                                uint32 numVertices,
                                uint32 numIndices,
                                std::size_t offsetPosition,
                                std::size_t offsetNormal,
                                std::size_t offsetUv0,
                                std::size_t offsetIndices) = 0;

        uint32 getNumVertices() const { return m_numVertices; };
        uint32 getNumIndices() const { return m_numIndices; };

        bool checkInputMask(uint32 requireInputMask) const;
        PrimitiveTopology getTopology() const { return m_topology; }

    protected:
        uint32 m_numVertices{};
        uint32 m_numIndices{};

        uint32 m_vertexInputMask{};
        PrimitiveTopology m_topology{PrimitiveTopology::TriangleList};
    };
} // namespace blink
