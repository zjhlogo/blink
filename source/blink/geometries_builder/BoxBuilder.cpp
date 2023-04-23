/*********************************************************************
 * \file   BoxBuilder.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   February 2023
 *********************************************************************/
#include "BoxBuilder.h"
#include "PlaneBuilder.h"

#include <core/modules/IResourceModule.h>
#include <foundation/BuiltinFormatter.h>

namespace blink
{
    BoxBuilder& BoxBuilder::size(float width, float height, float depth)
    {
        m_size.x = width;
        m_size.y = height;
        m_size.z = depth;
        return *this;
    }

    BoxBuilder& BoxBuilder::size(const glm::vec3& sz)
    {
        return size(sz.x, sz.y, sz.z);
    }

    BoxBuilder& BoxBuilder::segment(uint16_t widthSegment, uint16_t heightSegment, uint16_t depthSegment)
    {
        m_segment.x = widthSegment < 1 ? 1 : widthSegment;
        m_segment.y = heightSegment < 1 ? 1 : heightSegment;
        m_segment.z = depthSegment < 1 ? 1 : depthSegment;
        return *this;
    }

    tstring BoxBuilder::getUniqueId() const
    {
        //
        return fmt::format("box_{0}_{1}", m_size, m_segment);
    }

    IGeometry* BoxBuilder::build(bool buildNormal, bool buildUv, glm::mat3* inertiaTensorOut) const
    {
        std::vector<glm::vec3> vertsPos;
        std::vector<uint16_t> indices;
        std::vector<glm::vec3> vertsNormal;
        std::vector<glm::vec2> vertsUv0;

        std::vector<glm::vec3>* pVertsNormal = nullptr;
        std::vector<glm::vec2>* pVertsUv0 = nullptr;
        if (buildNormal)
        {
            pVertsNormal = &vertsNormal;
        }
        if (buildUv)
        {
            pVertsUv0 = &vertsUv0;
        }

        PlaneBuilder builder;

        // top
        builder.size(m_size.x, m_size.z)
            .segment(m_segment.x, m_segment.z)
            .translate(glm::vec3(0.0f, 0.5f * m_size.y, 0.0f))
            .generateData(vertsPos, indices, pVertsNormal, pVertsUv0);

        // bottom
        builder.size(m_size.x, m_size.z)
            .segment(m_segment.x, m_segment.z)
            .orient(glm::angleAxis(glm::pi<float>(), glm::right()))
            .translate(glm::vec3(0.0f, -0.5f * m_size.y, 0.0f))
            .generateData(vertsPos, indices, pVertsNormal, pVertsUv0);

        // front
        builder.size(m_size.x, m_size.y)
            .segment(m_segment.x, m_segment.y)
            .orient(glm::angleAxis(glm::half_pi<float>(), glm::right()))
            .translate(glm::vec3(0.0f, 0.0f, 0.5f * m_size.z))
            .generateData(vertsPos, indices, pVertsNormal, pVertsUv0);

        // back
        builder.size(m_size.x, m_size.y)
            .segment(m_segment.x, m_segment.y)
            .orient(glm::angleAxis(-glm::half_pi<float>(), glm::right()))
            .translate(glm::vec3(0.0f, 0.0f, -0.5f * m_size.z))
            .generateData(vertsPos, indices, pVertsNormal, pVertsUv0);

        // left
        builder.size(m_size.y, m_size.z)
            .segment(m_segment.y, m_segment.z)
            .orient(glm::angleAxis(glm::half_pi<float>(), -glm::forward()))
            .translate(glm::vec3(-0.5f * m_size.x, 0.0f, 0.0f))
            .generateData(vertsPos, indices, pVertsNormal, pVertsUv0);

        // right
        builder.size(m_size.y, m_size.z)
            .segment(m_segment.y, m_segment.z)
            .orient(glm::angleAxis(-glm::half_pi<float>(), -glm::forward()))
            .translate(glm::vec3(0.5f * m_size.x, 0.0f, 0.0f))
            .generateData(vertsPos, indices, pVertsNormal, pVertsUv0);

        auto geometry = getResourceModule()->createGeometry(getUniqueId(), PrimitiveTopology::TriangleList);
        if (!geometry->uploadData(indices, vertsPos, vertsNormal, vertsUv0))
        {
            SAFE_RELEASE(geometry);
            return nullptr;
        }

        if (inertiaTensorOut)
        {
            *inertiaTensorOut = CalculateInertiaTensor(vertsPos.data(), vertsPos.size());
        }

        return geometry;
    }
} // namespace blink
