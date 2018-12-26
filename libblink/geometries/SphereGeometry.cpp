#include "SphereGeometry.h"
#include "BufferAttributes.h"
#include <glm/gtc/constants.hpp>

namespace blink
{
    SphereGeometry::SphereGeometry(float radius, int rings, int sections)
    {
        if (rings < 3) rings = 3;
        if (sections < 3) sections = 3;

        m_radius = radius;
        m_rings = rings;
        m_sections = sections;

        VertAttrPos3Uv2NormalList verts;
        Uint16List indis;

        float const R = 1.0f / (float)(rings - 1);
        float const S = 1.0f / (float)(sections - 1);

        // case rings 0
        verts.push_back({ 0.0f, -radius, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f });

        for (int r = 0; r < rings; ++r)
        {
            float const y = sin(-glm::half_pi<float>() + glm::pi<float>() * r * R);
            for (int s = 0; s < sections; ++s)
            {
                float const x = cos(glm::two_pi<float>() * s * S) * sin(glm::pi<float>() * r * R);
                float const z = sin(glm::two_pi<float>() * s * S) * sin(glm::pi<float>() * r * R);

                verts.push_back({ x*radius, y*radius, z*radius, s * S, r * R, x, y, z});
            }
        }

        // case rings - 1
        verts.push_back({ 0.0f, radius, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f });

        // case rings 0
        {
            for (int s = 0; s < sections - 1; ++s)
            {
                indis.push_back(s + 1);
                indis.push_back(s + 2);
                indis.push_back(s + 0);
            }

            indis.push_back(sections);
            indis.push_back(1);
            indis.push_back(0);
        }

        // cases in the middle of sections
        for (int r = 1; r < rings - 1; ++r)
        {
            int prevRingIndex = 1 + (r - 1) * sections;
            int currRingIndex = prevRingIndex + sections;

            for (int s = 0; s < sections - 1; ++s)
            {
                indis.push_back(prevRingIndex + s);
                indis.push_back(currRingIndex + s);
                indis.push_back(currRingIndex + s + 1);

                indis.push_back(prevRingIndex + s);
                indis.push_back(currRingIndex + s + 1);
                indis.push_back(prevRingIndex + s + 1);
            }

            indis.push_back(currRingIndex + sections - 1);
            indis.push_back(currRingIndex);
            indis.push_back(prevRingIndex + sections - 1);

            indis.push_back(prevRingIndex + sections - 1);
            indis.push_back(currRingIndex);
            indis.push_back(prevRingIndex);
        }

        // case rings - 1
        {
            int prevRingIndex = 1 + (rings - 2) * sections;
            int currRingIndex = prevRingIndex + sections + 1;

            for (int s = 0; s < sections - 1; ++s)
            {
                indis.push_back(prevRingIndex + s + 1);
                indis.push_back(prevRingIndex + s);
                indis.push_back(currRingIndex);
            }

            indis.push_back(currRingIndex);
            indis.push_back(prevRingIndex);
            indis.push_back(prevRingIndex + sections - 1);
        }

        uploadVertexBuffer(BufferAttributes::fromStock(BufferAttributes::StockAttributes::Pos3Uv2Normal)
            , verts.data()
            , sizeof(verts[0])*verts.size());

        uploadIndexBuffer(indis.data(), indis.size());
    }

    SphereGeometry::~SphereGeometry()
    {

    }
}
