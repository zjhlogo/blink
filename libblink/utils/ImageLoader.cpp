#include "ImageLoader.h"

namespace blink
{
    ImageUtil::ImageFileType ImageUtil::getImageFileType(const tstring& filePath)
    {
        tstring strExt = FileUtil::getFileExtension(filePath);
        if (strExt == "png") return ImageFileType::Png;
        if (strExt == "pvr") return ImageFileType::Pvr;
        return ImageFileType::Unknown;
    }

    static void FuncReaderCallback(png_structp pPngStruct, png_bytep pData, png_size_t nSize)
    {
        BufferStreamReader* pBufferStreamReader = (BufferStreamReader*)png_get_io_ptr(pPngStruct);

        if (!pBufferStreamReader->read(pData, nSize))
        {
            png_error(pPngStruct, "FuncReaderCallback failed");
        }
    }

    /** setjmp buffer for error recovery. */
    static jmp_buf GSetjmpBuffer;

    static void pngUserErrorCallback(png_structp inPngStruct, png_const_charp inErrorMsg)
    {
        //	ImageInfo* context = (ImageInfo*)png_get_io_ptr(inPngStruct);
        LOGE("PNG Error: {}", inErrorMsg);

        longjmp(GSetjmpBuffer, 1);
    }

    static void pngUserWarningCallback(png_structp inPngStruct, png_const_charp inWarnMsg)
    {
        LOGW("PNG Warning: {}", inWarnMsg);
    }

    bool ImageUtil::decodePngImage(ImageInfo& imageInfoOut, const tstring& filePath)
    {
        BufferData fileBuffer;
        File file(filePath);
        if (!file.read(fileBuffer, file.fileSize()))
        {
            LOGE("Decode png file failed, can not read file {0}", filePath);
            return false;
        }
        file.close();
        png_sig_cmp((png_const_bytep)fileBuffer.data(), 0, 8);

        png_structp pPngStruct = png_create_read_struct(PNG_LIBPNG_VER_STRING, &imageInfoOut, pngUserErrorCallback, pngUserWarningCallback);
        png_infop pPngInfo = png_create_info_struct(pPngStruct);

        // define our own callback function for I/O instead of reading from a file
        BufferStreamReader bufferStream(fileBuffer);
        png_set_read_fn(pPngStruct, &bufferStream, FuncReaderCallback);

        png_read_info(pPngStruct, pPngInfo);

        int width = png_get_image_width(pPngStruct, pPngInfo);
        int height = png_get_image_height(pPngStruct, pPngInfo);
        // can be PNG_COLOR_TYPE_RGB, PNG_COLOR_TYPE_PALETTE, ...
        png_byte nColorType = png_get_color_type(pPngStruct, pPngInfo);
        int bpp = png_get_bit_depth(pPngStruct, pPngInfo);
        int channels = png_get_channels(pPngStruct, pPngInfo);

#if 1
        ASSERT(width > 0);
        ASSERT(height > 0);
        const bool bIsRawGray = ((nColorType & PNG_COLOR_MASK_COLOR) == 0);
        const bool bIsRawtRNS = !!png_get_valid(pPngStruct, pPngInfo, PNG_INFO_tRNS);

        const bool bOutputGray = bIsRawGray;
        const bool bOutputBGRA = false;

        const uint32 nOutBitDepth = 8;

        png_destroy_read_struct(&pPngStruct, &pPngInfo, nullptr);

        // Reset to the beginning of file so we can use png_read_png(), which expects to start at the beginning
        bufferStream.reset();

        pPngStruct = png_create_read_struct(PNG_LIBPNG_VER_STRING, &imageInfoOut, pngUserErrorCallback, pngUserWarningCallback);
        pPngInfo = png_create_info_struct(pPngStruct);
        png_set_read_fn(pPngStruct, &bufferStream, FuncReaderCallback);

        if (nColorType == PNG_COLOR_TYPE_PALETTE)
        {
            png_set_palette_to_rgb(pPngStruct);
        }

        if (bIsRawGray && bpp < 8)
        {
            png_set_expand_gray_1_2_4_to_8(pPngStruct);
        }

        // insert alpha channel with full opacity for RGB images without alpha
        if ((nColorType & PNG_COLOR_MASK_ALPHA) == 0 && !bOutputGray)
        {
            // png images don't set PNG_COLOR_MASK_ALPHA if they have alpha from a tRNS chunk,
            // but png_set_add_alpha seems to be safe regardless
            if (bIsRawGray)
            {
                //if (bInputtRNS)
                {
                    png_set_tRNS_to_alpha(pPngStruct);
                }
            }
            else if (nColorType == PNG_COLOR_TYPE_PALETTE)
            {
                png_set_tRNS_to_alpha(pPngStruct);
            }

            if (nOutBitDepth == 8)
            {
                png_set_add_alpha(pPngStruct, 0xff, PNG_FILLER_AFTER);
            }
            else if (nOutBitDepth == 16)
            {
                png_set_add_alpha(pPngStruct, 0xffff, PNG_FILLER_AFTER);
            }
        }

        // calculate Pixel Depth
        const uint32 nPixelChannels = (bOutputGray) ? 1 : 4;
        const uint32 nBytesPerPixel = (nOutBitDepth * nPixelChannels) / 8;
        const uint32 nBytesPerRow = nBytesPerPixel * width;
        channels = nPixelChannels;

        imageInfoOut.m_bufferData.resize(height * nBytesPerRow);
        png_bytep* rowPointers = (png_bytep*)png_malloc(pPngStruct, height * sizeof(png_bytep));
        for (int32 y = 0; y < height; ++y)
        {
            rowPointers[y] = imageInfoOut.m_bufferData.data() + (height - y - 1) * nBytesPerRow;
        }
        png_set_rows(pPngStruct, pPngInfo, rowPointers);

        uint32 nTransform = bOutputBGRA ? PNG_TRANSFORM_BGR : PNG_TRANSFORM_IDENTITY;

        // PNG files store 16-bit pixels in network byte order (big-endian, ie. most significant bits first).
        // We're little endian so we need to swap
        if (bpp == 16) nTransform |= PNG_TRANSFORM_SWAP_ENDIAN;

        // Convert grayscale png to RGB if requested
        if (bIsRawGray && !bOutputGray) nTransform |= PNG_TRANSFORM_GRAY_TO_RGB;

        // Convert RGB png to grayscale if requested
        if (!bIsRawGray && bOutputGray)
        {
            // This is not necessarily the best option, instead perhaps:
            //png_color background = {0,0,0};
            //png_set_background(png_ptr, &background, PNG_BACKGROUND_GAMMA_SCREEN, 0, 1.0);
            png_set_rgb_to_gray_fixed(pPngStruct, 2 /* warn if image is in color */, -1, -1);
        }

        // Strip alpha channel if requested output is grayscale
        if (bOutputGray) nTransform |= PNG_TRANSFORM_STRIP_ALPHA;

        // Reduce 16-bit to 8-bit if requested
        if (bpp == 16 && nOutBitDepth == 8)
        {
#if PNG_LIBPNG_VER >= 10504
            nTransform |= PNG_TRANSFORM_SCALE_16;
#else
            nTransform |= PNG_TRANSFORM_STRIP_16;
#endif
        }

        // Increase 8-bit to 16-bit if requested
        if (bpp <= 8 && nOutBitDepth == 16)
        {
#if PNG_LIBPNG_VER >= 10504
            nTransform |= PNG_TRANSFORM_EXPAND_16;
#else
            // Expanding 8-bit images to 16-bit via transform needs a libpng update
            ASSERT_NOT_IMPLEMENTED;
#endif
        }

        //png_read_update_info(pPngStruct, pPngInfo);

        if (!setjmp(GSetjmpBuffer))
        {
            png_read_png(pPngStruct, pPngInfo, nTransform, nullptr);
        }

        png_free(pPngStruct, rowPointers);

#else
        // convert stuff to appropriate formats!
        if (nColorType == PNG_COLOR_TYPE_PALETTE)
        {
            png_set_packing(pPngStruct);
            // expand data to 24-bit RGB or 32-bit RGBA if alpha available
            png_set_palette_to_rgb(pPngStruct);
        }

        // expand data to 24-bit RGB or 32-bit RGBA if alpha available
        if (nColorType == PNG_COLOR_TYPE_GRAY && bpp < 8) png_set_expand_gray_1_2_4_to_8(pPngStruct);
        if (nColorType == PNG_COLOR_TYPE_GRAY_ALPHA) png_set_gray_to_rgb(pPngStruct);
        if (bpp == 16) png_set_strip_16(pPngStruct);

        // expand palette or RGB images with transparency to full alpha channels so the data will be available as RGBA quartets.
        if (png_get_valid(pPngStruct, pPngInfo, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(pPngStruct);

        if (nColorType == PNG_COLOR_TYPE_RGB)
        {
            if (bpp <= 8) png_set_add_alpha(pPngStruct, 0xff, PNG_FILLER_AFTER);
            else if (bpp == 16) png_set_add_alpha(pPngStruct, 0xffff, PNG_FILLER_AFTER);
            channels = (channels == 3) ? 4 : channels;
        }

        // 	/* Flip the RGB pixels to BGR (or RGBA to BGRA) */
        // 	png_set_bgr(pPngStruct);

        imageInfoOut.m_bufferData.resize(width * height * channels);

        // read image data into pRowPointers
        std::vector<int8*> rowPointer;
        rowPointer.resize(height);
        for (int y = 0; y < height; y++)
        {
            rowPointer[y] = (int8*)imageInfoOut.m_bufferData.data() + (height - y - 1) * width * channels;		//each pixel(RGBA) has 4 bytes
        }

        if (!setjmp(GSetjmpBuffer))
        {
            png_read_image(pPngStruct, (png_bytepp)rowPointer.data());
        }
#endif

        // free the stream object and png structure
        png_destroy_read_struct(&pPngStruct, &pPngInfo, nullptr);

        imageInfoOut.m_width = width;
        imageInfoOut.m_height = height;
        imageInfoOut.m_channels = channels;

        return true;
    }

    static void FuncWriterCallback(png_structp pPngStruct, png_bytep pData, png_size_t nSize)
    {
        BufferStreamWriter* pBufferStreamWriter = (BufferStreamWriter*)png_get_io_ptr(pPngStruct);

        if (!pBufferStreamWriter->write(pData, nSize))
        {
            png_error(pPngStruct, "FuncWriterCallback failed");
        }
    }

    bool ImageUtil::savePngImage(const tstring& filePath, const ImageInfo& imageInfo)
    {
        // Initialize write structure
        png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        if (!png_ptr)
        {
            LOGE("Could not allocate write struct");
            return false;
        }

        // Initialize info structure
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr)
        {
            LOGE("Could not allocate info struct");
            png_destroy_write_struct(&png_ptr, (png_infopp)nullptr);
            return false;
        }

        // Setup Exception handling
        if (setjmp(png_jmpbuf(png_ptr)))
        {
            LOGE("Error during png creation");
            png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
            png_destroy_write_struct(&png_ptr, &info_ptr);
            return false;
        }

        BufferStreamWriter streamWriter(512 * 1024, 512 * 1024);
        png_set_write_fn(png_ptr, &streamWriter, FuncWriterCallback, nullptr);

        // Write header
        switch (imageInfo.m_channels)
        {
        case 1:
            png_set_IHDR(png_ptr, info_ptr, imageInfo.m_width, imageInfo.m_height, 8, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
            break;
        case 3:
            png_set_IHDR(png_ptr, info_ptr, imageInfo.m_width, imageInfo.m_height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
            break;
        case 4:
            png_set_IHDR(png_ptr, info_ptr, imageInfo.m_width, imageInfo.m_height, 8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
            break;
        default:
            png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
            png_destroy_write_struct(&png_ptr, &info_ptr);
            return false;
            break;
        }

        png_write_info(png_ptr, info_ptr);

        // Write image data
        for (int y = 0; y < imageInfo.m_height; ++y)
        {
            png_const_bytep row = &imageInfo.m_bufferData[y*imageInfo.m_width*imageInfo.m_channels];
            png_write_row(png_ptr, row);
        }

        // End write
        png_write_end(png_ptr, nullptr);

        png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
        png_destroy_write_struct(&png_ptr, &info_ptr);

        File file;
        if (!file.open(filePath, File::AM_WRITE)) return false;
        file.write(streamWriter.getData(), streamWriter.getSize());
        file.close();

        return true;
    }

    bool ImageUtil::createTextureFromRawData(TextureInfo& textureInfo, int width, int height, int channels, const BufferData& buffer)
    {
        GLint glFormat = GL_ALPHA;
        switch (channels)
        {
        case 1:
            glFormat = GL_ALPHA;
            break;
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
#if 1
        glTexImage2D(GL_TEXTURE_2D, 0, glFormat, width, height, 0, glFormat, GL_UNSIGNED_BYTE, buffer.data());
#else
        glTexImage2D(GL_TEXTURE_2D, 0, glFormat, width, height, 0, glFormat, GL_UNSIGNED_BYTE, nullptr);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, glFormat, GL_UNSIGNED_BYTE, buffer.data());
#endif
        glBindTexture(GL_TEXTURE_2D, 0);

        textureInfo.m_textureId = textureId;
        textureInfo.m_texSize = glm::ivec2(width, height);
        return true;
    }

    bool ImageUtil::loadTextureFromImage(ImageInfo& imageInfo, ImageFileType eImageFileType, const tstring& filePath)
    {
        switch (eImageFileType)
        {
        case ImageFileType::Png:
        {
            if (!decodePngImage(imageInfo, filePath)) return false;
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
