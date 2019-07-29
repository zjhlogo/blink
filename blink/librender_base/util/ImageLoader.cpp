/*!
 * \file ImageLoader.cpp
 *
 * \author zjhlogo
 * \date 2019/07/26
 *
 * 
 */
#include "ImageLoader.h"
#include <Log.h>
#include <PathParser.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#define STBI_ONLY_JPEG
#include "stb_image.h"

namespace blink
{
    ImageLoader::CreateTextureFromRawCb ImageLoader::m_createTextureFromRawData;

    ImageLoader::ImageInfo::~ImageInfo()
    {
        if (data)
        {
            stbi_image_free(data);
            data = nullptr;
        }
    }

    ImageLoader::ImageFileType ImageLoader::getImageType(const tstring& filePath)
    {
        tstring strExt = PathParser::getFileLowerExtension(filePath);
        if (strExt == "png") return ImageFileType::Png;
        if (strExt == "jpg" || strExt == "jpeg") return ImageFileType::Jpg;
        if (strExt == "pvr") return ImageFileType::Pvr;
        return ImageFileType::Unknown;
    }

    bool ImageLoader::decodeRegularImage(ImageInfo& imageInfoOut, const tstring& filePath, bool flipY)
    {
        stbi_set_flip_vertically_on_load(flipY);
        imageInfoOut.data = stbi_load(filePath.c_str(), &imageInfoOut.width, &imageInfoOut.height, &imageInfoOut.channels, 0);
        if (!imageInfoOut.data) return false;

        return true;
    }

    bool ImageLoader::loadTextureFromImage(ImageInfo& imageInfo, const tstring& filePath, bool flipY)
    {
        auto eImageFileType = getImageType(filePath);

        switch (eImageFileType)
        {
        case ImageFileType::Png:
        case ImageFileType::Jpg:
        {
            if (!decodeRegularImage(imageInfo, filePath, flipY)) return false;
        }
        break;
        case ImageFileType::Pvr:
        {
            LOGE("Unsupport yet");
            return false;
        }
        break;
        default:
        {
            return false;
        }
        break;
        }

        return true;
    }
}
