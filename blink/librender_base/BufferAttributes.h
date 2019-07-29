/*!
 * \file BufferAttributes.h
 *
 * \author zjhlogo
 * \date 2019/07/25
 *
 * 
 */
#pragma once
#include <BaseTypes.h>

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
            uint32 size;
            AttributeItemType type;
            uint32 offset;
            tstring name;
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
        BufferAttributes();
        virtual ~BufferAttributes();

        inline uint32 getStride() const { return m_attributeItems[m_numItems].offset; };
        inline int getNumAttributeItems() const { return m_numItems; };

        const AttributeItem* getAttributeItem(int nIndex) const;
        const AttributeItem* getAttributeItemByName(const tstring& name) const;
        bool isEqual(const BufferAttributes* pVertexAttrs) const;

        static BufferAttributes* fromFile(const tstring& filePath);
        static BufferAttributes* fromStock(StockAttributes stockAttrs);
        static BufferAttributes* fromAttributeItems(const tstring& id, const AttributeItem* pAttrItems);

    private:
        static uint32 getAttributeItemSize(uint32 nSize, AttributeItemType eType);
        static AttributeItemType getAttributeItemType(const tstring& strType);

    private:
        int m_numItems{};
        AttributeItem m_attributeItems[MAX_ATTRIBUTE_ITEMS + 1];

    };
}
