/*!
 * \file PredefinedVertexAttributes.h
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 *
 */
#pragma once
#include <foundation/BaseTypes.h>

NS_BEGIN

// pre-define vertex attributes
class VertexPos3
{
public:
    float x, y, z; // pos3
};

class VertexPos3Color
{
public:
    float x, y, z; // pos3
    uint32 color;  // color
};

class VertexPos3Uv2
{
public:
    float x, y, z; // pos3
    float u, v;    // uv2
};

class VertexPos3Uv2Normal
{
public:
    float x, y, z;    // pos3
    float u, v;       // uv2
    float nx, ny, nz; // normal
};

class VertexPos3Uv2Color
{
public:
    float x, y, z; // pos3
    float u, v;    // uv2
    uint32 color;  // color
};

class VertexPos3Uv2NormalTangent
{
public:
    float x, y, z;    // pos3
    float u, v;       // uv2
    float nx, ny, nz; // normal
    float tx, ty, tz; // tangent
};

NS_END
