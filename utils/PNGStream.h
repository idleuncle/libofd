#ifndef __UTILS_PNGSTREAM_H__
#define __UTILS_PNGSTREAM_H__

#include <tuple>
#include "utils/ImageStream.h"

namespace utils{

std::tuple<ImageDataHead, char*, size_t> LoadPNGData(char* data, size_t dataSize);

}; // namespace utils

#endif // __UTILS_PNGSTREAM_H__
