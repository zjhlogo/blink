#include "GeometryUtils.h"

namespace blink
{
    void GeometryUtils::buildPlaneFaceIndex(std::vector<uint16> & indis, uint16 startIndex, int segmentX, int segmentY)
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
                indis.push_back(startIndex + i2);
                indis.push_back(startIndex + i1);

                indis.push_back(startIndex + i1);
                indis.push_back(startIndex + i2);
                indis.push_back(startIndex + i3);
            }
        }
    }
}
