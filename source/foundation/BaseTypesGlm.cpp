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
        static std::random_device rd;  //Will be used to obtain a seed for the random number engine
        static std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()

        std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);

        glm::vec3 n;

        n.x = distribution(gen);
        n.y = distribution(gen);
        n.z = distribution(gen);

        return glm::normalize(n);
    }
} // namespace blink
