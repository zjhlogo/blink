/**

    @file      IGeometryBuilder.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      4.11.2021
    @copyright © zjhlogo, 2021. All right reserved.

**/
#pragma once

#include "../Geometry.h"

#include <foundation/BaseTypesGlm.h>

#include <vector>

namespace blink
{
    class VulkanLogicalDevice;
    class VulkanCommandPool;
    class Geometry;

    class IGeometryBuilder
    {
    public:
        virtual tstring getUniqueId() const = 0;

        virtual bool build(std::vector<glm::vec3>& positionsOut,
                           std::vector<uint16>& indicesOut,
                           std::vector<glm::vec3>* normalsOut = nullptr,
                           std::vector<glm::vec2>* uvsOut = nullptr) const
        {
            return false;
        };

        template <typename T> static void buildTangent(std::vector<T>& verts, const std::vector<uint16>& indis)
        {
            int numVerts = static_cast<int>(verts.size());
            std::vector<std::vector<glm::vec3>> tangentList;
            tangentList.resize(numVerts);

            int numFaces = static_cast<int>(indis.size() / 3);
            for (int i = 0; i < numFaces; ++i)
            {
                T& vert1 = verts[indis[i * 3 + 0]];
                T& vert2 = verts[indis[i * 3 + 1]];
                T& vert3 = verts[indis[i * 3 + 2]];

                glm::vec3 edge1 = glm::vec3(vert2.x - vert1.x, vert2.y - vert1.y, vert2.z - vert1.z);
                glm::vec3 edge2 = glm::vec3(vert3.x - vert1.x, vert3.y - vert1.y, vert3.z - vert1.z);
                glm::vec2 deltaUV1 = glm::vec2(vert2.u - vert1.u, vert2.v - vert1.v);
                glm::vec2 deltaUV2 = glm::vec2(vert3.u - vert1.u, vert3.v - vert1.v);

                float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

                glm::vec3 tangent;
                tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
                tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
                tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
                tangent = glm::normalize(tangent);

                tangentList[indis[i * 3 + 0]].push_back(tangent);
                tangentList[indis[i * 3 + 1]].push_back(tangent);
                tangentList[indis[i * 3 + 2]].push_back(tangent);
            }

            for (int i = 0; i < numVerts; ++i)
            {
                if (!tangentList[i].empty())
                {
                    glm::vec3 tangent{};
                    for (const auto& t : tangentList[i])
                    {
                        tangent += t;
                    }
                    tangent = glm::normalize(tangent);

                    verts[i].tx = tangent.x;
                    verts[i].ty = tangent.y;
                    verts[i].tz = tangent.z;
                }
            }
        }

        template <typename T> static void buildBinormal(std::vector<T>& verts, const std::vector<uint16>& indis)
        {
            int numVerts = static_cast<int>(verts.size());
            std::vector<std::vector<glm::vec3>> binormalList;
            binormalList.resize(numVerts);

            int numFaces = indis.size() / 3;
            for (int i = 0; i < numFaces; ++i)
            {
                T& vert1 = verts[indis[i * 3 + 0]];
                T& vert2 = verts[indis[i * 3 + 1]];
                T& vert3 = verts[indis[i * 3 + 2]];

                glm::vec3 edge1 = glm::vec3(vert2.x - vert1.x, vert2.y - vert1.y, vert2.z - vert1.z);
                glm::vec3 edge2 = glm::vec3(vert3.x - vert1.x, vert3.y - vert1.y, vert3.z - vert1.z);
                glm::vec2 deltaUV1 = glm::vec2(vert2.u - vert1.u, vert2.v - vert1.v);
                glm::vec2 deltaUV2 = glm::vec2(vert3.u - vert1.u, vert3.v - vert1.v);

                float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

                glm::vec3 binormal;
                binormal.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
                binormal.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
                binormal.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
                binormal = glm::normalize(binormal);

                binormalList[indis[i * 3 + 0]].push_back(binormal);
                binormalList[indis[i * 3 + 1]].push_back(binormal);
                binormalList[indis[i * 3 + 2]].push_back(binormal);
            }

            for (int i = 0; i < numVerts; ++i)
            {
                if (!binormalList[i].empty())
                {
                    glm::vec3 binormal{};
                    for (const auto& t : binormalList[i])
                    {
                        binormal += t;
                    }
                    binormal = glm::normalize(binormal);

                    verts[i].bx = binormal.x;
                    verts[i].by = binormal.y;
                    verts[i].bz = binormal.z;
                }
            }
        }
    };
} // namespace blink
