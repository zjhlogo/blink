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
    glm::vec3 randomVec3Normalized()
    {
        static std::random_device rd;               // Will be used to obtain a seed for the random number engine
        static std::default_random_engine re(rd()); // Standard mersenne_twister_engine seeded with rd()
        return randomVec3Normalized(re);
    }

    glm::vec3 randomVec3Normalized(std::default_random_engine& re)
    {
        std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);

        glm::vec3 n(distribution(re), distribution(re), distribution(re));
        return glm::normalize(n);
    }

    float randomRange(std::default_random_engine& re, float min, float max)
    {
        std::uniform_real_distribution<float> distribution(min, max);
        return distribution(re);
    }

    float randomRadian(std::default_random_engine& re)
    {
        return randomRange(re, 0.0f, glm::pi<float>());
    }

    glm::quat randomQuat(std::default_random_engine& re)
    {
        return glm::quat(glm::vec3(randomRadian(re), randomRadian(re), 0.0f));
    }
} // namespace blink
