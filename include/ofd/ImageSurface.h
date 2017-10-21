#ifndef __OFD_IMAGESURFACE_H__
#define __OFD_IMAGESURFACE_H__

#include <cairo/cairo.h>

namespace ofd{

    class ImageSurface{
    public:
        ImageSurface(char *imageData, size_t imageDataSize, int widthA, int heightA, int scaledWidth, int scaledHeight, int nComps, int nBits);
        virtual ~ImageSurface();
        cairo_surface_t *GetCairoSurface() const;       
    private:
        class ImplCls;
        std::unique_ptr<ImplCls> m_impl;
    };


}; // namespace ofd

#endif // __OFD_IMAGESURFACE_H__
