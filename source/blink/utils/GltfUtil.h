
/*********************************************************************
 * \file   GltfUtil.h
 * \brief
 *
 * \author zjhlogo
 * \date   August 2022
 *********************************************************************/
#pragma once
#include <foundation/BaseTypes.h>

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include <tinygltf/tiny_gltf.h>

namespace blink
{
    class GltfUtil
    {
    public:
        static bool loadFromFile(tinygltf::Model& outModel, const tstring& filePath);

    private:
        static tinygltf::FsCallbacks m_fsCallback;
    };
} // namespace blink