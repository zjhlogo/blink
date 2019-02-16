#pragma once
#include <BaseType.h>

namespace blink
{
    class ImageLoader
    {
    public:
        enum class ImageFileType
        {
            Unknown = 0,
            Png,
            Jpg,
            Pvr,
        };

        class ImageInfo
        {
        public:
            ~ImageInfo();

            int width{};
            int height{};
            int channels{};
            uint8* data{};
        };

        class TextureInfo
        {
        public:
            uint32 textureId{};
            glm::ivec2 texSize;
        };

    public:
        static ImageFileType getImageType(const tstring& filePath);
        static bool loadTextureFromImage(ImageInfo& imageInfo, const tstring& filePath, bool flipY = false);
        static bool decodeRegularImage(ImageInfo& imageInfoOut, const tstring& filePath, bool flipY);

        static bool createTextureFromRawData(TextureInfo& textureInfo, int width, int height, int channels, const void* data);
    };
}
