#pragma once
#include <Rtti.h>

namespace blink
{
    class BufferAttributes;

    class RenderBuffer
    {
    public:
        virtual bool uploadBuffer(const void* bufferData, uint32 bufferSize) = 0;
        virtual bool reload(bool freeOld);

    protected:
        RenderBuffer();
        virtual ~RenderBuffer();

    };

    class MemVertexBuffer : public RenderBuffer
    {
    public:
        virtual bool uploadBuffer(const void* bufferData, uint32 bufferSize) override;

        const void* getBufferMemAddr() const { return m_bufferData.data(); };
        uint32 getBufferSize() const { return m_bufferData.size(); };

        const BufferAttributes* getVertexAttributes() const { return m_pVertAttrs; };
        uint32 getStride() const;

    protected:
        MemVertexBuffer(const BufferAttributes* pVertAttrs);
        virtual ~MemVertexBuffer();

    protected:
        const BufferAttributes* m_pVertAttrs{};
        BufferData m_bufferData;

    };

    class VMemVertexBuffer : public MemVertexBuffer
    {
    public:
        virtual bool uploadBuffer(const void* bufferData, uint32 bufferSize) override;
        virtual bool reload(bool freeOld) override;

        uint32 getBufferId() const { return m_bufferId; };

    protected:
        VMemVertexBuffer(const BufferAttributes* pVertAttrs);
        virtual ~VMemVertexBuffer();

    private:
        void destroyVBuffer();
        bool updateVBufferData();

    private:
        uint32 m_bufferId{};

    };

    class MemIndexBuffer : public RenderBuffer
    {
    public:
        virtual bool uploadBuffer(const void* bufferData, uint32 bufferSize) override;

        const void* getBufferMemAddr() const { return m_bufferData.data(); };
        uint32 getBufferSize() const { return m_bufferData.size(); };
        uint32 getStride() const { return sizeof(uint16); };

    protected:
        MemIndexBuffer();
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
        VMemIndexBuffer();
        virtual ~VMemIndexBuffer();

    private:
        void destroyVBuffer();
        bool updateVBufferData();

    private:
        uint32 m_bufferId{};

    };
}
