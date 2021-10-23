/*!
 * \file BufferAttributes.h
 *
 * \author zjhlogo
 * \date 2019/07/25
 *
 *
 */
#pragma once
#include <foundation/BaseTypes.h>

#include <vector>

NS_BEGIN

class BufferAttributes
{
public:
    static const int MAX_ATTR_SIZE = 128;

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
    };

    using AttributeItemList = std::vector<AttributeItem>;

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

    uint32 getStride() const;
    int getNumAttributeItems() const;

    const AttributeItem* getAttributeItem(int nIndex) const;
    bool isEqual(const BufferAttributes* pVertexAttrs) const;

    static BufferAttributes* fromFile(const tstring& filePath);
    static BufferAttributes* fromStock(StockAttributes stockAttrs);
    static BufferAttributes* fromAttributeItems(const tstring& id, const AttributeItemList& attrItems);

private:
    static uint32 getAttributeItemSize(uint32 nSize, AttributeItemType eType);
    static AttributeItemType getAttributeItemType(const tstring& strType);

private:
    AttributeItemList m_attributeItems;

};

NS_END
