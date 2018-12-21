#pragma once
#include "BufferAttributes.h"

namespace blink
{
    class GeometryUtils
    {
    public:
        static void buildPlane(VertAttrPos3Uv2NormalList& verts, Uint16List& indis, int ax, int ay, int az, float width, float height, float udir, float vdir, float posZ, int segmentX, int segmentY, const glm::vec3& normal);

    };
}
