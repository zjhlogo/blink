/**

    @file      IGeometryBuilder.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      5.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/

#include "IGeometryBuilder.h"

namespace blink
{
    glm::mat3 IGeometryBuilder::CalculateInertiaTensor(const glm::vec3* verts, std::size_t count) const
    {
        glm::mat3 m = glm::zero<glm::mat3>();
        float invM = 1.0f / count;

        for (std::size_t i = 0; i < count; ++i)
        {
            const auto& vert = verts[i];
            float xx = vert.x * vert.x;
            float yy = vert.y * vert.y;
            float zz = vert.z * vert.z;
            float xy = vert.x * vert.y;
            float xz = vert.x * vert.z;
            float yz = vert.y * vert.z;

            m[0][0] += invM * (yy + zz);
            m[0][1] -= invM * xy;
            m[0][2] -= invM * xz;
            m[1][0] -= invM * xy;
            m[1][1] += invM * (xx + zz);
            m[1][2] -= invM * yz;
            m[2][0] -= invM * xz;
            m[2][1] -= invM * yz;
            m[2][2] += invM * (xx + yy);
        }

        return m;
    }

} // namespace blink
