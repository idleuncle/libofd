#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <setjmp.h>  
#include <math.h>  
#include <sys/time.h>  
#include <time.h>  
#include <assert.h>
#include <tuple>
  
// jpeg库头文件必须放到stdio.h后面  
#include "jpeglib.h"  
#include "jerror.h"  

#include "JPEGStream.h"
#include "utils.h"
using namespace utils;
  
// 参考 libjpeg学习2：内存篇 http://blog.csdn.net/subfate/article/details/46700675
// poppler/poppler/JPEG2000Stream.cc

struct my_error_mgr{
    struct jpeg_error_mgr pub; 
    jmp_buf setjmp_buffer;
};

typedef struct my_error_mgr * my_error_ptr;  
  
void my_error_exit (j_common_ptr cinfo)  
{  
    my_error_ptr myerr = (my_error_ptr) cinfo->err;  
  
    (*cinfo->err->output_message) (cinfo);  
  
    longjmp(myerr->setjmp_buffer, 1);  
}  
  
std::tuple<char*, size_t, int, int, int> jpeg2rgb(unsigned char* jpeg_buffer, int jpeg_size) {  
    struct jpeg_decompress_struct cinfo;  
    struct my_error_mgr jerr;  
  
    JSAMPARRAY buffer;  
    int row_stride = 0;  
    unsigned char* tmp_buffer = nullptr;  
    unsigned char* imageData = nullptr;
    int rgb_size;  
  
    assert(jpeg_buffer != nullptr);
      
    cinfo.err = jpeg_std_error(&jerr.pub);  
    jerr.pub.error_exit = my_error_exit;  
  
    if (setjmp(jerr.setjmp_buffer))  
    {  
        jpeg_destroy_decompress(&cinfo);  
        return std::make_tuple(nullptr, 0, 0, 0, 0);
    }  
  
    jpeg_create_decompress(&cinfo);  
  
    jpeg_mem_src(&cinfo, jpeg_buffer, jpeg_size);  
  
    jpeg_read_header(&cinfo, TRUE);  
  
    //cinfo.out_color_space = JCS_RGB; //JCS_YCbCr;  // 设置输出格式  
  
    jpeg_start_decompress(&cinfo);  
  
    row_stride = cinfo.output_width * cinfo.output_components;  
    int imageWidth = cinfo.output_width;  
    int imageHeight = cinfo.output_height;  
    int imageComponents = cinfo.output_components;
  
    rgb_size = row_stride * cinfo.output_height; // 总大小  
    //if (*size < rgb_size)  
    //{  
        //printf("rgb buffer to small, we need %d but has only: %d\n", rgb_size, *size);  
    //}  
      
    int imageSize = rgb_size;  
    imageData = (unsigned char*)new unsigned char[rgb_size];
  
    buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);  
  
    printf("debug--:\nrgb_size: %d, size: %d w: %d h: %d row_stride: %d \n", rgb_size,  
                cinfo.image_width*cinfo.image_height*3,  
                cinfo.image_width,   
                cinfo.image_height,  
                row_stride);  
    tmp_buffer = imageData + (cinfo.image_height - 1) * row_stride;  
    while (cinfo.output_scanline < cinfo.output_height) // 解压每一行  
    {  
        jpeg_read_scanlines(&cinfo, buffer, 1);  
        //// 复制到内存  
        if (utils::IsColorBGR()){
            memcpy(tmp_buffer, buffer[0], row_stride);  
        } else {
            for ( auto n = 0 ; n < row_stride ; n += imageComponents ){
                for ( auto k = 0 ; k < imageComponents ; k++ ){
                    tmp_buffer[n+k] = buffer[0][n+imageComponents-k-1]; 
                }
            }
        }
        tmp_buffer -= row_stride;  
    }  
  
    jpeg_finish_decompress(&cinfo);  
    jpeg_destroy_decompress(&cinfo);  
  
    return std::make_tuple((char*)imageData, imageSize, imageWidth, imageHeight, imageComponents);  
}  
  
int rgb2jpeg(unsigned char* rgb_buffer, int width, int height, int quality, unsigned char** jpeg_buffer, unsigned long* jpeg_size)  
{  
    struct jpeg_compress_struct cinfo;  
    struct jpeg_error_mgr jerr;  
    int row_stride = 0;  
  
    if (jpeg_buffer == NULL)  
    {  
        printf("you need a pointer for jpeg buffer.\n");  
        return -1;  
    }  
      
    cinfo.err = jpeg_std_error(&jerr);  
  
    jpeg_create_compress(&cinfo);  
  
    jpeg_mem_dest(&cinfo, jpeg_buffer, jpeg_size);  
  
    cinfo.image_width = width;  
    cinfo.image_height = height;  
    cinfo.input_components = 3;  
    cinfo.in_color_space = JCS_RGB;  
  
    jpeg_set_defaults(&cinfo);  
    jpeg_set_quality(&cinfo, quality, (boolean)1);  // todo 1 == true  
    jpeg_start_compress(&cinfo, TRUE);  
    row_stride = width * cinfo.input_components;  
  
    while (cinfo.next_scanline < cinfo.image_height)  {  
        JSAMPROW row_pointer[1];  
        row_pointer[0] = &rgb_buffer[cinfo.next_scanline * row_stride];  
        jpeg_write_scanlines(&cinfo, row_pointer, 1);  
    }  
  
    jpeg_finish_compress(&cinfo);  
    jpeg_destroy_compress(&cinfo);  
  
    return 0;  
}  

namespace utils {

std::tuple<ImageDataHead, char*, size_t> LoadJPEGData(char *data, size_t dataSize){
    ImageDataHead imageDataHead;
    char *imageData = nullptr;
    size_t imageDataSize = 0;
    int width = 0;
    int height = 0;
    int components = 0;
    std::tie(imageData, imageDataSize, width, height, components) = jpeg2rgb((unsigned char*)data, dataSize);

    imageDataHead.Width = width;
    imageDataHead.Height = height;
    imageDataHead.Components = components; // default 4
    imageDataHead.Bits = 8;

    return std::make_tuple(imageDataHead, imageData, imageDataSize);
}

}; // namespace utils
