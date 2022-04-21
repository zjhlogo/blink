
/*********************************************************************
 * \file   IGeometry.h
 * \brief
 *
 * \author zjhlogo
 * \date   04/21/2022
 *********************************************************************/
#pragma once

#include "IGeometry.h"

#include <foundation/BaseTypesGlm.h>

#include <vector>

namespace blink
{
    class IGeometryTriangleList : public IGeometry
    {
        friend class IResourceModule;

    public:
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
    };
} // namespace blink
