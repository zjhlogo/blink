#pragma once
#include <entityx/entityx.h>
#include <BaseType.h>

class PlayerMovementSystem : public entityx::System<PlayerMovementSystem>, public entityx::Receiver<PlayerMovementSystem>
{
public:
    PlayerMovementSystem();
    virtual ~PlayerMovementSystem();

    void configure(entityx::EventManager &events) override;
    void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt) override;

private:
    glm::ivec2 m_currGrid;
    glm::ivec2 m_nextGrid;
    glm::vec2 m_position;

};
