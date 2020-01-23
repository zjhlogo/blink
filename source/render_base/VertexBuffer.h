/*!
 * \file VertexBuffer.h
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 *
 */
#pragma once
#include "BufferAttributes.h"
#include "BufferObject.h"

NS_BEGIN

class VertexBuffer
{
public:
    VertexBuffer(const BufferAttributes* attributes);
    virtual ~VertexBuffer();

    const BufferAttributes* getAttributes() { return m_attributes; };

    virtual bool uploadBuffer(const void* buffer, uint32 size, BufferObject::Usage usage) = 0;

protected:
    const BufferAttributes* m_attributes{};
};

NS_END
