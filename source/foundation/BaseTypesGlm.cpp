/*!
 * \file BaseTypesGlm.cpp
 *
 * \author zjhlogo
 * \date 2019/07/26
 *
 *
 */
#include "BaseTypesGlm.h"

#include <random>

namespace blink
{
    const glm::vec3 VEC3_PX = glm::vec3(1.0f, 0.0f, 0.0f);
    const glm::vec3 VEC3_NX = glm::vec3(-1.0f, 0.0f, 0.0f);
    const glm::vec3 VEC3_PY = glm::vec3(0.0f, 1.0f, 0.0f);
    const glm::vec3 VEC3_NY = glm::vec3(0.0f, -1.0f, 0.0f);
    const glm::vec3 VEC3_PZ = glm::vec3(0.0f, 0.0f, 1.0f);
    const glm::vec3 VEC3_NZ = glm::vec3(0.0f, 0.0f, -1.0f);

    glm::vec3 randomVec3Normalized()
    {
        static std::random_device rd;  //Will be used to obtain a seed for the random number engine
        static std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()

        std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

        glm::vec3 n;

        n.x = distribution(gen);
        n.y = distribution(gen);
        n.z = distribution(gen);

        return glm::normalize(n);
    }
} // namespace blink
