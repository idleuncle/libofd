#ifndef __UTILS_IMAGESTREAM_H__
#define __UTILS_IMAGESTREAM_H__

namespace utils{

    typedef struct ImageDataHead{
        int Width;
        int Height;
        int Components;
        int Bits;

        ImageDataHead(int width = 0, int height = 0, int comps = 0, int bits = 0) :
            Width(width), Height(height), Components(comps), Bits(bits)
        {
        }

        int GetLineSize() const{
            int nVals = Width * Components;
            int lineSize = (nVals * Bits + 7) >> 3;
            return lineSize;
        }
    } ImageDataHead_t;

}; // namespace utils

#endif // __UTILS_IMAGESTREAM_H__
