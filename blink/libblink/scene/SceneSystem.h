/*!
 * \file SceneSystem.h
 *
 * \author zjhlogo
 * \date 2019/11/15
 *
 * 
 */
#pragma once
#include <entityx/entityx.h>
#include "TransformData.h"

NS_BEGIN

class SceneSystem : public entityx::System<SceneSystem>, public entityx::Receiver<SceneSystem>
{
public:
    class Node
    {
    public:
        int id;
        int parentId;
        entityx::Entity entity;
        bool transformChanged;
    };

public:
    virtual ~SceneSystem() {};

    void configure(entityx::EventManager &events) override;
    void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt) override;

    void receive(const entityx::ComponentAddedEvent<HierarchyData>& evt);
    void receive(const entityx::ComponentRemovedEvent<HierarchyData>& evt);

private:
    void updateNodeTransform(Node& node, const glm::mat4& parentToWorldTransform, bool parentTransformChanged);
    void updateHierarchy();

};

NS_END
