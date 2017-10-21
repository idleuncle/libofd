
#include <stdint.h>
#include "CairoRescaleBox.h"
#include <GfxState.h>
#include <OutputDev.h>
#include "utils/utils.h"

using namespace utils;

// poppler/poppler/CairoOutputDev.cc

class RescaleDrawImage : public CairoRescaleBox {
private:
  ImageStream *imgStr;
  GfxRGB *lookup;
  int width;
  GfxImageColorMap *colorMap;
  int nComps;
  int nBits;
  int *maskColors;
  int current_row;
  GBool imageError;

public:
  RescaleDrawImage() :
    imgStr(nullptr), lookup(nullptr),
    width(-1), colorMap(nullptr), nComps(-1), nBits(-1),
    maskColors(nullptr), current_row(-1), imageError(gFalse){

  }

  cairo_surface_t *getSourceImage(Stream *str,
                                  int widthA, int heightA,
                                  int scaledWidth, int scaledHeight,
                                  GBool printing,
                                  GfxImageColorMap *colorMapA,
                                  int *maskColorsA) {

      colorMap = colorMapA;
      int nCompsA = colorMap->getNumPixelComps();
      int nBitsA = colorMap->getBits();
      return getSourceImage(str, widthA, heightA, scaledWidth, scaledHeight, printing, nCompsA, nBitsA, maskColorsA);
  }

  cairo_surface_t *getSourceImage(Stream *str,
                                  int widthA, int height,
                                  int scaledWidth, int scaledHeight,
                                  GBool printing,
                                  int nCompsA,
                                  int nBitsA,
                                  //GfxImageColorMap *colorMapA,
                                  int *maskColorsA) {
    cairo_surface_t *image = NULL;
    //int i;

    lookup = NULL;
    //colorMap = colorMapA;
    nComps = nCompsA;
    nBits = nBitsA;
    maskColors = maskColorsA;
    width = widthA;
    current_row = -1;
    imageError = gFalse;

    /* TODO: Do we want to cache these? */
    imgStr = new ImageStream(str, width,
            nComps, nBits);
                             //colorMap->getNumPixelComps(),
                             //colorMap->getBits());
    imgStr->reset();

#if 0
    /* ICCBased color space doesn't do any color correction
     * so check its underlying color space as well */
    int is_identity_transform;
    is_identity_transform = colorMap->getColorSpace()->getMode() == csDeviceRGB ||
      (colorMap->getColorSpace()->getMode() == csICCBased &&
       ((GfxICCBasedColorSpace*)colorMap->getColorSpace())->getAlt()->getMode() == csDeviceRGB);
#endif

    // special case for one-channel (monochrome/gray/separation) images:
    // build a lookup table here
    if ( colorMap != nullptr && colorMap->getNumPixelComps() == 1) {
        int n;
        Guchar pix;

        n = 1 << colorMap->getBits();
        lookup = (GfxRGB *)gmallocn(n, sizeof(GfxRGB));
        for (int i = 0; i < n; ++i) {
            pix = (Guchar)i;

            colorMap->getRGB(&pix, &lookup[i]);
        }
    }

    if (printing || scaledWidth >= width || scaledHeight >= height) {
      // No downscaling. Create cairo image containing the source image data.
      unsigned char *buffer;
      int stride;

      image = cairo_image_surface_create (maskColors ?
                                          CAIRO_FORMAT_ARGB32 :
                                          CAIRO_FORMAT_RGB24,
                                          width, height);
      if (cairo_surface_status (image))
        goto cleanup;

      buffer = cairo_image_surface_get_data (image);
      stride = cairo_image_surface_get_stride (image);
      for (int y = 0; y < height; y++) {
        uint32_t *dest = (uint32_t *) (buffer + y * stride);
        getRow(y, dest);
      }
    } else {
      // // Downscaling required. Create cairo image the size of the
      // rescaled image and // downscale the source image data into
      // the cairo image. downScaleImage() will call getRow() to read
      // source image data from the image stream. This avoids having
      // to create an image the size of the source image which may
      // exceed cairo's 32676x32767 image size limit (and also saves a
      // lot of memory).
      image = cairo_image_surface_create (maskColors ?
                                          CAIRO_FORMAT_ARGB32 :
                                          CAIRO_FORMAT_RGB24,
                                          scaledWidth, scaledHeight);
      if (cairo_surface_status (image))
        goto cleanup;

      downScaleImage(width, height,
                     scaledWidth, scaledHeight,
                     0, 0, scaledWidth, scaledHeight,
                     image);
    }
    cairo_surface_mark_dirty (image);

  cleanup:
    gfree(lookup);
    imgStr->close();
    delete imgStr;
    return image;
  }

  void getRow(int row_num, uint32_t *row_data) {
    int i;
    Guchar *pix;

    if (row_num <= current_row)
      return;

    while (current_row  < row_num) {
      pix = imgStr->getLine();
      current_row++;
    }

    if (unlikely(pix == NULL)) {
        memset(row_data, 0, width*4);
        if (!imageError) {
            error(errInternal, -1, "Bad image stream");
            imageError = gTrue;
        }
    } else if (lookup) {
        Guchar *p = pix;
        GfxRGB rgb;

        for (i = 0; i < width; i++) {
            rgb = lookup[*p];
            row_data[i] =
                ((int) colToByte(rgb.r) << 16) |
                ((int) colToByte(rgb.g) << 8) |
                ((int) colToByte(rgb.b) << 0);
            p++;
        }
    } else {
        // FIXME
        if ( colorMap != nullptr ){
            colorMap->getRGBLine (pix, row_data, width);
        } else {
            //memcpy(row_data, pix, width * 4);
            for ( int x = 0; x < width ; x++ ){
                unsigned char *b = (unsigned char*)row_data;
                for ( int i = 0 ; i < nComps ; ++i){
                    b[i] = pix[i];
                }
                row_data++;
                pix += nComps;
            }
        }
    }

    if ( maskColors != nullptr ) {
        for (int x = 0; x < width; x++) {
            bool is_opaque = false;
            for (int i = 0; i < nComps; ++i) {
                if (pix[i] < maskColors[2*i] ||
                        pix[i] > maskColors[2*i+1]) {
                    is_opaque = true;
                    break;
                }
            }
            if (is_opaque)
                *row_data |= 0xff000000;
            else
                *row_data = 0;
            row_data++;
            pix += nComps;
        }
        }
    }

};

#include <math.h>

namespace ofd{
    cairo_surface_t *createImageSurface(char *imageData, size_t imageDataSize, int widthA, int heightA, int scaledWidth, int scaledHeight, int nComps, int nBits){  

        MemStream *memStream = new MemStream(imageData, 0, imageDataSize, Object(objNull));
        //ofd::MemStream *memStream = new ofd::MemStream(imageData, 0, imageDataSize);
        //memStream->reset();

        RescaleDrawImage rescale;
        cairo_surface_t *imageSurface = rescale.getSourceImage(memStream, widthA, heightA, scaledWidth, scaledHeight, false, nComps, nBits, nullptr);//colorMap, maskColors);

        // FIXME
        //delete memStream;

        return imageSurface;
    }


    cairo_surface_t *createImageSurface(Stream *str, int widthA, int heightA, int scaledWidth, int scaledHeight, GfxImageColorMap *colorMap, int *maskColors){  
        RescaleDrawImage rescale;
        cairo_surface_t *imageSurface = rescale.getSourceImage(str, widthA, heightA, scaledWidth, scaledHeight, false, colorMap, maskColors);
        return imageSurface;
    }


    /* Taken from cairo/doc/tutorial/src/singular.c */
    static void get_singular_values (const cairo_matrix_t *matrix, double *major, double *minor){
        double xx = matrix->xx, xy = matrix->xy;
        double yx = matrix->yx, yy = matrix->yy;

        double a = xx*xx+yx*yx;
        double b = xy*xy+yy*yy;
        double k = xx*xy+yx*yy;

        double f = (a+b) * .5;
        double g = (a-b) * .5;
        double delta = sqrt (g*g + k*k);

        if (major)
            *major = sqrt (f + delta);
        if (minor)
            *minor = sqrt (f - delta);
    }

    void getImageScaledSize(const cairo_matrix_t *matrix,
            int                   orig_width,
            int                   orig_height,
            int                  *scaledWidth,
            int                  *scaledHeight) {
        double xScale;
        double yScale;
        if (orig_width > orig_height)
            get_singular_values(matrix, &xScale, &yScale);
        else
            get_singular_values (matrix, &yScale, &xScale);

        int tx, tx2, ty, ty2; /* the integer co-oridinates of the resulting image */
        if (xScale >= 0) {
            tx = splashRound(matrix->x0 - 0.01);
            tx2 = splashRound(matrix->x0 + xScale + 0.01) - 1;
        } else {
            tx = splashRound(matrix->x0 + 0.01) - 1;
            tx2 = splashRound(matrix->x0 + xScale - 0.01);
        }
        *scaledWidth = abs(tx2 - tx) + 1;
        //scaledWidth = splashRound(fabs(xScale));
        if (*scaledWidth == 0) {
            // technically, this should draw nothing, but it generally seems
            // better to draw a one-pixel-wide stripe rather than throwing it
            // away
            *scaledWidth = 1;
        }
        if (yScale >= 0) {
            ty = splashFloor(matrix->y0 + 0.01);
            ty2 = splashCeil(matrix->y0 + yScale - 0.01);
        } else {
            ty = splashCeil(matrix->y0 - 0.01);
            ty2 = splashFloor(matrix->y0 + yScale + 0.01);
        }
        *scaledHeight = abs(ty2 - ty);
        if (*scaledHeight == 0) {
            *scaledHeight = 1;
        }
    }

    cairo_filter_t getFilterForSurface(cairo_surface_t *image, cairo_t *cr, bool interpolate) {
        if (interpolate)
            return CAIRO_FILTER_BILINEAR;

        int orig_width = cairo_image_surface_get_width(image);
        int orig_height = cairo_image_surface_get_height(image);
        if (orig_width == 0 || orig_height == 0)
            return CAIRO_FILTER_NEAREST;

        /* When printing, don't change the interpolation. */
        //if ( m_printing )
        //return CAIRO_FILTER_NEAREST;

        cairo_matrix_t matrix;
        cairo_get_matrix(cr, &matrix);
        int scaled_width, scaled_height;
        getImageScaledSize(&matrix, orig_width, orig_height, &scaled_width, &scaled_height);

        /* When scale factor is >= 400% we don't interpolate. See bugs #25268, #9860 */
        if (scaled_width / orig_width >= 4 || scaled_height / orig_height >= 4)
            return CAIRO_FILTER_NEAREST;

        return CAIRO_FILTER_BILINEAR;
    }

}

