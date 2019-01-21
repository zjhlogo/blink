#pragma once
#include <BaseType.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace blink
{
    enum TransformFlag
    {
        TRANSFORM_LOCAL_DIRTY = 1 << 0,
        TRANSFORM_PARENT_DIRTY = 1 << 1,
    };

    struct Transform
    {
        uint32 bitFlag{ TransformFlag::TRANSFORM_LOCAL_DIRTY | TRANSFORM_PARENT_DIRTY };
        glm::vec3 position{ VEC3_ZERO };
        glm::quat rotation{ QUAT_ZERO };
        glm::vec3 scale{ VEC3_ONE };
        glm::mat4 localToParentTransform;
        glm::mat4 localToWorldTransform;
    };

    static const int MAX_ATTR_SIZE = 128;
    static const int MAX_ATTRIBUTE_ITEMS = 8;

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
        uint32 m_size;
        AttributeItemType m_attrType;
        uint32 m_glType;
        uint32 m_offset;
        tstring m_name;
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
        uint32 m_vertexArrayObjectId{};
        uint32 m_vertexBufferId{};
        uint32 m_indexBufferId{};
        uint32 m_numIndex{};
    };

}
