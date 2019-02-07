#pragma once
#include <BaseType.h>

struct MapData
{
    enum BitFlag
    {
        BF_DIRTY = 1 << 0,
    };

    explicit MapData(int w, int h, blink::uint8* data)
        :width(w)
        , height(h)
        , bitFlag(BF_DIRTY)
    {
        buffer.resize(w*h);
        std::memcpy(buffer.data(), data, w*h);
    }

    int width;
    int height;
    blink::BufferData buffer;
    blink::uint32 bitFlag;
};

struct MapDataUpdateEvent
{
    explicit MapDataUpdateEvent(const MapData* _mapData)
        : mapData(_mapData)
    {

    };

    const MapData* mapData;
};

struct MapRenderData
{

};
