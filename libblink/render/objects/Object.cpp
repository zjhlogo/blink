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
        m_children.push_back(object);
        return true;
    }

    bool Object::remove(Object * object)
    {
        auto it = std::find(m_children.begin(), m_children.end(), object);
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

    void Object::update(float dt)
    {
        if (m_transformDirty)
        {
            updateTransform();
            m_transformDirty = false;
        }
    }

    void Object::updateTransform()
    {
        glm::mat4 matRot = glm::mat4_cast(m_rotation);
        glm::mat4 matScale = glm::scale(MAT4_IDENTITY, m_scale);
        glm::mat4 matTransLocalPos = glm::translate(MAT4_IDENTITY, m_position);

        m_localToParent = matTransLocalPos * matRot * matScale;	// scale -> rotation -> translate
    }
}