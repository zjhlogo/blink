#pragma once
#include <BaseType.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace blink
{
    enum TransformFlag
    {
        TRANSFORM_LOCAL_DIRTY = 1 << 0,
    };

    struct Transform
    {
        uint32 bitFlag{ TransformFlag::TRANSFORM_LOCAL_DIRTY };
        glm::vec3 position{ VEC3_ZERO };
        glm::quat rotation{ QUAT_ZERO };
        glm::vec3 scale{ VEC3_ONE };
        glm::mat4 localToParentTransform;
        glm::mat4 localToWorldTransform;
    };

    struct Hierarchy
    {
        Hierarchy(int id_, int parentId_) : id(id_), parentId(parentId_) {};

        int id;
        int parentId;
    };

    struct CameraData
    {
        CameraData(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& up) : cameraPos(pos), cameraTarget(target), cameraUp(up) {};

        glm::vec3 cameraPos;
        glm::vec3 cameraTarget;
        glm::vec3 cameraUp;
        glm::mat4 worldToCamera;
        glm::mat4 cameraToClip;
        glm::mat4 worldToClip;
        uint32 bitFlag{ TransformFlag::TRANSFORM_LOCAL_DIRTY };
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

    struct MouseEvent
    {
        enum class Action
        {
            Move,
            ButtonDown,
            ButtonUp,
            Scroll,
        };

        enum MouseButton
        {
            MOUSE_BUTTON_1 = 0,
            MOUSE_BUTTON_2 = 1,
            MOUSE_BUTTON_3 = 2,
            MOUSE_BUTTON_4 = 3,
            MOUSE_BUTTON_5 = 4,
            MOUSE_BUTTON_6 = 5,
            MOUSE_BUTTON_7 = 6,
            MOUSE_BUTTON_8 = 7,

            MOUSE_BUTTON_LEFT = MOUSE_BUTTON_1,
            MOUSE_BUTTON_RIGHT = MOUSE_BUTTON_2,
            MOUSE_BUTTON_MIDDLE = MOUSE_BUTTON_3,
        };

        static const uint32 MOD_SHIFT = 1 << 0;
        static const uint32 MOD_CONTROL = 1 << 1;
        static const uint32 MOD_ALT = 1 << 2;
        static const uint32 MOD_SUPER = 1 << 3;

        MouseEvent(Action act, const glm::vec2& pos) :action(act), mousePos(pos) {};
        MouseEvent(Action act, MouseButton button, uint32 modKey) :action(act), mouseButton(button), modifyKey(modKey) {};

        Action action{ Action::Move };
        MouseButton mouseButton{};
        glm::vec2 mousePos{};
        uint32 modifyKey{};
    };
}
