/*!
 * \file BufferAttributes.cpp
 *
 * \author zjhlogo
 * \date 2019/07/25
 *
 *
 */
#include "BufferAttributes.h"

#include <foundation/File.h>
#include <foundation/JsonSerializer.h>

#include <rttr/registration>
#include <rttr/registration_friend>
#include <rttr/type>

RTTR_REGISTRATION
{
    rttr::registration::enumeration<NS::BufferAttributes::AttributeItemType>(
        "AttributeItemType")(rttr::value("byte", NS::BufferAttributes::AttributeItemType::Byte),
                             rttr::value("unsigned_byte", NS::BufferAttributes::AttributeItemType::UnsignedByte),
                             rttr::value("short", NS::BufferAttributes::AttributeItemType::Short),
                             rttr::value("unsigned_short", NS::BufferAttributes::AttributeItemType::UnsignedShort),
                             rttr::value("float", NS::BufferAttributes::AttributeItemType::Float));

    rttr::registration::class_<NS::BufferAttributes::AttributeItem>("AttributeItem")(rttr::policy::ctor::as_object)
        .property("size", &NS::BufferAttributes::AttributeItem::size)
        .property("type", &NS::BufferAttributes::AttributeItem::type)
        .property("offset", &NS::BufferAttributes::AttributeItem::offset);

    rttr::registration::class_<NS::BufferAttributes>("BufferAttributes").property("attribute_items", &NS::BufferAttributes::m_attributeItems);
}

NS_BEGIN

BufferAttributes::BufferAttributes()
{
}

BufferAttributes::~BufferAttributes()
{
}

uint32 NS::BufferAttributes::getStride() const
{
    if (m_attributeItems.size() <= 0) return 0;

    const auto& item = m_attributeItems[m_attributeItems.size() - 1];
    return item.offset + item.size;
}

int NS::BufferAttributes::getNumAttributeItems() const
{
    return static_cast<int>(m_attributeItems.size());
}

const BufferAttributes::AttributeItem* BufferAttributes::getAttributeItem(int nIndex) const
{
    int numItems = static_cast<int>(m_attributeItems.size());
    if (nIndex < 0 || nIndex >= numItems) return nullptr;
    return &m_attributeItems[nIndex];
}

bool BufferAttributes::isEqual(const BufferAttributes* pVertexAttrs) const
{
    if (!pVertexAttrs) return false;

    int numItems = static_cast<int>(m_attributeItems.size());
    if (numItems != pVertexAttrs->getNumAttributeItems()) return false;

    for (int i = 0; i < numItems; ++i)
    {
        const AttributeItem* pAttrItem = pVertexAttrs->getAttributeItem(i);

        if (m_attributeItems[i].size != pAttrItem->size) return false;
        if (m_attributeItems[i].type != pAttrItem->type) return false;
    }

    return true;
}

uint32 BufferAttributes::getAttributeItemSize(uint32 nSize, AttributeItemType eType)
{
    static const int s_ItemSizes[static_cast<int>(AttributeItemType::NumberOfAttributeItemType)] = {
        0, // AIT_UNKNOWN,
        1, // AIT_BYTE,
        1, // AIT_UNSIGNED_BYTE,
        2, // AIT_SHORT,
        2, // AIT_UNSIGNED_SHORT,
        4, // AIT_FLOAT,
    };

    if (eType < AttributeItemType::Unknown || eType >= AttributeItemType::NumberOfAttributeItemType) return 0;
    return nSize * s_ItemSizes[static_cast<int>(eType)];
}

BufferAttributes::AttributeItemType BufferAttributes::getAttributeItemType(const tstring& strType)
{
    if (strType == "Unknown")
        return AttributeItemType::Unknown;
    else if (strType == "Byte")
        return AttributeItemType::Byte;
    else if (strType == "UnsignedByte")
        return AttributeItemType::UnsignedByte;
    else if (strType == "Short")
        return AttributeItemType::Short;
    else if (strType == "UnsignedShort")
        return AttributeItemType::UnsignedShort;
    else if (strType == "Float")
        return AttributeItemType::Float;
    return AttributeItemType::Unknown;
}

BufferAttributes* BufferAttributes::fromFile(const tstring& filePath)
{
    tstring strJson;
    if (!File::readFileAsString(strJson, filePath)) return nullptr;

    BufferAttributes* attrs = new BufferAttributes();
    if (!JsonSerializer::fromJson(strJson, *attrs))
    {
        SAFE_DELETE(attrs);
        return nullptr;
    }

    return attrs;
}

BufferAttributes* BufferAttributes::fromStock(StockAttributes stockAttrs)
{
    static const AttributeItemList s_attrPos3 = {
        {3, AttributeItemType::Float, 0},
    };

    static const AttributeItemList s_attrPos3Color = {
        {3, AttributeItemType::Float, 0},
        {4, AttributeItemType::UnsignedByte, 12},
    };

    static const AttributeItemList s_attrPos3Uv2 = {
        {3, AttributeItemType::Float, 0},
        {2, AttributeItemType::Float, 12},
    };

    static const AttributeItemList s_attrPos3Normal = {
        {3, AttributeItemType::Float, 0},
        {2, AttributeItemType::Float, 12},
    };

    static const AttributeItemList s_attrPos3Uv2Normal = {
        {3, AttributeItemType::Float, 0},
        {2, AttributeItemType::Float, 12},
        {3, AttributeItemType::Float, 20},
    };

    static const AttributeItemList s_attrPos3Uv2Color = {
        {3, AttributeItemType::Float, 0},
        {2, AttributeItemType::Float, 12},
        {4, AttributeItemType::UnsignedByte, 20},
    };

    static const AttributeItemList s_attrPos3Uv2NormalTangent = {
        {3, AttributeItemType::Float, 0},
        {2, AttributeItemType::Float, 12},
        {3, AttributeItemType::Float, 20},
        {3, AttributeItemType::Float, 32},
    };

    static const AttributeItemList* s_stockAttributeItems[static_cast<int>(StockAttributes::NumberOfStockAttributes)] = {
        &s_attrPos3,                 // Pos3
        &s_attrPos3Color,            // Pos3Color
        &s_attrPos3Uv2,              // Pos3Uv2
        &s_attrPos3Normal,           // Pos3Normal
        &s_attrPos3Uv2Normal,        // Pos3Uv2Normal
        &s_attrPos3Uv2Color,         // Pos3Uv2Color
        &s_attrPos3Uv2NormalTangent, // Pos3Uv2Color
    };

    static const tstring s_stockAttributeId[static_cast<int>(StockAttributes::NumberOfStockAttributes)] = {
        "stock::Pos3",
        "stock::Pos3Color",
        "stock::Pos3Uv2",
        "stock::Pos3Normal",
        "stock::Pos3Uv2Normal",
        "stock::Pos3Uv2Color",
        "stock::Pos3Uv2NormalTangent",
    };

    return fromAttributeItems(s_stockAttributeId[static_cast<int>(stockAttrs)], *s_stockAttributeItems[static_cast<int>(stockAttrs)]);
}

BufferAttributes* BufferAttributes::fromAttributeItems(const tstring& id, const AttributeItemList& attrItems)
{
    BufferAttributes* bufferAttributes = new BufferAttributes();
    bufferAttributes->m_attributeItems = attrItems;
    return bufferAttributes;
}

NS_END
