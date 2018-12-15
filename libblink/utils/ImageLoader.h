#pragma once
#include <BaseType.h>

namespace blink
{
    class ImageUtil
    {
    public:
        enum class ImageFileType
        {
            Unknown = 0,
            Png,
            Pvr,
        };

        class ImageInfo
        {
        public:
            int m_width{};
            int m_height{};
            int m_channels{ 1 };
            BufferData m_bufferData;
        };

        class TextureInfo
        {
        public:
            uint32 m_textureId{};
            glm::ivec2 m_texSize;
        };

    public:
        static ImageFileType getImageFileType(const tstring& filePath);
        static bool loadTextureFromImage(ImageInfo& imageInfo, ImageFileType eImageFileType, const tstring& filePath);
        static bool createTextureFromRawData(TextureInfo& textureInfo, int width, int height, int channels, const BufferData& buffer);

        static bool decodePngImage(ImageInfo& imageInfoOut, const tstring& filePath);
        static bool savePngImage(const tstring& filePath, const ImageInfo& imageInfo);
    };
}
