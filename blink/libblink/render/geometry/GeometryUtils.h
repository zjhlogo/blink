#pragma once
#include "BufferAttributes.h"
#include <BaseTypesGlm.h>
#include <vector>
#include <glm/gtc/constants.hpp>

namespace blink
{
    class GeometryUtils
    {
    public:
        template <typename T> static int buildPlaneVertexPos3(std::vector<T>& verts, int ax, int ay, int az, float width, float height, float udir, float vdir, float posZ, int segmentX, int segmentY)
        {
            float divX = static_cast<float>(segmentX);
            float divY = static_cast<float>(segmentY);

            glm::vec2 uvTemp;
            glm::vec3 vertTemp;
            vertTemp[az] = posZ;

            // build vertex list
            int startIndex = static_cast<int>(verts.size());
            verts.resize(startIndex + (segmentX + 1) * (segmentY + 1));
            int currIndex = startIndex;

            for (int y = 0; y < (segmentY + 1); ++y)
            {
                uvTemp.y = y / divY - 0.5f;
                vertTemp[ay] = uvTemp.y * height * vdir;
                for (int x = 0; x < (segmentX + 1); ++x)
                {
                    uvTemp.x = x / divX - 0.5f;
                    vertTemp[ax] = uvTemp.x * width * udir;

                    T& vertex = verts[currIndex++];

                    vertex.x = vertTemp.x;
                    vertex.y = vertTemp.y;
                    vertex.z = vertTemp.z;
                }
            }

            return currIndex - startIndex;
        }

        template <typename T> static int buildPlaneVertexPos3Uv2(std::vector<T>& verts, int ax, int ay, int az, float width, float height, float udir, float vdir, float posZ, int segmentX, int segmentY)
        {
            float divX = static_cast<float>(segmentX);
            float divY = static_cast<float>(segmentY);

            glm::vec2 uvTemp;
            glm::vec3 vertTemp;
            vertTemp[az] = posZ;

            // build vertex list
            int startIndex = static_cast<int>(verts.size());
            verts.resize(startIndex + (segmentX + 1) * (segmentY + 1));
            int currIndex = startIndex;

            for (int y = 0; y < (segmentY + 1); ++y)
            {
                uvTemp.y = y / divY - 0.5f;
                vertTemp[ay] = uvTemp.y * height * vdir;
                for (int x = 0; x < (segmentX + 1); ++x)
                {
                    uvTemp.x = x / divX - 0.5f;
                    vertTemp[ax] = uvTemp.x * width * udir;

                    T& vertex = verts[currIndex++];

                    vertex.x = vertTemp.x;
                    vertex.y = vertTemp.y;
                    vertex.z = vertTemp.z;

                    vertex.u = uvTemp.x + 0.5f;
                    vertex.v = uvTemp.y + 0.5f;
                }
            }

            return currIndex - startIndex;
        }

        template <typename T> static int buildSphereVertexPos3Normal(std::vector<T>& verts, float radius, int rings, int sections)
        {
            // build vertex list
            int startIndex = static_cast<int>(verts.size());
            verts.resize(startIndex + rings * (sections + 1));
            int currIndex = startIndex;

            float const R = 1.0f / (rings - 1);
            float const S = 1.0f / sections;

            // calculate the vertex position
            for (int r = 0; r < rings; ++r)
            {
                float const y = sin(-glm::half_pi<float>() + glm::pi<float>() * r * R);
                for (int s = 0; s < sections + 1; ++s)
                {
                    float const x = cos(glm::two_pi<float>() * s * S) * sin(glm::pi<float>() * r * R);
                    float const z = sin(glm::two_pi<float>() * s * S) * sin(glm::pi<float>() * r * R);

                    T& vertex = verts[currIndex++];

                    vertex.x = x * radius;
                    vertex.y = y * radius;
                    vertex.z = z * radius;

                    vertex.nx = x;
                    vertex.ny = y;
                    vertex.nz = z;
                }
            }

            return currIndex - startIndex;
        }

        template <typename T> static int buildSphereVertexPos3Uv2Normal(std::vector<T>& verts, float radius, int rings, int sections)
        {
            // build vertex list
            int startIndex = static_cast<int>(verts.size());
            verts.resize(startIndex + rings * (sections + 1));
            int currIndex = startIndex;

            float const R = 1.0f / (rings - 1);
            float const S = 1.0f / sections;

            // calculate the vertex position
            for (int r = 0; r < rings; ++r)
            {
                float const y = sin(-glm::half_pi<float>() + glm::pi<float>() * r * R);
                for (int s = 0; s < sections + 1; ++s)
                {
                    float const x = cos(glm::two_pi<float>() * s * S) * sin(glm::pi<float>() * r * R);
                    float const z = sin(glm::two_pi<float>() * s * S) * sin(glm::pi<float>() * r * R);

                    T& vertex = verts[currIndex++];

                    vertex.x = x * radius;
                    vertex.y = y * radius;
                    vertex.z = z * radius;

                    vertex.u = s * S;
                    vertex.v = r * R;

                    vertex.nx = x;
                    vertex.ny = y;
                    vertex.nz = z;
                }
            }

            return currIndex - startIndex;
        }

        template <typename T> static void buildColor(std::vector<T>& verts, int startIndex, int count, uint32 color)
        {
            for (int i = 0; i < count; ++i)
            {
                T& vertex = verts[startIndex + i];
                vertex.color = color;
            }
        }

        template <typename T> static void buildNormal(std::vector<T>& verts, int startIndex, int count, const glm::vec3& normal)
        {
            for (int i = 0; i < count; ++i)
            {
                T& vertex = verts[startIndex + i];
                vertex.nx = normal.x;
                vertex.ny = normal.y;
                vertex.nz = normal.z;
            }
        }

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

        static void buildPlaneFaceIndex(std::vector<uint16>& indis, uint16 startIndex, int segmentX, int segmentY);
        static void buildSphereFaceIndex(std::vector<uint16>& indis, int rings, int sections);

    };
}
