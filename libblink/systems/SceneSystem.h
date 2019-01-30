#pragma once
#include <BaseType.h>
#include <entityx/entityx.h>

namespace blink
{
    class SceneSystem : public entityx::System<SceneSystem>
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

    private:
        void updateNodeTransform(Node& node, const glm::mat4& parentToWorldTransform, bool parentTransformChanged);

    };
}
