#ifndef __UTILS_JPEGSTREAM_H__
#define __UTILS_JPEGSTREAM_H__

#include <tuple>

/** 
 * 利用libjpeg将缓冲区的JPEG转换成RGB 解压JPEG 
 *  
 * @param[IN]  jpeg_buffer  JPEG图片缓冲区 
 * @param[IN]  jpeg_size    JPEG图片缓冲区大小 
 * @param[IN] rgb_buffer    RGB缓冲区 
 * @param[IN/OUT] size      RGB缓冲区大小 
 * @param[OUT] width        图片宽 
 * @param[OUT] height       图片高 
 * 
 * @return  
 *         0：成功 
 *         -1：打开文件失败 
 * @note 
 *         jpeg、rgb空间由调用者申请，size为输入输出参数，传入为rgb空间大小，转出为rgb实际大小 
 */  
 std::tuple<char*, size_t, int, int, int> jpeg2rgb(unsigned char* jpeg_buffer, int jpeg_size);
  
/** 
 * 利用libjpeg将缓冲区的RGB转换成JPEG 压缩为JPEG 
 *  
 * @param[IN]  rgb_buffer JPEG图片RGB数据 
 * @param[IN]  width       图片宽 
 * @param[IN]  height      图片高 
 * @param[IN]  quality     图片质量 
 * @param[OUT] jpeg_buffer JPEG缓冲区指针 
 * @param[OUT] jpeg_size   JPEG缓冲区大小 
 * 
 * @return  
 *         0：成功 
 *         -1：打开文件失败 
 * @note 
 *         jpeg_buffer为二级指针，无须调用者申请空间，由libjpeg申请，但调用者要自行释放 
 */  
int rgb2jpeg(unsigned char* rgb_buffer, int width, int height, int quality, unsigned char** jpeg_buffer, unsigned long* jpeg_size);  

#endif // __UTILS_JPEGSTREAM_H__
