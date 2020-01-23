/*!
 * \file ImageLoader.h
 *
 * \author zjhlogo
 * \date 2019/07/26
 *
 *
 */
#pragma once
#include <foundation/BaseTypes.h>
#include <foundation/BaseTypesGlm.h>
#include <functional>

NS_BEGIN

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

    typedef std::function<bool(TextureInfo& textureInfo, int width, int height, int channels, const void* data)>
        CreateTextureFromRawCb;

public:
    static ImageFileType getImageType(const tstring& filePath);
    static bool loadTextureFromImage(ImageInfo& imageInfo, const tstring& filePath, bool flipY = false);
    static bool decodeRegularImage(ImageInfo& imageInfoOut, const tstring& filePath, bool flipY);

    static void setCreateTextureFromRawCb(const CreateTextureFromRawCb& cb) { m_createTextureFromRawData = cb; };

private:
    static CreateTextureFromRawCb m_createTextureFromRawData;
};

NS_END
