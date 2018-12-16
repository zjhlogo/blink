#pragma once

#include <glm/glm.hpp>
#include <cstdint>
#include <string>
#include <vector>
#include <set>

namespace blink
{
    typedef int8_t                      int8;
    typedef uint8_t                     uint8;
    typedef int16_t                     int16;
    typedef uint16_t                    uint16;
    typedef int32_t                     int32;
    typedef unsigned int                uint32;
    typedef long long                   int64;
    typedef unsigned long long          uint64;

    typedef std::string                 tstring;
    extern const tstring                EMPTY_STRING;
    typedef std::vector<tstring>        StringList;
    typedef std::vector<uint8>          BufferData;
    typedef std::vector<glm::mat4>      Mat4List;

    extern const glm::ivec2             IVEC2_ZERO;
    extern const glm::ivec3             IVEC3_ZERO;
    extern const glm::ivec4             IVEC4_ZERO;

    extern const glm::vec2              VEC2_ZERO;
    extern const glm::vec3              VEC3_ZERO;
    extern const glm::vec4              VEC4_ZERO;

    extern const glm::vec2              VEC2_HALF;
    extern const glm::vec3              VEC3_HALF;
    extern const glm::vec4              VEC4_HALF;

    extern const glm::vec2              VEC2_ONE;
    extern const glm::vec3              VEC3_ONE;
    extern const glm::vec4              VEC4_ONE;

    extern const glm::vec3              VEC3_PX;
    extern const glm::vec3              VEC3_NX;
    extern const glm::vec3              VEC3_PY;
    extern const glm::vec3              VEC3_NY;
    extern const glm::vec3              VEC3_PZ;
    extern const glm::vec3              VEC3_NZ;

    extern const glm::quat              QUAT_ZERO;

    extern const glm::vec4              VEC4_WHITE;
    extern const glm::vec4              VEC4_BLACK;
    extern const glm::vec4              VEC4_GRAY;
    extern const glm::vec4              VEC4_RED;
    extern const glm::vec4              VEC4_GREEN;
    extern const glm::vec4              VEC4_BLUE;

    extern const glm::mat4              MAT4_IDENTITY;

    class EnumClassHash
    {
    public:
        template <typename T>
        std::size_t operator()(T t) const
        {
            return static_cast<std::size_t>(t);
        }
    };

}

#define SAFE_DELETE(x) if (x) {delete x; x=nullptr;}
#define SAFE_DELETE_ARRAY(x) if (x) {delete[] x; x=nullptr;}
#define SAFE_DELETE_AND_TERMINATE(x) if (x) {x->terminate(); delete x; x=nullptr;}

#define SAFE_RELEASE(x) if (x) {x->release(); x=nullptr;}
