/*!
 * \file BaseTypes.h
 *
 * \author zjhlogo
 * \date 2019/07/26
 *
 * 
 */
#pragma once

#include <cstdint>
#include <string>

namespace blink
{
    typedef int8_t      int8;
    typedef uint8_t     uint8;
    typedef int16_t     int16;
    typedef uint16_t    uint16;
    typedef int32_t     int32;
    typedef uint32_t    uint32;
    typedef int64_t     int64;
    typedef uint64_t    uint64;
    typedef std::string tstring;

    extern const tstring EMPTY_STRING;
}

#define SAFE_DELETE(x) if (x) {delete x; x=nullptr;}
#define SAFE_DELETE_ARRAY(x) if (x) {delete[] x; x=nullptr;}
#define SAFE_DELETE_AND_TERMINATE(x) if (x) {x->terminate(); delete x; x=nullptr;}
