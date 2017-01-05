#ifndef __OFDOUTPUTDEV_H__
#define __OFDOUTPUTDEV_H__

#include <memory>
#include <cairo/cairo-ft.h>
#include <OutputDev.h>
#include "TextOutputDev.h"
#include <PDFDoc.h>
#include <GfxState.h>
#include "OFDCommon.h"
#include "OFDPackage.h"
#include "OFDDocument.h"
#include "OFDPage.h"
#include "OFDFont.h"

class OFDOutputDev;
typedef std::shared_ptr<OFDOutputDev> OFDOutputDevPtr;
typedef std::shared_ptr<PDFDoc> PDFDocPtr;

class OFDOutputDev : public OutputDev {
public:
    OFDOutputDev(ofd::OFDPackagePtr ofdPackage);
    virtual ~OFDOutputDev();

    void ProcessDoc(PDFDocPtr pdfDoc);
    void PrintFonts() const;

    void SetTextPage(TextPage *textPage);
    TextPage *GetTextPage() const {return m_textPage;};
    TextPage *TakeTextPage();

    void SetCairo(cairo_t *cairo); 
    void SetAntialias(cairo_antialias_t antialias);

    // Does this device use upside-down coordinates?
    // (Upside-down means (0,0) is the top left corner of the page.)
    virtual GBool upsideDown() { return gTrue; }

    // Does this device use drawChar() or drawString()?
    virtual GBool useDrawChar() { return gTrue; }

    // Does this device use tilingPatternFill()?  If this returns false,
    // tiling pattern fills will be reduced to a series of other drawing
    // operations.
    virtual GBool useTilingPatternFill() { return gTrue; }

    // Does this device use functionShadedFill(), axialShadedFill(), and
    // radialShadedFill()?  If this returns false, these shaded fills
    // will be reduced to a series of other drawing operations.
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 12, 0)
    virtual GBool useShadedFills(int type) { return type <= 7; }
#else
    virtual GBool useShadedFills(int type) { return type > 1 && type < 4; }
#endif

    // Does this device use FillColorStop()?
    virtual GBool useFillColorStop() { return gTrue; }

    // Does this device use beginType3Char/endType3Char?  Otherwise,
    // text in Type 3 fonts will be drawn with drawChar/drawString.
    virtual GBool interpretType3Chars() { return gFalse; }

    // Does this device need to clip pages to the crop box even when the
    // box is the crop box?
    virtual GBool needClipToCropBox() { return gTrue; }

    // Does this device need non-text content?
    virtual GBool needNonText() { return gTrue; }


    //----- initialization and control

    // Start a page.
    virtual void startPage(int pageNum, GfxState *state, XRef *xref);

    // End a page.
    virtual void endPage();

    //----- save/restore graphics state
    virtual void saveState(GfxState *state);
    virtual void restoreState(GfxState *state);

    //----- update text state
    virtual void updateFont(GfxState *state);
    virtual void updateAll(GfxState *state); 
    virtual void setDefaultCTM(double *ctm);
    virtual void updateCTM(GfxState *state, double m11, double m12,
            double m21, double m22, double m31, double m32);
    virtual void updateLineDash(GfxState *state);
    virtual void updateLineJoin(GfxState *state);
    virtual void updateLineCap(GfxState *state);
    virtual void updateMiterLimit(GfxState *state);
    virtual void updateLineWidth(GfxState *state);
    virtual void updateFillColor(GfxState *state);
    virtual void updateStrokeColor(GfxState *state);
    virtual void updateFillOpacity(GfxState *state);
    virtual void updateStrokeOpacity(GfxState *state);
    virtual void updateFillColorStop(GfxState *state, double offset);
    virtual void updateBlendMode(GfxState *state);

    //----- text drawing
    virtual void beginString(GfxState *state, GooString *s);
    virtual void endString(GfxState *state);
    virtual void drawChar(GfxState *state, double x, double y,
            double dx, double dy,
            double originX, double originY,
            CharCode code, int nBytes, Unicode *u, int uLen);
    virtual void incCharCount(int nChars);
    virtual void beginTextObject(GfxState *state);
    virtual void endTextObject(GfxState *state);
    virtual void beginActualText(GfxState *state, GooString *text);
    virtual void endActualText(GfxState *state);

    //----- path painting
    virtual void stroke(GfxState *state);
    virtual void fill(GfxState *state);
    virtual void eoFill(GfxState *state);
    virtual void clipToStrokePath(GfxState *state);
    virtual GBool tilingPatternFill(GfxState *state, Gfx *gfx, Catalog *cat, Object *str,
            double *pmat, int paintType, int tilingType, Dict *resDict,
            double *mat, double *bbox,
            int x0, int y0, int x1, int y1,
            double xStep, double yStep);
    virtual GBool functionShadedFill(GfxState *state, GfxFunctionShading *shading);
    virtual GBool axialShadedFill(GfxState *state, GfxAxialShading *shading, double tMin, double tMax);
    virtual GBool axialShadedSupportExtend(GfxState *state, GfxAxialShading *shading);
    virtual GBool radialShadedFill(GfxState *state, GfxRadialShading *shading, double sMin, double sMax);
    virtual GBool radialShadedSupportExtend(GfxState *state, GfxRadialShading *shading);
    virtual GBool gouraudTriangleShadedFill(GfxState *state, GfxGouraudTriangleShading *shading);
    virtual GBool patchMeshShadedFill(GfxState *state, GfxPatchMeshShading *shading);

private:

    PDFDocPtr m_pdfDoc;
    XRef *m_xref;
    TextPage *m_textPage;	    
    ActualText *m_actualText;

    ofd::OFDPackagePtr m_ofdPackage;
    ofd::OFDDocumentPtr m_ofdDocument;
    ofd::OFDPagePtr m_currentOFDPage;

    void processTextLine(TextLine *line, ofd::OFDLayerPtr bodyLayer);
    void processTextPage(TextPage *textPage, ofd::OFDPagePtr currentOFDPage);


public:

    // CropBox (pixel)
    int m_cropX;
    int m_cropY;
    int m_cropW;
    int m_cropH;
    int m_paperWidth;
    int m_paperHeight;
    bool m_usePDFPageSize;
    bool m_printing;
    bool m_useCropBox;
    bool m_expand;
    bool m_noShrink;
    bool m_transp;
    bool m_noCenter;
    double m_resolution;
    double m_resolutionX; 
    double m_resolutionY;
    int m_scaleTo;
    int m_scaleToX;
    int m_scaleToY;

    cairo_surface_t *m_outputSurface;
    FILE *m_outputFile;
    cairo_t *m_cairo;
    cairo_matrix_t m_origMatrix;

    cairo_pattern_t *m_groupPattern;
    cairo_pattern_t *m_shapePattern;
    cairo_pattern_t *m_maskPattern;
    cairo_matrix_t m_mask_matrix;
    cairo_surface_t *m_cairoShapeSurface;

    cairo_t *m_cairoShape;
    cairo_path_t *m_textClipPath;
    GfxRGB m_strokeColor;
    GfxRGB m_fillColor;
    double m_strokeOpacity;
    double m_fillOpacity;
    bool m_uncoloredPattern;
    bool m_strokeAdjust;
    bool m_adjustedStrokeWidth;
    bool m_alignStrokeCoords;
    bool m_needFontUpdate;

    bool m_inUncoloredPattern;     // inside a uncolored pattern (PaintType = 2)
    cairo_antialias_t m_antialiasEnum;

    bool m_useShowTextGlyphs;
    cairo_text_cluster_t *m_cairoTextClusters;
    cairo_glyph_t *m_cairoGlyphs;
    int m_clustersCount;
    int m_glyphsCount;
    char *m_utf8;
    int m_utf8Count;
    int m_utf8Max;
    bool m_textMatrixValid;
    ofd::OFDFontPtr m_currentFont;
    double m_currentFontSize;
    double *m_currentCTM;
    cairo_pattern_t *m_fillPattern, *m_strokePattern;
    cairo_antialias_t m_antialias;

    struct StrokePathClip {
        GfxPath *path;
        cairo_matrix_t ctm;
        double line_width;
        double *dashes;
        int dash_count;
        double dash_offset;
        cairo_line_cap_t cap;
        cairo_line_join_t join;
        double miter;
        int ref_count;
    } * m_strokePathClip;

protected:
    void fillToStrokePathClip(GfxState *state);
    void doPath(cairo_t *cairo, GfxState *state, GfxPath *path);
    void alignStrokeCoords(GfxSubpath *subpath, int i, double *x, double *y);

private:
    void getCropSize(double page_w, double page_h, double *width, double *height); 
    void getOutputSize(double page_w, double page_h, double *width, double *height);
    void getFitToPageTransform(double page_w, double page_h, double paper_w, double paper_h, cairo_matrix_t *m); 
    std::tuple<double, double> getPageSize(PDFDocPtr pdfDoc, int pg, int firstPage);

    void writePageImage(const std::string &filename);

    std::tuple<cairo_surface_t*, FILE*> beforeDocument(const std::string &outputFileName, double w, double h); 
    void afterDocument();
    void beforePage(double w, double h);
    void afterPage(const std::string &imageFileName);
    void renderPage(int pg, double page_w, double page_h, double output_w, double output_h); 

}; // class OFDOutputDev

#endif // __OFDOUTPUTDEV_H__
