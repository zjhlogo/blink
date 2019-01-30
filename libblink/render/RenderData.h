#pragma once
#include <BaseType.h>

namespace blink
{
    enum AttributeConst
    {
        MaxAttributeSize = 128,
        MaxAttributeItem = 8,
    };

    enum AttributeItemType
    {
        Unknown = 0,
        Byte,
        UnsignedByte,
        Short,
        UnsignedShort,
        Float,
        NumberOfAttributeItemType,
    };

    struct AttributeItem
    {
        uint32 size;
        AttributeItemType attrType;
        uint32 glType;
        uint32 offset;
        tstring name;
    };

    enum StockAttributes
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

    struct BufferGeometry
    {
        int numAttributes{};
        AttributeItem* attributeItems{};
        uint32 vertexArrayObjectId{};
        uint32 vertexBufferId{};
        uint32 indexBufferId{};
        uint32 numIndex{};
    };

    struct PhongMaterial
    {
        glm::vec3 diffuseColor{ 1.0f, 1.0f, 1.0f };
    };
}
