/*!
 * \file GeometryUtils.cpp
 *
 * \author zjhlogo
 * \date 2019/11/15
 *
 * 
 */
#include "GeometryUtils.h"

NS_BEGIN

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

void GeometryUtils::buildSphereFaceIndex(std::vector<uint16>& indis, int rings, int sections)
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

NS_END
