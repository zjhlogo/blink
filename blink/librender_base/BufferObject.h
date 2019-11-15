/*!
 * \file BufferObject.h
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 * 
 */
#pragma once
#include <BaseTypes.h>

NS_BEGIN

class BufferObject
{
public:
    enum class BufferType
    {
        ArrayBuffer = 0,
        ElementArrayBuffer,
        NumBufferType,
    };

    enum class Usage
    {
        StreamDraw = 0,
        StreamRead,
        StreamCopy,
        StaticDraw,
        StaticRead,
        StaticCopy,
        DynamicDraw,
        DynamicRead,
        DynamicCopy,
        NumUsage,
    };

public:
    virtual bool uploadBufferData(const void* data, uint32 size, Usage usage) = 0;

protected:
    BufferObject(BufferType bufferType);
    virtual ~BufferObject();

protected:
    BufferType m_bufferType{ BufferType::ElementArrayBuffer };

};

NS_END
