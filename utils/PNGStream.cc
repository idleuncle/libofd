#include <stdint.h>
#include <inttypes.h>
#include <limits.h>
#include <string>
#include <tuple>
#include <png.h>

#include "PNGStream.h"
using namespace utils;

// **************** class ofd::PNGStream ****************
class PNGStream{

    public:
        PNGStream();
        virtual ~PNGStream();

        bool Open(const std::string pngFileName);
        void Close();
        void Reset();

        inline int DoGetChars(int nChars, uint8_t *buffer);


}; // class PNGStream

class ImageSource {
    public:
        ImageSource(char *dataA, size_t dataSizeA) : data(dataA), dataSize(dataSizeA), offset(0){};

        char *data;
        size_t dataSize;
        size_t offset;
};

static void pngReadCallback(png_structp png_ptr, png_byte *data, png_size_t length){
    ImageSource *imageSource = (ImageSource*)png_get_io_ptr(png_ptr);
    if ( imageSource->offset + length <= imageSource->dataSize ){
        memcpy((void*)data, (void*)(imageSource->data + imageSource->offset), length);
        imageSource->offset += length;
    } else {
        png_error(png_ptr, "pngReadCallback() failed.");
    }
}

PNGStream::PNGStream(){
}

PNGStream::~PNGStream(){
}

void PNGStream::Reset(){
}

void PNGStream::Close(){
}

int PNGStream::DoGetChars(int nChars, uint8_t *buffer){
    int dataSize = 0;
    return dataSize;
}

namespace utils{

std::tuple<ImageDataHead, char*, size_t> LoadPNGData(char* data, size_t dataSize){
    ImageDataHead imageDataHead;
    char *imageData = nullptr;
    size_t imageDataSize = 0; 

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
    if ( png_ptr != 0 ){
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if ( info_ptr != 0 ){
            if ( setjmp(png_jmpbuf(png_ptr)) == 0 ){
                ImageSource imageSource(data, dataSize);
                
                png_set_read_fn(png_ptr, (png_voidp)&imageSource, pngReadCallback);
                png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND /*| PNG_TRANSFORM_STRIP_ALPHA*/, 0);
                // 图像的色彩类型
                int color_type = png_get_color_type(png_ptr, info_ptr);
                // 图像的宽高
                int w = png_get_image_width(png_ptr, info_ptr);
                int h = png_get_image_height(png_ptr, info_ptr);
                imageDataHead.Width = w;
                imageDataHead.Height = h;
                imageDataHead.Components = 4;
                imageDataHead.Bits = 8;

                imageDataSize = (size_t)w * (size_t)h * 4;
                imageData = (char*)new char[imageDataSize];
                size_t idx = 0;

                // 获得图像的所有行像素数据
                png_bytep *row_pointers = png_get_rows(png_ptr, info_ptr);
                switch ( color_type ){
                    case PNG_COLOR_TYPE_RGB_ALPHA:

                        for ( int y = 0 ; y < h ; ++y ){
                            for ( int x = 0 ; x < w * 4; ){
                                __attribute__((unused)) uint8_t red = row_pointers[y][x++];
                                __attribute__((unused)) uint8_t green = row_pointers[y][x++];
                                __attribute__((unused)) uint8_t blue = row_pointers[y][x++];
                                __attribute__((unused)) uint8_t alpha = row_pointers[y][x++];
                                imageData[idx++] = red;
                                imageData[idx++] = green;
                                imageData[idx++] = blue;
                                imageData[idx++] = alpha;
                            }
                        }
                        break;

                    case PNG_COLOR_TYPE_RGB:

                        for ( int y = 0 ; y < h ; ++y ){
                            for ( int x = 0 ; x < w * 3; ){
                                __attribute__((unused)) uint8_t red = row_pointers[y][x++];
                                __attribute__((unused)) uint8_t green = row_pointers[y][x++];
                                __attribute__((unused)) uint8_t blue = row_pointers[y][x++];
                                __attribute__((unused)) uint8_t alpha = 255;

                                imageData[idx++] = red;
                                imageData[idx++] = green;
                                imageData[idx++] = blue;
                                imageData[idx++] = alpha;
                            }
                        }
                        break;

                    default:
                        break;

                }; // switch ( color_type )

            }
            png_destroy_read_struct(&png_ptr, &info_ptr, 0);
        } else {
            png_destroy_read_struct(&png_ptr, 0, 0);
        }
    }

    return std::make_tuple(imageDataHead, imageData, imageDataSize);
}

}; // namespace utils
