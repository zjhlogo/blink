#pragma once
#include <BaseType.h>

namespace blink
{
    class RenderModule;
    class VertexAttributes;

    class RenderBuffer
    {
        friend class RenderModule;

    public:
        virtual bool uploadBuffer(const void* bufferData, uint32 bufferSize) = 0;
        virtual bool reload(bool freeOld);

    protected:
        RenderBuffer(RenderModule* pRenderModule);
        virtual ~RenderBuffer();

    protected:
        RenderModule* m_pRenderModule{};

    };

    class MemVertexBuffer : public RenderBuffer
    {
        friend class RenderModule;

    public:
        virtual bool uploadBuffer(const void* bufferData, uint32 bufferSize) override;

        const void* getBufferMemAddr() const { return m_bufferData.data(); };
        uint32 getBufferSize() const { return m_bufferData.size(); };

        const VertexAttributes* getVertexAttributes() const { return m_pVertAttrs; };
        uint32 getStride() const;

    protected:
        MemVertexBuffer(RenderModule* pRenderModule, const VertexAttributes* pVertAttrs);
        virtual ~MemVertexBuffer();

    protected:
        const VertexAttributes* m_pVertAttrs{};
        BufferData m_bufferData;

    };

    class VMemVertexBuffer : public MemVertexBuffer
    {
        friend class RenderModule;

    public:
        virtual bool uploadBuffer(const void* bufferData, uint32 bufferSize) override;
        virtual bool reload(bool freeOld) override;

        uint32 getBufferId() const { return m_bufferId; };

    protected:
        VMemVertexBuffer(RenderModule* pRenderModule, const VertexAttributes* pVertAttrs);
        virtual ~VMemVertexBuffer();

    private:
        void destroyVBuffer();
        bool updateVBufferData();

    private:
        uint32 m_bufferId{};

    };

    class MemIndexBuffer : public RenderBuffer
    {
        friend class RenderModule;

    public:
        virtual bool uploadBuffer(const void* bufferData, uint32 bufferSize) override;

        const void* getBufferMemAddr() const { return m_bufferData.data(); };
        uint32 getBufferSize() const { return m_bufferData.size(); };
        uint32 getStride() const { return sizeof(uint16); };

    protected:
        MemIndexBuffer(RenderModule* pRenderModule);
        virtual ~MemIndexBuffer();

    protected:
        BufferData m_bufferData;

    };

    class VMemIndexBuffer : public MemIndexBuffer
    {
        friend class RenderModule;

    public:
        virtual bool uploadBuffer(const void* bufferData, uint32 bufferSize) override;
        virtual bool reload(bool freeOld) override;

        uint32 getBufferId() const { return m_bufferId; };

    protected:
        VMemIndexBuffer(RenderModule* pRenderModule);
        virtual ~VMemIndexBuffer();

    private:
        void destroyVBuffer();
        bool updateVBufferData();

    private:
        uint32 m_bufferId{};

    };
}
