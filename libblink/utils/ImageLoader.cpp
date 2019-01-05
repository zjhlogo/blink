#include "ImageLoader.h"
#include <Log.h>
#include <PathParser.h>
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#define STBI_ONLY_JPEG
#include "stb_image.h"

namespace blink
{
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

    bool ImageLoader::decodeRegularImage(ImageInfo& imageInfoOut, const tstring& filePath)
    {
        imageInfoOut.data = stbi_load(filePath.c_str(), &imageInfoOut.width, &imageInfoOut.height, &imageInfoOut.channels, 0);
        if (!imageInfoOut.data) return false;

        return true;
    }

    bool ImageLoader::createTextureFromRawData(TextureInfo& textureInfo, int width, int height, int channels, const void* data)
    {
        GLint glFormat = GL_ALPHA;
        switch (channels)
        {
        case 1:
            glFormat = GL_LUMINANCE;
            break;
        case  2:
            glFormat = GL_LUMINANCE_ALPHA;
        case 3:
            glFormat = GL_RGB;
            break;
        case 4:
            glFormat = GL_RGBA;
            break;
        default:
            return false;
            break;
        }

        GLuint textureId = 0;
        glGenTextures(1, &textureId);
        if (textureId == 0) return false;

        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, glFormat, width, height, 0, glFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        textureInfo.textureId = textureId;
        textureInfo.texSize = glm::ivec2(width, height);
        return true;
    }

    bool ImageLoader::loadTextureFromImage(ImageInfo& imageInfo, const tstring& filePath)
    {
        auto eImageFileType = getImageType(filePath);

        switch (eImageFileType)
        {
        case ImageFileType::Png:
        case ImageFileType::Jpg:
        {
            if (!decodeRegularImage(imageInfo, filePath)) return false;
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
