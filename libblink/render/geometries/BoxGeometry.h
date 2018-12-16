#pragma once
#include "BufferGeometry.h"
#include "BufferAttributes.h"

namespace blink
{
    class BoxGeometry : public BufferGeometry
    {
    public:
        RTTI_DEF(BoxGeometry, BufferGeometry);

        BoxGeometry(float width, float height, float depth, int widthSegments = 1, int heightSegments = 1, int depthSegments = 1);
        virtual ~BoxGeometry();

    private:
        void buildPlane(VertAttrPos3Uv2NormalList& verts, Uint16List& indis, int ax, int ay, int az, float width, float height, float udir, float vdir, float posZ, int segmentX, int segmentY, const glm::vec3& normal);

    private:
        float m_width{};
        float m_height{};
        float m_depth{};
        int m_widthSegments{};
        int m_heightSegments{};
        int m_depthSegments{};

    };
}
