#pragma once
#include "../../core/Entity.h"
#include <BaseType.h>
#include <glm/gtc/quaternion.hpp>

namespace blink
{
    class Object : public Entity
    {
    public:
        typedef std::vector<Object*> ObjectList;

    public:
        RTTI_DEF(Object, Entity);

        Object();
        virtual ~Object();

        bool add(Object* object);
        bool remove(Object* object);
        void removeAll();

        void setPosition(const glm::vec3& pos) { m_position = pos; m_transformDirty = true; };
        const glm::vec3& getPosition() const { return m_position; };

        void setRotation(const glm::quat& rot) { m_rotation = rot; m_transformDirty = true; };
        const glm::quat& getRotation() const { return m_rotation; };

        void setScale(const glm::vec3& scale) { m_scale = scale; m_transformDirty = true; };
        const glm::vec3& getScale() const { return m_scale; };

        const glm::mat4& getLocalToParentTransform() const { return m_localToParent; };
        const glm::mat4& getLocalToWorldTransform() const { return m_localToWorld; };

        void updateWorldTransform(const glm::mat4& parentToWorld);

    private:
        void updateLocalTransform();

    private:
        glm::vec3 m_position{ VEC3_ZERO };
        glm::quat m_rotation{ QUAT_ZERO };
        glm::vec3 m_scale{ 1.0f, 1.0f, 1.0f };

        glm::mat4 m_localToParent;
        glm::mat4 m_localToWorld;
        bool m_transformDirty{ true };

        ObjectList m_children;

    };
}
