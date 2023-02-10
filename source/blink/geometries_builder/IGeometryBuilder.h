/**

    @file      IGeometryBuilder.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      4.11.2021
    @copyright © zjhlogo, 2021. All right reserved.

**/
#pragma once

#include <core/resources/IGeometry.h>
#include <foundation/BaseTypesGlm.h>

#include <vector>

namespace blink
{
    class VulkanLogicalDevice;
    class VulkanCommandPool;
    class IGeometry;

    class IGeometryBuilder
    {
    public:
        virtual ~IGeometryBuilder() = default;
        virtual tstring getUniqueId() const = 0;

        template <typename T> static void buildTangent(std::vector<T>& verts, const std::vector<uint16_t>& indies)
        {
            int numVerts = static_cast<int>(verts.size());
            std::vector<std::vector<glm::vec3>> tangentList;
            tangentList.resize(numVerts);

            int numFaces = static_cast<int>(indies.size() / 3);
            for (int i = 0; i < numFaces; ++i)
            {
                T& vert1 = verts[indies[i * 3 + 0]];
                T& vert2 = verts[indies[i * 3 + 1]];
                T& vert3 = verts[indies[i * 3 + 2]];

                auto edge1 = glm::vec3(vert2.x - vert1.x, vert2.y - vert1.y, vert2.z - vert1.z);
                auto edge2 = glm::vec3(vert3.x - vert1.x, vert3.y - vert1.y, vert3.z - vert1.z);
                auto deltaUV1 = glm::vec2(vert2.u - vert1.u, vert2.v - vert1.v);
                auto deltaUV2 = glm::vec2(vert3.u - vert1.u, vert3.v - vert1.v);

                float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

                glm::vec3 tangent;
                tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
                tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
                tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
                tangent = glm::normalize(tangent);

                tangentList[indies[i * 3 + 0]].push_back(tangent);
                tangentList[indies[i * 3 + 1]].push_back(tangent);
                tangentList[indies[i * 3 + 2]].push_back(tangent);
            }

            for (int i = 0; i < numVerts; ++i)
            {
                if (!tangentList[i].empty())
                {
                    glm::vec3 tangent{};
                    for (const auto& t : tangentList[i]) { tangent += t; }
                    tangent = glm::normalize(tangent);

                    verts[i].tx = tangent.x;
                    verts[i].ty = tangent.y;
                    verts[i].tz = tangent.z;
                }
            }
        }

        template <typename T> static void buildBiNormal(std::vector<T>& verts, const std::vector<uint16_t>& indies)
        {
            int numVerts = static_cast<int>(verts.size());
            std::vector<std::vector<glm::vec3>> biNormalList;
            biNormalList.resize(numVerts);

            int numFaces = indies.size() / 3;
            for (int i = 0; i < numFaces; ++i)
            {
                T& vert1 = verts[indies[i * 3 + 0]];
                T& vert2 = verts[indies[i * 3 + 1]];
                T& vert3 = verts[indies[i * 3 + 2]];

                auto edge1 = glm::vec3(vert2.x - vert1.x, vert2.y - vert1.y, vert2.z - vert1.z);
                auto edge2 = glm::vec3(vert3.x - vert1.x, vert3.y - vert1.y, vert3.z - vert1.z);
                auto deltaUv1 = glm::vec2(vert2.u - vert1.u, vert2.v - vert1.v);
                auto deltaUv2 = glm::vec2(vert3.u - vert1.u, vert3.v - vert1.v);

                float f = 1.0f / (deltaUv1.x * deltaUv2.y - deltaUv2.x * deltaUv1.y);

                glm::vec3 biNormal;
                biNormal.x = f * (-deltaUv2.x * edge1.x + deltaUv1.x * edge2.x);
                biNormal.y = f * (-deltaUv2.x * edge1.y + deltaUv1.x * edge2.y);
                biNormal.z = f * (-deltaUv2.x * edge1.z + deltaUv1.x * edge2.z);
                biNormal = glm::normalize(biNormal);

                biNormalList[indies[i * 3 + 0]].push_back(biNormal);
                biNormalList[indies[i * 3 + 1]].push_back(biNormal);
                biNormalList[indies[i * 3 + 2]].push_back(biNormal);
            }

            for (int i = 0; i < numVerts; ++i)
            {
                if (!biNormalList[i].empty())
                {
                    glm::vec3 biNormal{};
                    for (const auto& t : biNormalList[i]) { biNormal += t; }
                    biNormal = glm::normalize(biNormal);

                    verts[i].bx = biNormal.x;
                    verts[i].by = biNormal.y;
                    verts[i].bz = biNormal.z;
                }
            }
        }

    protected:
        glm::mat3 CalculateInertiaTensor(const glm::vec3* verts, std::size_t count) const;
    };
} // namespace blink
