/*********************************************************************
 * \file   IGeometry.h
 * \brief
 *
 * \author zjhlogo
 * \date   04/21/2022
 *********************************************************************/
#pragma once

#include "IResource.h"

#include <core/types/Color.h>
#include <core/types/VertexAttrs.h>

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
        void release() override;

        virtual bool uploadData(const std::vector<uint16_t>& indices,
                                const std::vector<glm::vec3>& positions,
                                const std::vector<Color>& colors) = 0;

        virtual bool uploadData(const std::vector<uint16_t>& indices,
                                const std::vector<glm::vec3>& positions,
                                const std::vector<glm::vec3>& normals,
                                const std::vector<glm::vec2>& uv0s) = 0;

        virtual bool uploadData(const void* data,
                                std::size_t dataSize,
                                uint32_t numVertices,
                                uint32_t numIndices,
                                std::size_t offsetPosition,
                                std::size_t offsetNormal,
                                std::size_t offsetUv0,
                                std::size_t offsetIndices) = 0;

        uint32_t getNumVertices() const { return m_numVertices; }
        uint32_t getNumIndices() const { return m_numIndices; }

        bool hasVertexAttrs(VertexAttrs requiredVertexAttrs) const;
        PrimitiveTopology getTopology() const { return m_topology; }

    protected:
        uint32_t m_numVertices{};
        uint32_t m_numIndices{};

        VertexAttrs m_vertexAttrs{VertexAttrs::None};
        PrimitiveTopology m_topology{PrimitiveTopology::TriangleList};
    };
} // namespace blink
