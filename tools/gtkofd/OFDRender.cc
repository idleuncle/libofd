#include <cairo/cairo.h>
#include "ofd/Document.h"
#include "ofd/Page.h"
#include "utils/logger.h"
#include "OFDRender.h"

double g_resolutionX = 240.0; // 120, 160, 240
double g_resolutionY = 240.0;
double ZOOM_BASE = exp(1.0);

using namespace ofd;

OFDRender::OFDRender(int screenWidth, int screenHeight, int screenBPP) :
    m_screenWidth(screenWidth), m_screenHeight(screenHeight),
    m_screenBPP(screenBPP)
{
    RebuildBackgroundImage(m_screenWidth, m_screenHeight, m_screenBPP);
}

OFDRender::~OFDRender(){
}

bool OFDRender::RebuildBackgroundImage(int screenWidth, int screenHeight, int screenBPP){

    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;
    m_screenBPP = screenBPP;

    if (m_cairoRender != nullptr){
        m_cairoRender->Rebuild(m_screenWidth, m_screenHeight, g_resolutionX, g_resolutionY);
    } else {
        m_cairoRender = utils::make_unique<ofd::CairoRender>(m_screenWidth, m_screenHeight, g_resolutionX, g_resolutionY);
    }

    return true;
}

//cairo_surface_t* OFDRender::createCairoSurface(unsigned char *pixels){
    //cairo_surface_t *cairoSurface = cairo_image_surface_create_for_data(
            //pixels,
            //CAIRO_FORMAT_ARGB32,
            //m_screenWidth,
            //m_screenHeight,
            //m_screenBPP,
            //);
    //assert(cairoSurface != nullptr);
    //return cairoSurface;
//}

void OFDRender::RenderBackgroundImage(DocumentPtr document, size_t pageIndex){
    //cairo_surface_t *cairoSurface = createCairoSurface();
    //onRender(cairoSurface);
    //cairo_surface_destroy(cairoSurface);

    if ( document != nullptr && m_cairoRender != nullptr ){
        size_t totalPages = document->GetNumPages();
        if ( totalPages > 0 ){
            PagePtr page = document->GetPage(pageIndex);
            if ( page->Open() ){
                //std::unique_ptr<OFDCairoRender> m_cairoRender(new OFDCairoRender(m_screenWidth, m_screenHeight, g_resolutionX, g_resolutionY));

                /***
                 * viewArea {offsetX, offsetY, scaling}
                 * (offsetX, offsetY) 显示窗口左上角坐标与页面原点（左上角）的像素偏移。
                 * scaling - 缩放比例
                 *
                 ***/
                double offsetX = m_offsetX;
                double offsetY = m_offsetY;
                double scaling = m_scaling;

                double fitScaling = page->GetFitScaling(m_screenWidth, m_screenHeight, g_resolutionX, g_resolutionY);

                //double factor = 1.0;
                //double delta = m_zoomFactor - ZOOM_BASE;
                //if ( delta >= 0.0 ){
                    //factor = log(m_zoomFactor);
                //} else {
                    //factor = 1.0 - (1.0 / ( 1.0 + exp(delta)) - 0.5) * 2;
                //}
                //scaling = fitScaling * factor;
                scaling = fitScaling;
                m_scaling = scaling;

                //LOG(DEBUG) << "delta: " << delta << " factor: " << factor << " scaling=" << scaling << " offset=(" << pixelX << "," << pixelY << ")";

                m_cairoRender->SaveState();{

                    ofd::ViewArea viewArea = std::make_tuple(offsetX, offsetY, scaling);
                    m_cairoRender->DrawPage(page, viewArea);
                }
                m_cairoRender->RestoreState();
            } else {
                LOG(ERROR) << "page->Open() failed. pageIndex=" << pageIndex;
            }
        }
    }
}

void OFDRender::UpdateScreenRenderer(){
}

void OFDRender::UpdateScreen(){
}
