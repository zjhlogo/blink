#include "VertexAttributes.h"
#include "RenderModule.h"
#include "glad.h"
#include <tinyxml2.h>

namespace blink
{
    VertexAttributes::VertexAttributes(RenderModule* pRenderModule)
        :m_pRenderModule(pRenderModule)
    {
        // TODO: 
    }

    VertexAttributes::~VertexAttributes()
    {
        // TODO: 
    }

    bool VertexAttributes::loadFromFile(const tstring& filePath)
    {
        tinyxml2::XMLDocument doc;
        if (doc.LoadFile(filePath.c_str()) != tinyxml2::XML_SUCCESS) return false;

        tinyxml2::XMLElement* pXmlRoot = doc.RootElement();
        if (!pXmlRoot) return false;

        AttributeItem attrItems[MAX_ATTRIBUTE_ITEMS + 1];
        int attrIndex = 0;

        for (tinyxml2::XMLElement* pXmlAttr = pXmlRoot->FirstChildElement(); pXmlAttr != nullptr; pXmlAttr = pXmlAttr->NextSiblingElement())
        {
            int size = 0;
            pXmlAttr->QueryAttribute("size", &size);
            if (size <= 0 || size > MAX_ATTR_SIZE) return false;

            const char* pszType = pXmlAttr->Attribute("type");
            if (!pszType) return false;

            const char* pszName = pXmlAttr->Attribute("name");
            if (!pszName) return false;

            attrItems[attrIndex].m_size = size;
            attrItems[attrIndex].m_attrType = getAttributeItemType(pszType);
            attrItems[attrIndex].m_glType = getGlType(attrItems[attrIndex].m_attrType);
            attrItems[attrIndex].m_name = pszName;

            ++attrIndex;
        }

        if (attrIndex <= 0 || attrIndex > MAX_ATTRIBUTE_ITEMS) return false;

        attrItems[attrIndex].m_size = 0;
        attrItems[attrIndex].m_attrType = AttributeItemType::Unknown;
        attrItems[attrIndex].m_glType = getGlType(attrItems[attrIndex].m_attrType);
        attrItems[attrIndex].m_offset = 0;
        attrItems[attrIndex].m_name.clear();

        if (createVertexAttribute(attrItems)) return true;

        return false;
    }

    const VertexAttributes::AttributeItem* VertexAttributes::getAttributeItem(int nIndex) const
    {
        if (nIndex < 0 || nIndex >= m_numItems) return nullptr;
        return &m_attributeItems[nIndex];
    }

    const VertexAttributes::AttributeItem* VertexAttributes::getAttributeItemByName(const tstring& name) const
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

    bool VertexAttributes::isEqual(const VertexAttributes* pVertexAttrs) const
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

    uint32 VertexAttributes::getGlType(AttributeItemType eType)
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

    uint32 VertexAttributes::getAttributeItemSize(uint32 nSize, AttributeItemType eType)
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

    VertexAttributes::AttributeItemType VertexAttributes::getAttributeItemType(const tstring& strType)
    {
        if (strType == "Unknown") return AttributeItemType::Unknown;
        else if (strType == "Byte") return AttributeItemType::Byte;
        else if (strType == "UnsignedByte") return AttributeItemType::UnsignedByte;
        else if (strType == "Short") return AttributeItemType::Short;
        else if (strType == "UnsignedShort") return AttributeItemType::UnsignedShort;
        else if (strType == "Float") return AttributeItemType::Float;
        return AttributeItemType::Unknown;
    }

    bool VertexAttributes::createVertexAttribute(const AttributeItem* pAttrItems)
    {
        int nNumItems = 0;
        const AttributeItem* pCurrItem = pAttrItems;
        while (pCurrItem && pCurrItem->m_attrType != AttributeItemType::Unknown)
        {
            nNumItems++;
            pCurrItem++;
        }

        if (nNumItems <= 0 || nNumItems > MAX_ATTRIBUTE_ITEMS) return false;

        m_numItems = nNumItems;
        uint32 currOffset = 0;
        for (int i = 0; i < nNumItems; ++i)
        {
            m_attributeItems[i].m_size = pAttrItems[i].m_size;
            m_attributeItems[i].m_attrType = pAttrItems[i].m_attrType;
            m_attributeItems[i].m_glType = pAttrItems[i].m_glType;
            m_attributeItems[i].m_offset = currOffset;
            m_attributeItems[i].m_name = pAttrItems[i].m_name;
            currOffset += getAttributeItemSize(m_attributeItems[i].m_size, m_attributeItems[i].m_attrType);
        }

        m_attributeItems[m_numItems].m_size = 0;
        m_attributeItems[m_numItems].m_attrType = AttributeItemType::Unknown;
        m_attributeItems[m_numItems].m_glType = getGlType(AttributeItemType::Unknown);
        m_attributeItems[m_numItems].m_offset = currOffset;
        m_attributeItems[m_numItems].m_name.clear();

        return true;
    }
}
