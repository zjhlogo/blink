/**

    @file      Components.h
    @brief     
    @details   ~
    @author    zjhlogo
    @date      31.10.2021
    @copyright © Cool Guy, 2021. All right reserved.

**/
#pragma once

#include <foundation/BaseTypesGlm.h>

NS_BEGIN

class Mesh;
class Material;

struct Position
{
    glm::vec3 value;
};

struct Rotation
{
    glm::quat value;
};

struct StaticModel
{
    Mesh* mesh;
    Material* material;
};

NS_END
