
/*********************************************************************
 * \file   UserCommandSystem.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   04/02/2022
 *********************************************************************/
#include "UserCommandSystem.h"
#include "../components/UserCommandComponents.h"

#include <imgui/imgui.h>
#include <physics/components/Components.h>

UserCommandSystem::UserCommandSystem()
{
}

bool UserCommandSystem::initialize(flecs::world& world)
{
    world.system<blink::PhysicsAccumulate>("process user command push me")
        .each(
            [&](flecs::entity e, blink::PhysicsAccumulate& pa)
            {
                if (!m_pushMeClicked) return;

                pa.torqueAccum += blink::randomVec3Normalized() * 1000.0f;
            });

    return true;
}

void UserCommandSystem::terminate(flecs::world& world)
{
    //
}

void UserCommandSystem::framePreUpdate(flecs::world& world)
{
    //
}

void UserCommandSystem::framePostUpdate(flecs::world& world)
{
    m_pushMeClicked = false;
}

void UserCommandSystem::renderUi()
{
    if (ImGui::Button("Push Me", ImVec2(-1.0f, 0.0f)))
    {
        m_pushMeClicked = true;
    }
}
