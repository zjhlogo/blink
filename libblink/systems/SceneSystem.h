#pragma once
#include <BaseType.h>
#include <entityx/entityx.h>

namespace blink
{
    class SceneSystem : public entityx::System<SceneSystem>
    {
    public:
        class SceneNode
        {
        public:
            entityx::Entity entity;
            std::vector<SceneNode*> children;
        };

    public:
        virtual ~SceneSystem() {};

        void configure(entityx::EventManager &events) override;
        void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt) override;

        SceneNode* add(entityx::Entity& entity, SceneNode* parent);

    private:
        void updateTransformRecruite(SceneNode* node, const glm::mat4& parentToWorldTransform, bool localChanged = false);

    private:
        std::vector<SceneNode*> m_nodes;

    };
}
