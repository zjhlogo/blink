#pragma once
#include <BaseTypes.h>
#include <BaseTypesGlm.h>
#include <vector>

struct MapData
{
    enum BitFlag
    {
        BF_DIRTY = 1 << 0,
    };

    explicit MapData(int w, int h, NS::uint8* data)
        :size(w, h)
        , bitFlag(BF_DIRTY)
    {
        buffer.resize(w*h);
        std::memcpy(buffer.data(), data, w*h);
    }

    glm::ivec2 size;
    glm::ivec2 origin;
    std::vector<NS::uint8> buffer;
    NS::uint32 bitFlag;
};

struct MapDataUpdateEvent
{
    explicit MapDataUpdateEvent(const MapData* _mapData)
        : mapData(_mapData)
    {

    };

    const MapData* mapData;
};
