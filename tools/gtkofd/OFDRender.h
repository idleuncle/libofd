#ifndef __OFDRENDER_H__
#define __OFDRENDER_H__

#include <memory>
#include "ofd/Common.h"
#include "ofd/Document.h"
#include "ofd/CairoRender.h"

//struct _cairo_surface;
namespace ofd{

class OFDRender{
public:
    OFDRender(int screenWidth, int screenHeight, int screenBPP);
    virtual ~OFDRender();

    bool RebuildBackgroundImage(int screenWidth, int screenHeight, int screenBPP = 32);
    void RenderBackgroundImage(DocumentPtr document, size_t pageIndex);
    std::shared_ptr<ofd::CairoRender> GetCairoRender() const{
        return m_cairoRender;
    }

    virtual void UpdateScreenRenderer();
    virtual void UpdateScreen();

    void MoveUp();
    void MoveDown();
    void MoveLeft();
    void MoveRight();
    void ZoomIn();
    void ZoomOut();
    void ZoomFitBest();
    void SetOffsetX(double offsetX){m_offsetX = offsetX;};
    void SetOffsetY(double offsetY){m_offsetY = offsetY;};
    void SetScaling(double scaling){m_scaling = scaling;};

protected:
    int m_screenWidth = -1;
    int m_screenHeight = 0;
    int m_screenBPP = 32;
    double m_offsetX = 0.0;
    double m_offsetY = 0.0;
    double m_scaling = 1.0;
    double m_zoomFactor = exp(1.0);


private:
    //_cairo_surface* createCairoSurface();
    //void onRender(_cairo_surface *surface);

    std::shared_ptr<ofd::CairoRender> m_cairoRender = nullptr;
}; // class OFDRender

typedef std::shared_ptr<OFDRender> OFDRenderPtr;

}

#endif // __OFD_RENDER_H__
