/*!
 * \file BufferAttributes.cpp
 *
 * \author zjhlogo
 * \date 2019/07/25
 *
 * 
 */
#include "BufferAttributes.h"
#include <InstanceManager.h>
#include <tinyxml2/tinyxml2.h>

namespace blink
{
    BufferAttributes::BufferAttributes()
    {

    }

    BufferAttributes::~BufferAttributes()
    {

    }

    const BufferAttributes::AttributeItem* BufferAttributes::getAttributeItem(int nIndex) const
    {
        if (nIndex < 0 || nIndex >= m_numItems) return nullptr;
        return &m_attributeItems[nIndex];
    }

    const BufferAttributes::AttributeItem* BufferAttributes::getAttributeItemByName(const tstring& name) const
    {
        for (int i = 0; i < m_numItems; ++i)
        {
            if (m_attributeItems[i].name == name)
            {
                return &m_attributeItems[i];
            }
        }

        return nullptr;
    }

    bool BufferAttributes::isEqual(const BufferAttributes* pVertexAttrs) const
    {
        if (!pVertexAttrs) return false;

        if (m_numItems != pVertexAttrs->getNumAttributeItems()) return false;

        for (int i = 0; i < m_numItems; ++i)
        {
            const AttributeItem* pAttrItem = pVertexAttrs->getAttributeItem(i);

            if (m_attributeItems[i].size != pAttrItem->size) return false;
            if (m_attributeItems[i].type != pAttrItem->type) return false;
            if (m_attributeItems[i].name != pAttrItem->name) return false;
        }

        return true;
    }

    uint32 BufferAttributes::getAttributeItemSize(uint32 nSize, AttributeItemType eType)
    {
        static const int s_ItemSizes[static_cast<int>(AttributeItemType::NumberOfAttributeItemType)] =
        {
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
        if (strType == "Unknown") return AttributeItemType::Unknown;
        else if (strType == "Byte") return AttributeItemType::Byte;
        else if (strType == "UnsignedByte") return AttributeItemType::UnsignedByte;
        else if (strType == "Short") return AttributeItemType::Short;
        else if (strType == "UnsignedShort") return AttributeItemType::UnsignedShort;
        else if (strType == "Float") return AttributeItemType::Float;
        return AttributeItemType::Unknown;
    }

    BufferAttributes* BufferAttributes::fromFile(const tstring & filePath)
    {
        tinyxml2::XMLDocument doc;
        if (doc.LoadFile(filePath.c_str()) != tinyxml2::XML_SUCCESS) return nullptr;

        tinyxml2::XMLElement* pXmlRoot = doc.RootElement();
        if (!pXmlRoot) return nullptr;

        AttributeItem attrItems[MAX_ATTRIBUTE_ITEMS + 1];
        int attrIndex = 0;

        for (tinyxml2::XMLElement* pXmlAttr = pXmlRoot->FirstChildElement(); pXmlAttr != nullptr; pXmlAttr = pXmlAttr->NextSiblingElement())
        {
            int size = 0;
            pXmlAttr->QueryAttribute("size", &size);
            if (size <= 0 || size > MAX_ATTR_SIZE) return nullptr;

            const char* pszType = pXmlAttr->Attribute("type");
            if (!pszType) return nullptr;

            const char* pszName = pXmlAttr->Attribute("name");
            if (!pszName) return nullptr;

            attrItems[attrIndex].size = size;
            attrItems[attrIndex].type = getAttributeItemType(pszType);
            attrItems[attrIndex].name = pszName;

            ++attrIndex;
        }

        if (attrIndex <= 0 || attrIndex > MAX_ATTRIBUTE_ITEMS) return nullptr;

        attrItems[attrIndex].size = 0;
        attrItems[attrIndex].type = AttributeItemType::Unknown;
        attrItems[attrIndex].offset = 0;
        attrItems[attrIndex].name.clear();

        return fromAttributeItems("file::" + filePath, attrItems);
    }

    BufferAttributes* BufferAttributes::fromStock(StockAttributes stockAttrs)
    {
        static AttributeItem s_attrPos3[] =
        {
            {3, AttributeItemType::Float, 0, "a_position"},
            {0, AttributeItemType::Unknown, 0, ""},
        };

        static AttributeItem s_attrPos3Color[] =
        {
            {3, AttributeItemType::Float, 0, "a_position"},
            {4, AttributeItemType::UnsignedByte, 12, "a_color"},
            {0, AttributeItemType::Unknown, 0, ""},
        };

        static AttributeItem s_attrPos3Uv2[] =
        {
            {3, AttributeItemType::Float, 0, "a_position"},
            {2, AttributeItemType::Float, 12, "a_uv"},
            {0, AttributeItemType::Unknown, 0, ""},
        };

        static AttributeItem s_attrPos3Normal[] =
        {
            {3, AttributeItemType::Float, 0, "a_position"},
            {2, AttributeItemType::Float, 12, "a_normal"},
            {0, AttributeItemType::Unknown, 0, ""},
        };

        static AttributeItem s_attrPos3Uv2Normal[] = 
        {
            {3, AttributeItemType::Float, 0, "a_position"},
            {2, AttributeItemType::Float, 12, "a_uv"},
            {3, AttributeItemType::Float, 20, "a_normal"},
            {0, AttributeItemType::Unknown, 0, ""},
        };

        static AttributeItem s_attrPos3Uv2Color[] =
        {
            {3, AttributeItemType::Float, 0, "a_position"},
            {2, AttributeItemType::Float, 12, "a_uv"},
            {4, AttributeItemType::UnsignedByte, 20, "a_color"},
            {0, AttributeItemType::Unknown, 0, ""},
        };

        static AttributeItem s_attrPos3Uv2NormalTangent[] =
        {
            {3, AttributeItemType::Float, 0, "a_position"},
            {2, AttributeItemType::Float, 12, "a_uv"},
            {3, AttributeItemType::Float, 20, "a_normal"},
            {3, AttributeItemType::Float, 32, "a_tangent"},
            {0, AttributeItemType::Unknown, 0, ""},
        };

        static AttributeItem* s_stockAttributeItems[static_cast<int>(StockAttributes::NumberOfStockAttributes)] = 
        {
            s_attrPos3,                  // Pos3
            s_attrPos3Color,             // Pos3Color
            s_attrPos3Uv2,               // Pos3Uv2
            s_attrPos3Normal,            // Pos3Normal
            s_attrPos3Uv2Normal,         // Pos3Uv2Normal
            s_attrPos3Uv2Color,          // Pos3Uv2Color
            s_attrPos3Uv2NormalTangent,  // Pos3Uv2Color
        };

        static const tstring s_stockAttributeId[static_cast<int>(StockAttributes::NumberOfStockAttributes)] =
        {
            "stock::Pos3",
            "stock::Pos3Color",
            "stock::Pos3Uv2",
            "stock::Pos3Normal",
            "stock::Pos3Uv2Normal",
            "stock::Pos3Uv2Color",
            "stock::Pos3Uv2NormalTangent",
        };

        return fromAttributeItems(s_stockAttributeId[static_cast<int>(stockAttrs)], s_stockAttributeItems[static_cast<int>(stockAttrs)]);
    }

    BufferAttributes* BufferAttributes::fromAttributeItems(const tstring& id, const AttributeItem* pAttrItems)
    {
        int nNumItems = 0;
        const AttributeItem* pCurrItem = pAttrItems;
        while (pCurrItem && pCurrItem->type != AttributeItemType::Unknown)
        {
            nNumItems++;
            pCurrItem++;
        }

        if (nNumItems <= 0 || nNumItems > MAX_ATTRIBUTE_ITEMS) return nullptr;

        BufferAttributes* bufferAttributes = new BufferAttributes();
        bufferAttributes->m_numItems = nNumItems;

        uint32 currOffset = 0;
        for (int i = 0; i < nNumItems; ++i)
        {
            bufferAttributes->m_attributeItems[i].size = pAttrItems[i].size;
            bufferAttributes->m_attributeItems[i].type = pAttrItems[i].type;
            bufferAttributes->m_attributeItems[i].offset = currOffset;
            bufferAttributes->m_attributeItems[i].name = pAttrItems[i].name;
            currOffset += getAttributeItemSize(bufferAttributes->m_attributeItems[i].size, bufferAttributes->m_attributeItems[i].type);
        }

        bufferAttributes->m_attributeItems[nNumItems].size = 0;
        bufferAttributes->m_attributeItems[nNumItems].type = AttributeItemType::Unknown;
        bufferAttributes->m_attributeItems[nNumItems].offset = currOffset;
        bufferAttributes->m_attributeItems[nNumItems].name.clear();

        return bufferAttributes;
    }
}
