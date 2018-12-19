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

        void update(float dt);

        bool setPosition(const glm::vec3& pos) { m_position = pos; };
        const glm::vec3& getPosition() const { return m_position; };

        bool setRotation(const glm::quat& rot) { m_rotation = rot; };
        const glm::quat& getRotation() const { return m_rotation; };

        bool setScale(const glm::vec3& scale) { m_scale = scale; };
        const glm::vec3& getScale() const { return m_scale; };

        const glm::mat4& getLocalToParentTransform() const { return m_localToParent; };

    private:
        void updateTransform();

    private:
        glm::vec3 m_position;
        glm::quat m_rotation;
        glm::vec3 m_scale{ 1.0f, 1.0f, 1.0f };
        glm::mat4 m_localToParent;
        bool m_transformDirty{ true };

        ObjectList m_children;

    };
}
