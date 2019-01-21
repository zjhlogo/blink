#include "SceneSystem.h"
#include "../Components.h"
#include <glm/gtc/matrix_transform.hpp>

namespace blink
{
    void SceneSystem::configure(entityx::EventManager & events)
    {

    }

    void SceneSystem::update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt)
    {
        for (auto node : m_nodes)
        {
            updateTransformRecruite(node, blink::MAT4_IDENTITY);
        }
    }

    SceneSystem::SceneNode* SceneSystem::add(entityx::Entity& entity, SceneNode* parent)
    {
        SceneNode* node = new SceneNode();
        node->entity = entity;

        if (!parent)
        {
            m_nodes.push_back(node);
        }
        else
        {
            parent->children.push_back(node);
        }

        return node;
    }

    void SceneSystem::updateTransformRecruite(SceneNode * node, const glm::mat4& parentToWorldTransform, bool localChanged)
    {
        if (!node->entity.valid()) return;

        auto transform = node->entity.component<Transform>().get();

        if (transform->bitFlag & TransformFlag::TRANSFORM_LOCAL_DIRTY)
        {
            glm::mat4 matRot = glm::mat4_cast(transform->rotation);
            glm::mat4 matScale = glm::scale(MAT4_IDENTITY, transform->scale);
            glm::mat4 matTransLocalPos = glm::translate(MAT4_IDENTITY, transform->position);

            transform->localToParentTransform = matTransLocalPos * matRot * matScale;	// scale -> rotation -> translate
            transform->bitFlag &= (~TransformFlag::TRANSFORM_LOCAL_DIRTY);
            localChanged = true;
        }

        if (localChanged || transform->bitFlag & TransformFlag::TRANSFORM_PARENT_DIRTY)
        {
            transform->localToWorldTransform = parentToWorldTransform * transform->localToParentTransform;
            transform->bitFlag &= (~TransformFlag::TRANSFORM_PARENT_DIRTY);
        }

        for (auto child : node->children)
        {
            updateTransformRecruite(child, transform->localToWorldTransform, localChanged);
        }
    }
}
