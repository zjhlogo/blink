#pragma once
#include <BaseType.h>

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
            Pos3Uv2Normal,
            Pos3Uv2Color,
            NumberOfStockAttributes,
        };

    public:
        inline uint32 getStride() const { return m_attributeItems[m_numItems].m_offset; };
        inline int getNumAttributeItems() const { return m_numItems; };

        const AttributeItem* getAttributeItem(int nIndex) const;
        const AttributeItem* getAttributeItemByName(const tstring& name) const;
        bool isEqual(const BufferAttributes* pVertexAttrs) const;

        static BufferAttributes* fromFile(const tstring& filePath);
        static BufferAttributes* fromStock(StockAttributes stockAttrs);
        static BufferAttributes* fromAttributeItems(const AttributeItem* pAttrItems);

    protected:
        BufferAttributes() {};
        virtual ~BufferAttributes() {};

    private:
        static uint32 getGlType(AttributeItemType eType);
        static uint32 getAttributeItemSize(uint32 nSize, AttributeItemType eType);
        static AttributeItemType getAttributeItemType(const tstring& strType);

    private:
        int m_numItems{};
        AttributeItem m_attributeItems[MAX_ATTRIBUTE_ITEMS + 1];

    };

    // pre-define vertex attributes
    class VertAttrPos3
    {
    public:
        float x, y, z;		// pos3
    };

    typedef std::vector<VertAttrPos3> VertAttrPos3List;

    class VertAttrPos3Color
    {
    public:
        float x, y, z;		// pos3
        float r, g, b, a;	// color
    };

    typedef std::vector<VertAttrPos3Color> VertAttrPos3ColorList;

    class VertAttrPos3Uv2
    {
    public:
        float x, y, z;		// pos3
        float u, v;			// uv2
    };

    typedef std::vector<VertAttrPos3Uv2> VertAttrPos3Uv2List;

    class VertAttrPos3Uv2Normal
    {
    public:
        float x, y, z;		// pos3
        float u, v;			// uv2
        float nx, ny, nz;	// normal
    };

    typedef std::vector<VertAttrPos3Uv2Normal> VertAttrPos3Uv2NormalList;

    class VertAttrPos3Uv2Color
    {
    public:
        float x, y, z;		// pos3
        float u, v;			// uv2
        float r, g, b, a;	// color
    };

    typedef std::vector<VertAttrPos3Uv2Color> VertAttrPos3Uv2ColorList;

    typedef std::vector<uint16> Uint16List;
}
