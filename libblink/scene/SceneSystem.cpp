#include "SceneSystem.h"
#include <glm/gtc/matrix_transform.hpp>

namespace blink
{
    void SceneSystem::configure(entityx::EventManager & events)
    {
        events.subscribe<entityx::ComponentAddedEvent<HierarchyData>>(*this);
        events.subscribe<entityx::ComponentRemovedEvent<HierarchyData>>(*this);
    }

    void SceneSystem::update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt)
    {
        std::vector<Node> nodes;

        // collect all nodes
        uint32 index = 0;
        entities.each<TransformData, HierarchyData>([&](entityx::Entity entity, TransformData& transform, HierarchyData& hierarchy)
        {
            Node node;
            node.entity = entity;
            node.id = hierarchy.id;
            node.parentId = hierarchy.parentId;

            nodes.push_back(node);
        });

        // sort node by id
        std::sort(nodes.begin(), nodes.end(), [](Node& a, Node& b)
        {
            return a.id < b.id;
        });

        // remap id/parentId to 0 based index
        std::unordered_map<uint32, uint32> idsMap;
        idsMap[-1] = -1;

        index = 0;
        for (auto& node : nodes)
        {
            if (node.id != index)
            {
                idsMap[node.id] = index;
                node.id = index;
            }

            auto itNewParent = idsMap.find(node.parentId);
            assert(itNewParent != idsMap.end() && "must found parent id for sorted nodes");
            node.parentId = itNewParent->second;
            node.transformChanged = false;

            ++index;
        }

        // update node transform
        for (auto& node : nodes)
        {
            const glm::mat4* parentToWorldTransform = &MAT4_IDENTITY;
            bool parentTransformChanged = false;
            if (node.parentId != -1)
            {
                parentToWorldTransform = &(nodes[node.parentId].entity.component<TransformData>()->localToWorldTransform);
                parentTransformChanged = nodes[node.parentId].transformChanged;
            }

            updateNodeTransform(node, *parentToWorldTransform, parentTransformChanged);
        }
    }

    void SceneSystem::receive(const entityx::ComponentAddedEvent<HierarchyData>& evt)
    {
        // TODO: 
    }

    void SceneSystem::receive(const entityx::ComponentRemovedEvent<HierarchyData>& evt)
    {
        // TODO: 
    }

    void SceneSystem::updateNodeTransform(Node& node, const glm::mat4& parentToWorldTransform, bool parentTransformChanged)
    {
        auto transform = node.entity.component<TransformData>().get();

        if (transform->bitFlag & TransformData::BF_DIRTY)
        {
            // TODO: can be optimize by build matrix without multiple

            glm::mat4 matRot = glm::mat4_cast(transform->rotation);
            glm::mat4 matScale = glm::scale(MAT4_IDENTITY, transform->scale);
            glm::mat4 matTransLocalPos = glm::translate(MAT4_IDENTITY, transform->position);

            transform->localToParentTransform = matTransLocalPos * matRot * matScale;	// scale -> rotation -> translate
            transform->bitFlag &= (~TransformData::BF_DIRTY);
            node.transformChanged = true;
        }

        if (node.transformChanged || parentTransformChanged)
        {
            transform->localToWorldTransform = parentToWorldTransform * transform->localToParentTransform;
            node.transformChanged = true;
        }
    }
}
