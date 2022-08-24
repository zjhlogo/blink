/*********************************************************************
 * \file   GltfUtil.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   August 2022
 *********************************************************************/

// Define these only in *one* .cc file.
#define TINYGLTF_IMPLEMENTATION
#include "GltfUtil.h"

#include <foundation/File.h>
#include <foundation/Log.h>

namespace blink
{
    bool GltfUtil::loadFromFile(tinygltf::Model& outModel, const tstring& filePath)
    {
        tstring fileContent;
        if (!File::readFileIntoString(fileContent, filePath))
        {
            LOGE("Open file failed {0}", filePath);
            return false;
        }

        tstring err;
        tstring warn;

        tinygltf::TinyGLTF loader;
        bool ret = loader.LoadASCIIFromString(&outModel, &err, &warn, fileContent.data(), static_cast<unsigned int>(fileContent.length()), EMPTY_STRING);
        if (!warn.empty())
        {
            LOGW(warn);
        }

        if (!err.empty())
        {
            LOGE(err);
            return false;
        }

        if (!ret)
        {
            LOGE("Failed to parse glTF file {0}", filePath);
            return false;
        }

        return true;
    }
} // namespace blink
