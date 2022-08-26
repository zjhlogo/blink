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
    bool fileExistsFunction(const std::string& filePath, void* userData)
    {
        File file;
        return file.open(filePath, File::AM_READ);
    }

    std::string expandFilePathFunction(const std::string& filePath, void* userData)
    {
        return filePath;
    }

    bool readWholeFileFunction(std::vector<unsigned char>* outData, std::string* error, const std::string& filePath, void* userData)
    {
        if (!File::readFileIntoBuffer(*outData, filePath))
        {
            *error = fmt::format("read file into buffer failed. {}", filePath);
            return false;
        }

        return true;
    }

    bool writeWholeFileFunction(std::string* error, const std::string& filePath, const std::vector<unsigned char>& contents, void* userData)
    {
        // not support yet
        return false;
    }

    tinygltf::FsCallbacks GltfUtil::m_fsCallback{fileExistsFunction, expandFilePathFunction, readWholeFileFunction, writeWholeFileFunction, nullptr};

    bool GltfUtil::loadFromFile(tinygltf::Model& outModel, const tstring& filePath)
    {
        tstring err;
        tstring warn;

        tinygltf::TinyGLTF loader;
        loader.SetFsCallbacks(m_fsCallback);
        bool ret = loader.LoadASCIIFromFile(&outModel, &err, &warn, filePath);
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
