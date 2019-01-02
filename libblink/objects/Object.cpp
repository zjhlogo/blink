#include "Object.h"
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

namespace blink
{
    Object::Object()
    {
    }

    Object::~Object()
    {
        removeAll();
    }

    bool Object::add(Object * object)
    {
        assert(object);
        assert(std::find(m_children.begin(), m_children.end(), object) == m_children.end());

        m_children.push_back(object);
        return true;
    }

    bool Object::remove(Object * object)
    {
        auto it = std::find(m_children.begin(), m_children.end(), object);
        assert(it != m_children.end());

        if (it != m_children.end())
        {
            SAFE_DELETE(object);
            m_children.erase(it);
            return true;
        }

        return false;
    }

    void Object::removeAll()
    {
        for (auto object : m_children)
        {
            SAFE_DELETE(object);
        }
        m_children.clear();
    }

    void Object::applyRotation(float degrees, const glm::vec3& axis)
    {
        m_rotation *= glm::angleAxis(glm::radians(degrees), axis);
        m_transformDirty = true;
    }

    void Object::updateWorldTransform(const glm::mat4 & parentToWorld)
    {
        updateLocalTransform();
        m_localToWorld = parentToWorld * m_localToParent;

        for (auto child : m_children)
        {
            child->updateWorldTransform(m_localToWorld);
        }
    }

    void Object::updateLocalTransform()
    {
        if (m_transformDirty)
        {
            glm::mat4 matRot = glm::mat4_cast(m_rotation);
            glm::mat4 matScale = glm::scale(MAT4_IDENTITY, m_scale);
            glm::mat4 matTransLocalPos = glm::translate(MAT4_IDENTITY, m_position);

            m_localToParent = matTransLocalPos * matRot * matScale;	// scale -> rotation -> translate

            m_transformDirty = false;
        }
    }
}
