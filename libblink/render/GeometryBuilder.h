#pragma once
#include "RenderData.h"

namespace blink
{
    class GeometryBuilder
    {
    public:
        static void buildBox(BufferGeometry& bufferGeo, float width, float height, float depth, int widthSegments = 1, int heightSegments = 1, int depthSegments = 1);

    };
}
