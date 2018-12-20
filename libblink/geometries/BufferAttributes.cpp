#include "BufferAttributes.h"
#include <InstanceManager.h>
#include <glad/glad.h>
#include <tinyxml2.h>

namespace blink
{
    static InstanceManager<tstring, BufferAttributes> s_instanceManager;

    const BufferAttributes::AttributeItem* BufferAttributes::getAttributeItem(int nIndex) const
    {
        if (nIndex < 0 || nIndex >= m_numItems) return nullptr;
        return &m_attributeItems[nIndex];
    }

    const BufferAttributes::AttributeItem* BufferAttributes::getAttributeItemByName(const tstring& name) const
    {
        for (int i = 0; i < m_numItems; ++i)
        {
            if (m_attributeItems[i].m_name == name)
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

            if (m_attributeItems[i].m_size != pAttrItem->m_size) return false;
            if (m_attributeItems[i].m_attrType != pAttrItem->m_attrType) return false;
            if (m_attributeItems[i].m_name != pAttrItem->m_name) return false;
        }

        return true;
    }

    void BufferAttributes::release()
    {
        if (s_instanceManager.removeInstance(this))
        {
            delete this;
        }
    }

    uint32 BufferAttributes::getGlType(AttributeItemType eType)
    {
        static const uint32 s_GlType[static_cast<int>(AttributeItemType::NumberOfAttributeItemType)] =
        {
            GL_FLOAT,			// AT_UNKNOWN,
            GL_BYTE,			// AT_BYTE,
            GL_UNSIGNED_BYTE,	// AT_UNSIGNED_BYTE,
            GL_SHORT,			// AT_SHORT,
            GL_UNSIGNED_SHORT,	// AT_UNSIGNED_SHORT,
            GL_FLOAT,			// AT_FLOAT,
        };

        if (eType < AttributeItemType::Unknown || eType >= AttributeItemType::NumberOfAttributeItemType) return GL_FLOAT;
        return s_GlType[static_cast<int>(eType)];
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

    BufferAttributes * BufferAttributes::fromFile(const tstring & filePath)
    {
        auto exitInst = s_instanceManager.insertInstance(filePath);
        if (exitInst) return exitInst;

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

            attrItems[attrIndex].m_size = size;
            attrItems[attrIndex].m_attrType = getAttributeItemType(pszType);
            attrItems[attrIndex].m_glType = getGlType(attrItems[attrIndex].m_attrType);
            attrItems[attrIndex].m_name = pszName;

            ++attrIndex;
        }

        if (attrIndex <= 0 || attrIndex > MAX_ATTRIBUTE_ITEMS) return nullptr;

        attrItems[attrIndex].m_size = 0;
        attrItems[attrIndex].m_attrType = AttributeItemType::Unknown;
        attrItems[attrIndex].m_glType = getGlType(attrItems[attrIndex].m_attrType);
        attrItems[attrIndex].m_offset = 0;
        attrItems[attrIndex].m_name.clear();

        return fromAttributeItems("file::" + filePath, attrItems);
    }

    BufferAttributes * BufferAttributes::fromStock(StockAttributes stockAttrs)
    {
        static AttributeItem s_attrPos3[] =
        {
            {3, AttributeItemType::Float, GL_FLOAT, 0, "a_position"},
            {0, AttributeItemType::Unknown, GL_FLOAT, 0, ""},
        };

        static AttributeItem s_attrPos3Color[] =
        {
            {3, AttributeItemType::Float, GL_FLOAT, 0, "a_position"},
            {4, AttributeItemType::UnsignedByte, GL_UNSIGNED_BYTE, 12, "a_color"},
            {0, AttributeItemType::Unknown, GL_FLOAT, 0, ""},
        };

        static AttributeItem s_attrPos3Uv2[] =
        {
            {3, AttributeItemType::Float, GL_FLOAT, 0, "a_position"},
            {2, AttributeItemType::Float, GL_FLOAT, 12, "a_uv"},
            {0, AttributeItemType::Unknown, GL_FLOAT, 0, ""},
        };

        static AttributeItem s_attrPos3Uv2Normal[] = 
        {
            {3, AttributeItemType::Float, GL_FLOAT, 0, "a_position"},
            {2, AttributeItemType::Float, GL_FLOAT, 12, "a_uv"},
            {3, AttributeItemType::Float, GL_FLOAT, 20, "a_normal"},
            {0, AttributeItemType::Unknown, GL_FLOAT, 0, ""},
        };

        static AttributeItem s_attrPos3Uv2Color[] =
        {
            {3, AttributeItemType::Float, GL_FLOAT, 0, "a_position"},
            {2, AttributeItemType::Float, GL_FLOAT, 12, "a_uv"},
            {4, AttributeItemType::UnsignedByte, GL_UNSIGNED_BYTE, 20, "a_color"},
            {0, AttributeItemType::Unknown, GL_FLOAT, 0, ""},
        };

        static AttributeItem* s_stockAttributeItems[static_cast<int>(StockAttributes::NumberOfStockAttributes)] = 
        {
            s_attrPos3,                  // Pos3
            s_attrPos3Color,             // Pos3Color
            s_attrPos3Uv2,               // Pos3Uv2
            s_attrPos3Uv2Normal,         // Pos3Uv2Normal
            s_attrPos3Uv2Color,          // Pos3Uv2Color
        };

        static const tstring s_stockAttributeId[static_cast<int>(StockAttributes::NumberOfStockAttributes)] =
        {
            "stock::Pos3",
            "stock::Pos3Color",
            "stock::Pos3Uv2",
            "stock::Pos3Uv2Normal",
            "stock::Pos3Uv2Color",
        };

        return fromAttributeItems(s_stockAttributeId[static_cast<int>(stockAttrs)], s_stockAttributeItems[static_cast<int>(stockAttrs)]);
    }

    BufferAttributes * BufferAttributes::fromAttributeItems(const tstring& id, const AttributeItem* pAttrItems)
    {
        auto exitInst = s_instanceManager.insertInstance(id);
        if (exitInst) return exitInst;

        int nNumItems = 0;
        const AttributeItem* pCurrItem = pAttrItems;
        while (pCurrItem && pCurrItem->m_attrType != AttributeItemType::Unknown)
        {
            nNumItems++;
            pCurrItem++;
        }

        if (nNumItems <= 0 || nNumItems > MAX_ATTRIBUTE_ITEMS) return nullptr;

        BufferAttributes* pBufferAttributes = new BufferAttributes();
        pBufferAttributes->m_numItems = nNumItems;

        uint32 currOffset = 0;
        for (int i = 0; i < nNumItems; ++i)
        {
            pBufferAttributes->m_attributeItems[i].m_size = pAttrItems[i].m_size;
            pBufferAttributes->m_attributeItems[i].m_attrType = pAttrItems[i].m_attrType;
            pBufferAttributes->m_attributeItems[i].m_glType = pAttrItems[i].m_glType;
            pBufferAttributes->m_attributeItems[i].m_offset = currOffset;
            pBufferAttributes->m_attributeItems[i].m_name = pAttrItems[i].m_name;
            currOffset += getAttributeItemSize(pBufferAttributes->m_attributeItems[i].m_size, pBufferAttributes->m_attributeItems[i].m_attrType);
        }

        pBufferAttributes->m_attributeItems[nNumItems].m_size = 0;
        pBufferAttributes->m_attributeItems[nNumItems].m_attrType = AttributeItemType::Unknown;
        pBufferAttributes->m_attributeItems[nNumItems].m_glType = getGlType(AttributeItemType::Unknown);
        pBufferAttributes->m_attributeItems[nNumItems].m_offset = currOffset;
        pBufferAttributes->m_attributeItems[nNumItems].m_name.clear();

        return s_instanceManager.insertInstance(id, pBufferAttributes);
    }
}
