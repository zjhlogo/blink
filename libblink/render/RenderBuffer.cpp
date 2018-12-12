#include "RenderBuffer.h"
#include "VertexAttributes.h"
#include "RenderModule.h"
#include "GlConfig.h"
#include "glad.h"
#include <GLFW/glfw3.h>

namespace blink
{
    RenderBuffer::RenderBuffer(RenderModule* pRenderModule)
        :m_pRenderModule(pRenderModule)
    {

    }

    RenderBuffer::~RenderBuffer()
    {

    }

    bool RenderBuffer::reload(bool freeOld)
    {
        return true;
    }

    MemVertexBuffer::MemVertexBuffer(RenderModule* pRenderModule, const VertexAttributes* pVertAttrs)
        :RenderBuffer(pRenderModule)
        , m_pVertAttrs(pVertAttrs)
    {

    }

    MemVertexBuffer::~MemVertexBuffer()
    {

    }

    bool MemVertexBuffer::uploadBuffer(const void* bufferData, uint32 bufferSize)
    {
        if (!bufferData || bufferSize == 0) return false;

        m_bufferData.resize(bufferSize);
        memcpy(m_bufferData.data(), bufferData, bufferSize);

        return true;
    }

    uint32 MemVertexBuffer::getStride() const
    {
        if (!m_pVertAttrs) return 0;
        return m_pVertAttrs->getStride();
    }

    VMemVertexBuffer::VMemVertexBuffer(RenderModule* pRenderModule, const VertexAttributes* pVertAttrs)
        :MemVertexBuffer(pRenderModule, pVertAttrs)
    {

    }

    VMemVertexBuffer::~VMemVertexBuffer()
    {
        destroyVBuffer();
    }

    bool VMemVertexBuffer::uploadBuffer(const void* bufferData, uint32 bufferSize)
    {
        if (!MemVertexBuffer::uploadBuffer(bufferData, bufferSize)) return false;
        return updateVBufferData();
    }

    bool VMemVertexBuffer::reload(bool freeOld)
    {
        if (freeOld) destroyVBuffer();
        m_bufferId = 0;
        return updateVBufferData();
    }

    void VMemVertexBuffer::destroyVBuffer()
    {
        if (m_bufferId != 0)
        {
            glDeleteBuffers(1, &m_bufferId);
            m_bufferId = 0;
        }
    }

    bool VMemVertexBuffer::updateVBufferData()
    {
        // create buffer object
        if (m_bufferId == 0)
        {
            glGenBuffers(1, &m_bufferId);
            GL_ERROR_CHECK();
            if (m_bufferId == 0) return false;
        }

        // Bind the VBO
        glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
        GL_ERROR_CHECK();

        // Set the buffer's data
        glBufferData(GL_ARRAY_BUFFER, m_bufferData.size(), m_bufferData.data(), GL_STATIC_DRAW);
        GL_ERROR_CHECK();

        // Unbind the VBO
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        return true;
    }

    MemIndexBuffer::MemIndexBuffer(RenderModule* pRenderModule)
        :RenderBuffer(pRenderModule)
    {

    }

    MemIndexBuffer::~MemIndexBuffer()
    {

    }

    bool MemIndexBuffer::uploadBuffer(const void* bufferData, uint32 bufferSize)
    {
        if (!bufferData || bufferSize == 0) return false;

        m_bufferData.resize(bufferSize);
        memcpy(m_bufferData.data(), bufferData, bufferSize);

        return true;
    }

    VMemIndexBuffer::VMemIndexBuffer(RenderModule* pRenderModule)
        :MemIndexBuffer(pRenderModule)
    {

    }

    VMemIndexBuffer::~VMemIndexBuffer()
    {
        destroyVBuffer();
    }

    bool VMemIndexBuffer::uploadBuffer(const void* bufferData, uint32 bufferSize)
    {
        if (!MemIndexBuffer::uploadBuffer(bufferData, bufferSize)) return false;
        return updateVBufferData();
    }

    bool VMemIndexBuffer::reload(bool freeOld)
    {
        if (freeOld) destroyVBuffer();
        m_bufferId = 0;
        return updateVBufferData();
    }

    void VMemIndexBuffer::destroyVBuffer()
    {
        if (m_bufferId != 0)
        {
            glDeleteBuffers(1, &m_bufferId);
            m_bufferId = 0;
        }
    }

    bool VMemIndexBuffer::updateVBufferData()
    {
        // create buffer object
        if (m_bufferId == 0)
        {
            glGenBuffers(1, &m_bufferId);
            GL_ERROR_CHECK();
            if (m_bufferId == 0) return false;
        }

        // Bind the VBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferId);
        GL_ERROR_CHECK();

        // Set the buffer's data
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_bufferData.size(), m_bufferData.data(), GL_STATIC_DRAW);
        GL_ERROR_CHECK();

        // Unbind the VBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        return true;
    }
}
