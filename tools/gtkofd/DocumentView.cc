#include <assert.h>
#include "utils/logger.h"
#include "utils/Geometry.h"
#include "DocumentView.h"

//defined in OFDRender
extern double g_resolutionX;

using namespace ofd;

DocumentView::DocumentView(PackageViewPtr packageView)
    : m_packageView(packageView)
{
}

DocumentView::~DocumentView(){
}

void DocumentView::SetDocument(DocumentPtr document){
    m_document = document;

}

void DocumentView::Rebuild(){
    if (m_document != nullptr){
        if (m_pageWall != nullptr){
            //m_pageWall->SetWallViewArea(0, 0, 1.0);
            m_pageWall->Rebuild(m_document, m_rowPages);
        } else {
            LOG_WARN("DocumentView::SetDocument() m_pageWall == nullptr");
        }
    } else {
        LOG_WARN("DocumentView::SetDocument() document == nullptr");
        m_pageWall->Rebuild(nullptr, m_rowPages);
    }
}

void DocumentView::RedrawDocumentView(){
    if (m_drawingArea != nullptr){
        gdk_window_invalidate_rect(gtk_widget_get_window(m_drawingArea), nullptr, false);
    }
}

void DocumentView::OnSize(int width, int height){
    //if (m_ofdRender != nullptr){
        //m_ofdRender->RebuildBackgroundImage(allocation->width, allocation->height);
    //} else {
        //m_ofdRender = std::make_shared<ofd::OFDRender>(drawingArea, allocation->width, allocation->height);
    //}

    LOG_DEBUG("DocumentView::OnSize() width:%d height:%d", width, height);
    if (m_pageWall == nullptr){
        m_pageWall = std::make_shared<PageWall>(width, height);
        //m_pageWall->Rebuild(m_document, m_rowPages);
        Rebuild();
    } else {
        m_pageWall->OnSize(width, height);
    }
    RedrawDocumentView();
}

void DocumentView::OnDraw(cairo_t *cr){
    GdkRGBA rgba;
    rgba.red = 0.25;
    rgba.green = 0.25;
    rgba.blue = 0.25;
    rgba.alpha = 0.6;
    gdk_cairo_set_source_rgba(cr, &rgba);
    cairo_paint(cr);

    //if (m_ofdRender != nullptr){
        //m_ofdRender->RenderBackgroundImage(m_document, m_pageIndex);
        //cairo_surface_t *backgroundSurface = m_ofdRender->GetCairoRender()->GetCairoSurface();
        //cairo_set_source_surface(cr, backgroundSurface, 0, 0);
        //cairo_paint(cr);
    //}

    if (m_pageWall != nullptr){
        m_pageWall->RenderWall(cr);
    }
}

void DocumentView::FirstPage(){
    if (m_document == nullptr) return;
    size_t total_pages = m_document->GetNumPages();
    m_pageIndex = 0;
    //m_ofdRender->SetOffsetX(0.0);
    //m_ofdRender->SetOffsetY(0.0);
    LOG_DEBUG("Page %d/%d", m_pageIndex + 1, (int)total_pages);

    if (m_pageWall != nullptr)
        m_pageWall->MoveFirstPage();
}

void DocumentView::LastPage(){
    if (m_document == nullptr) return;
    size_t total_pages = m_document->GetNumPages();
    m_pageIndex = total_pages - 1;
    //m_ofdRender->SetOffsetX(0.0);
    //m_ofdRender->SetOffsetY(0.0);
    LOG_DEBUG("Page %d/%d", m_pageIndex + 1, total_pages);

    if (m_pageWall != nullptr)
        m_pageWall->MoveLastPage();
}

void DocumentView::NextPage(){
    if (m_document == nullptr) return;
    size_t total_pages = m_document->GetNumPages();
    if (m_pageIndex < total_pages - 1){
        m_pageIndex++;
    } else {
        m_pageIndex = 0;
    }
    LOG_DEBUG("Page %d/%d", m_pageIndex + 1, total_pages);

    if (m_pageWall != nullptr)
        m_pageWall->MoveNextPage();
}

void DocumentView::PreviousPage(){
    if (m_document == nullptr) return;
    size_t total_pages = m_document->GetNumPages();
    if (m_pageIndex > 0){
        m_pageIndex--;
    } else {
        m_pageIndex = total_pages - 1;
    }
    //m_ofdRender->SetOffsetX(0.0);
    //m_ofdRender->SetOffsetY(0.0);
    LOG_DEBUG("Page %d/%d", m_pageIndex + 1, total_pages);

    if (m_pageWall != nullptr)
        m_pageWall->MovePreviousPage();
}

void DocumentView::ZoomIn(){
    if (m_pageWall != nullptr){
        m_pageWall->ZoomIn();
    }
}

void DocumentView::ZoomOut(){
    if (m_pageWall != nullptr){
        m_pageWall->ZoomOut();
    }
}

void DocumentView::ZoomFitBest(){
    if (m_pageWall != nullptr){
        m_pageWall->ZoomFitBest();
    }
}

void DocumentView::ZoomOriginal(){
    if (m_pageWall != nullptr){
        m_pageWall->ZoomOriginal();
    }
}

void DocumentView::MoveUp(){
    if (m_pageWall != nullptr){
        m_pageWall->MoveUp();
    }
}

void DocumentView::MoveDown(){
    if (m_pageWall != nullptr){
        m_pageWall->MoveDown();
    }
}

void DocumentView::MoveLeft(){
    if (m_pageWall != nullptr){
        m_pageWall->MoveLeft();
    }
}

void DocumentView::MoveRight(){
    if (m_pageWall != nullptr){
        m_pageWall->MoveRight();
    }
}

#define SCROLL_TIME_DELTA 400* 1000
//#define SCROLL_MOVE_ACCELERATION 1.2
//#define SCROLL_MAX_MOVE_ACCELERATION 10
#define SCROLL_MOVE_ACCELERATION 1.2
#define SCROLL_MAX_MOVE_ACCELERATION 10
#define SCROLL_ZOOM_ACCELERATION 1.2
#define SCROLL_MAX_ZOOM_ACCELERATION 8

double DocumentView::applyScrollAccelerate(int scrollAction, double acceleration, double maxAcceleration){
    if (m_scrollAction != scrollAction){
        m_scrollAction = scrollAction;
        m_lastScrollTime = 0;
        m_scrollAcceRate = 1.0;
        m_scrollCount = 0;
    }

    m_scrollCount++;
    double scrollAcceRate = m_scrollAcceRate;

    unsigned long long scrollTime = utils::GetTimeTick();
    if (m_lastScrollTime != 0){
        if (scrollTime - m_lastScrollTime < SCROLL_TIME_DELTA){
            if (m_scrollCount % 2 == 0){
                scrollAcceRate = m_scrollAcceRate * acceleration;
                if (scrollAcceRate >= maxAcceleration){
                    scrollAcceRate = maxAcceleration;
                }
                //LOG_NOTICE("scrollAcceRate:%.6f", scrollAcceRate);
            }
        } else {
            scrollAcceRate = 1.0;
            m_scrollCount = 0;
            //LOG_INFO("scrollAcceRate set to 1.0");
        }
    }
    m_lastScrollTime = scrollTime;

    return scrollAcceRate;
}

void DocumentView::ScrollUp(){
    if (m_pageWall == nullptr) return;
    m_scrollAcceRate = applyScrollAccelerate(0, SCROLL_MOVE_ACCELERATION, SCROLL_MAX_MOVE_ACCELERATION);
    m_pageWall->MoveUp(m_scrollAcceRate);
    RedrawDocumentView();
}

void DocumentView::ScrollDown(){
    if (m_pageWall == nullptr) return;
    m_scrollAcceRate = applyScrollAccelerate(1, SCROLL_MOVE_ACCELERATION, SCROLL_MAX_MOVE_ACCELERATION);
    m_pageWall->MoveDown(m_scrollAcceRate);
    RedrawDocumentView();
}

void DocumentView::ScrollLeft(){
    if (m_pageWall == nullptr) return;
    m_scrollAcceRate = applyScrollAccelerate(2, SCROLL_MOVE_ACCELERATION, SCROLL_MAX_MOVE_ACCELERATION);
    m_pageWall->MoveLeft(m_scrollAcceRate);
    RedrawDocumentView();
}

void DocumentView::ScrollRight(){
    if (m_pageWall == nullptr) return;
    m_scrollAcceRate = applyScrollAccelerate(3, SCROLL_MOVE_ACCELERATION, SCROLL_MAX_MOVE_ACCELERATION);
    m_pageWall->MoveRight(m_scrollAcceRate);
    RedrawDocumentView();
}

void DocumentView::ScrollIn(){
    if (m_pageWall == nullptr) return;
    m_scrollAcceRate = applyScrollAccelerate(4, SCROLL_ZOOM_ACCELERATION, SCROLL_MAX_ZOOM_ACCELERATION);
    m_pageWall->ZoomIn(m_scrollAcceRate);
    RedrawDocumentView();
}

void DocumentView::ScrollOut(){
    if (m_pageWall == nullptr) return;
    m_scrollAcceRate = applyScrollAccelerate(5, SCROLL_ZOOM_ACCELERATION, SCROLL_MAX_ZOOM_ACCELERATION);
    m_pageWall->ZoomOut(m_scrollAcceRate);
    RedrawDocumentView();
}

void DocumentView::OffsetPageWall(double offsetX, double offsetY){
    if (m_pageWall != nullptr){
        m_pageWall->OffsetViewArea(offsetX, offsetY);
    }
}

// 
void DocumentView::SelectText(double x0, double y0, double x1, double y1){
    if (m_pageWall != nullptr){

        double wallOffsetX = 0;
        double wallOffsetY = 0;
        double wallScaling = 1.0;
        std::tie(wallOffsetX, wallOffsetY, wallScaling) = m_pageWall->GetWallViewArea();
        double xx0 = utils::geometry::pixel_to_mm((x0 + wallOffsetX) / wallScaling, g_resolutionX);
        double yy0 = utils::geometry::pixel_to_mm((y0 + wallOffsetY) / wallScaling, g_resolutionX);
        double xx1 = utils::geometry::pixel_to_mm((x1 + wallOffsetX) / wallScaling, g_resolutionX);
        double yy1 = utils::geometry::pixel_to_mm((y1 + wallOffsetY) / wallScaling, g_resolutionX);

        int pageIndex = m_pageWall->GetPageIndexFromWallPoint(x0, y0);
        //LOG_DEBUG("SelectText() pageIndex=%d", pageIndex);
        ofd::DocumentPtr document = m_pageWall->GetDocument();
        ofd::PagePtr page = document->GetPage(pageIndex);
        ofd::text::TextPagePtr textPage = page->GetTextPage();
        for (size_t i = 0 ; i < textPage->GetTextLinesCount() ; i++){
            ofd::text::TextLinePtr textLine = textPage->GetTextLine(i);
            textLine->SelectText(xx0, yy0, xx1, yy1);
        }
    }
}
