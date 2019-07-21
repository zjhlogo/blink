#pragma once
#include <BaseType.h>
#include <memory>

namespace blink
{
    class BufferAttributes
    {
    public:
        static const int MAX_ATTR_SIZE = 128;
        static const int MAX_ATTRIBUTE_ITEMS = 8;

        enum class AttributeItemType
        {
            Unknown = 0,
            Byte,
            UnsignedByte,
            Short,
            UnsignedShort,
            Float,
            NumberOfAttributeItemType,
        };

        class AttributeItem
        {
        public:
            uint32 m_size;
            AttributeItemType m_attrType;
            uint32 m_glType;
            uint32 m_offset;
            tstring m_name;
        };

        enum class StockAttributes
        {
            Pos3 = 0,
            Pos3Color,
            Pos3Uv2,
            Pos3Normal,

            Pos3Uv2Normal,
            Pos3Uv2Color,
            Pos3Uv2NormalTangent,

            NumberOfStockAttributes,
        };

    public:
        BufferAttributes() {};
        virtual ~BufferAttributes() {};

        inline uint32 getStride() const { return m_attributeItems[m_numItems].m_offset; };
        inline int getNumAttributeItems() const { return m_numItems; };

        const AttributeItem* getAttributeItem(int nIndex) const;
        const AttributeItem* getAttributeItemByName(const tstring& name) const;
        bool isEqual(const std::shared_ptr<BufferAttributes> pVertexAttrs) const;

        static std::shared_ptr<BufferAttributes> fromFile(const tstring& filePath);
        static std::shared_ptr<BufferAttributes> fromStock(StockAttributes stockAttrs);
        static std::shared_ptr<BufferAttributes> fromAttributeItems(const tstring& id, const AttributeItem* pAttrItems);

    private:
        static uint32 getGlType(AttributeItemType eType);
        static uint32 getAttributeItemSize(uint32 nSize, AttributeItemType eType);
        static AttributeItemType getAttributeItemType(const tstring& strType);

    private:
        int m_numItems{};
        AttributeItem m_attributeItems[MAX_ATTRIBUTE_ITEMS + 1];

    };

    // pre-define vertex attributes
    class VertexPos3
    {
    public:
        float x, y, z;      // pos3
    };

    class VertexPos3Color
    {
    public:
        float x, y, z;      // pos3
        uint32 color;       // color
    };

    class VertexPos3Uv2
    {
    public:
        float x, y, z;      // pos3
        float u, v;         // uv2
    };

    class VertexPos3Uv2Normal
    {
    public:
        float x, y, z;      // pos3
        float u, v;         // uv2
        float nx, ny, nz;   // normal
    };

    class VertexPos3Uv2Color
    {
    public:
        float x, y, z;      // pos3
        float u, v;         // uv2
        uint32 color;       // color
    };

    class VertexPos3Uv2NormalTangent
    {
    public:
        float x, y, z;      // pos3
        float u, v;         // uv2
        float nx, ny, nz;   // normal
        float tx, ty, tz;   // tangent
    };
}
