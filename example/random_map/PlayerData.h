#pragma once
#include <BaseTypes.h>
#include <BaseTypesGlm.h>

struct PlayerData
{
    enum STATUS
    {
        STATUS_STANDBY,
        STATUS_MOVING,
    };

    glm::vec3 position{};
    bool facingRight{ true };
    STATUS status{ STATUS_STANDBY };
    int frameIndex{};

    int head{ 1 };
    int arm{ 1 };
    int body{ 1 };
    int leg{ 1 };
};
