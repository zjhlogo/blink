/**

    @file      GeometryBuilder.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      2.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#include "GeometryBuilder.h"

namespace blink
{
    int GeometryBuilder::buildPlaneVertexPosUv(std::vector<glm::vec3>& vertsPos,
                                               std::vector<glm::vec2>& vertsUv0,
                                               int ax,
                                               int ay,
                                               int az,
                                               float width,
                                               float height,
                                               float udir,
                                               float vdir,
                                               float posZ,
                                               int segmentX,
                                               int segmentY)
    {
        float divX = static_cast<float>(segmentX);
        float divY = static_cast<float>(segmentY);

        glm::vec2 uvTemp;
        glm::vec3 vertTemp;
        vertTemp[az] = posZ;

        // build vertex list
        int startIndex = static_cast<int>(vertsPos.size());
        vertsPos.resize(startIndex + (segmentX + 1) * (segmentY + 1));
        vertsUv0.resize(startIndex + (segmentX + 1) * (segmentY + 1));
        int currIndex = startIndex;

        for (int y = 0; y < (segmentY + 1); ++y)
        {
            uvTemp.y = y / divY - 0.5f;
            vertTemp[ay] = uvTemp.y * height * vdir;
            for (int x = 0; x < (segmentX + 1); ++x)
            {
                uvTemp.x = x / divX - 0.5f;
                vertTemp[ax] = uvTemp.x * width * udir;

                vertsPos[currIndex] = vertTemp;
                vertsUv0[currIndex] = uvTemp + VEC2_HALF;

                ++currIndex;
            }
        }

        return currIndex - startIndex;
    }

    void GeometryBuilder::buildNormal(std::vector<glm::vec3>& normals, int startIndex, int count, const glm::vec3& normal)
    {
        for (int i = 0; i < count; ++i)
        {
            normals[startIndex + i] = normal;
        }
    }

    void GeometryBuilder::buildPlaneFaceIndex(std::vector<uint16>& indis, uint16 startIndex, int segmentX, int segmentY)
    {
        // build face list
        for (int y = 0; y < segmentY; ++y)
        {
            for (int x = 0; x < segmentX; ++x)
            {
                uint16 i0 = y * (segmentX + 1) + x;
                uint16 i1 = i0 + 1;
                uint16 i2 = (y + 1) * (segmentX + 1) + x;
                uint16 i3 = i2 + 1;

                indis.push_back(startIndex + i0);
                indis.push_back(startIndex + i1);
                indis.push_back(startIndex + i2);

                indis.push_back(startIndex + i1);
                indis.push_back(startIndex + i3);
                indis.push_back(startIndex + i2);
            }
        }
    }

    void GeometryBuilder::buildSphereFaceIndex(std::vector<uint16>& indis, int rings, int sections)
    {
        // setup face index
        for (int r = 0; r < rings; ++r)
        {
            int prevRingIndex = r * (sections + 1);
            int currRingIndex = prevRingIndex + sections + 1;

            for (int s = 0; s < sections; ++s)
            {
                indis.push_back(currRingIndex + s);
                indis.push_back(prevRingIndex + s);
                indis.push_back(currRingIndex + s + 1);

                indis.push_back(currRingIndex + s + 1);
                indis.push_back(prevRingIndex + s);
                indis.push_back(prevRingIndex + s + 1);
            }
        }
    }

} // namespace blink
