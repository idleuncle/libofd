#include <assert.h>
#include "ReadWindow.h"
#include "utils/logger.h"
#include "utils/utils.h"

ReadWindow::ReadWindow(){
}

ReadWindow::~ReadWindow(){
}

DocumentPtr ReadWindow::OpenOFDFile(const std::string &filename){
    m_package = std::make_shared<ofd::Package>();
    if ( !m_package->Open(filename) ){
        LOG_ERROR("OFDPackage::Open() failed. filename:%s", filename.c_str());
        return nullptr;
    }
    DocumentPtr document = m_package->GetDefaultDocument(); 
    assert(document != nullptr);
    LOG_DEBUG(document->to_string().c_str());

    bool bOpened = document->Open();
    if ( !bOpened ){
        LOG_ERROR("Open OFD Document failed. filename:%s", filename.c_str());
        return nullptr;
    }

    m_document = document;

    if (m_pageWall != nullptr){
        m_pageWall->SetWallViewArea(0, 0, 0);
        m_pageWall->Rebuild(m_document, m_rowPages);
    }


    return document;
}

void ReadWindow::RedrawPage(){
    gdk_window_invalidate_rect(gtk_widget_get_window(drawingArea), nullptr, false);
}

void ReadWindow::OnSize(int width, int height){
    //if (m_ofdRender != nullptr){
        //m_ofdRender->RebuildBackgroundImage(allocation->width, allocation->height);
    //} else {
        //m_ofdRender = std::make_shared<ofd::OFDRender>(drawingArea, allocation->width, allocation->height);
    //}

    if (m_pageWall == nullptr){
        m_pageWall = std::make_shared<PageWall>(width, height);
        m_pageWall->Rebuild(m_document, m_rowPages);
    } else {
        m_pageWall->OnSize(width, height);
    }
}

void ReadWindow::OnDraw(cairo_t *cr){
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

void ReadWindow::FirstPage(){
    if (m_document == nullptr) return;
    size_t total_pages = m_document->GetNumPages();
    m_pageIndex = 0;
    //m_ofdRender->SetOffsetX(0.0);
    //m_ofdRender->SetOffsetY(0.0);
    LOG_DEBUG("Page %d/%d", m_pageIndex + 1, total_pages);

    m_pageWall->MoveFirstPage();
}

void ReadWindow::LastPage(){
    if (m_document == nullptr) return;
    size_t total_pages = m_document->GetNumPages();
    m_pageIndex = total_pages - 1;
    //m_ofdRender->SetOffsetX(0.0);
    //m_ofdRender->SetOffsetY(0.0);
    LOG_DEBUG("Page %d/%d", m_pageIndex + 1, total_pages);

    m_pageWall->MoveLastPage();
}

void ReadWindow::NextPage(){
    if (m_document == nullptr) return;
    size_t total_pages = m_document->GetNumPages();
    if (m_pageIndex < total_pages - 1){
        m_pageIndex++;
    } else {
        m_pageIndex = 0;
    }
    LOG_DEBUG("Page %d/%d", m_pageIndex + 1, total_pages);

    m_pageWall->MoveNextPage();
}

void ReadWindow::PreviousPage(){
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

    m_pageWall->MovePreviousPage();
}

void ReadWindow::ZoomIn(){
    if (m_pageWall != nullptr){
        m_pageWall->ZoomIn();
    }
}

void ReadWindow::ZoomOut(){
    if (m_pageWall != nullptr){
        m_pageWall->ZoomOut();
    }
}

void ReadWindow::ZoomFitBest(){
    if (m_pageWall != nullptr){
        m_pageWall->ZoomFitBest();
    }
}

void ReadWindow::ZoomOriginal(){
    if (m_pageWall != nullptr){
        m_pageWall->ZoomOriginal();
    }
}

void ReadWindow::MoveUp(){
    if (m_pageWall != nullptr){
        m_pageWall->MoveUp();
    }
}

void ReadWindow::MoveDown(){
    if (m_pageWall != nullptr){
        m_pageWall->MoveDown();
    }
}

void ReadWindow::MoveLeft(){
    if (m_pageWall != nullptr){
        m_pageWall->MoveLeft();
    }
}

void ReadWindow::MoveRight(){
    if (m_pageWall != nullptr){
        m_pageWall->MoveRight();
    }
}

#define SCROLL_TIME_DELTA 300* 1000
#define SCROLL_MOVE_ACCELERATION 1.2
#define SCROLL_MAX_MOVE_ACCELERATION 10
#define SCROLL_ZOOM_ACCELERATION 1.2
#define SCROLL_MAX_ZOOM_ACCELERATION 8

double ReadWindow::applyScrollAccelerate(int scrollAction, double acceleration, double maxAcceleration){
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
                LOG_NOTICE("scrollAcceRate:%.6f", scrollAcceRate);
            }
        } else {
            scrollAcceRate = 1.0;
            m_scrollCount = 0;
            LOG_INFO("scrollAcceRate set to 1.0");
        }
    }
    m_lastScrollTime = scrollTime;

    return scrollAcceRate;
}

void ReadWindow::ScrollUp(){
    m_scrollAcceRate = applyScrollAccelerate(0, SCROLL_MOVE_ACCELERATION, SCROLL_MAX_MOVE_ACCELERATION);
    m_pageWall->MoveUp(m_scrollAcceRate);
    RedrawPage();
}

void ReadWindow::ScrollDown(){
    m_scrollAcceRate = applyScrollAccelerate(1, SCROLL_MOVE_ACCELERATION, SCROLL_MAX_MOVE_ACCELERATION);
    m_pageWall->MoveDown(m_scrollAcceRate);
    RedrawPage();
}

void ReadWindow::ScrollLeft(){
    m_scrollAcceRate = applyScrollAccelerate(2, SCROLL_MOVE_ACCELERATION, SCROLL_MAX_MOVE_ACCELERATION);
    m_pageWall->MoveLeft(m_scrollAcceRate);
    RedrawPage();
}

void ReadWindow::ScrollRight(){
    m_scrollAcceRate = applyScrollAccelerate(3, SCROLL_MOVE_ACCELERATION, SCROLL_MAX_MOVE_ACCELERATION);
    m_pageWall->MoveRight(m_scrollAcceRate);
    RedrawPage();
}

void ReadWindow::ScrollIn(){
    m_scrollAcceRate = applyScrollAccelerate(4, SCROLL_ZOOM_ACCELERATION, SCROLL_MAX_ZOOM_ACCELERATION);
    m_pageWall->ZoomIn(m_scrollAcceRate);
    RedrawPage();
}

void ReadWindow::ScrollOut(){
    m_scrollAcceRate = applyScrollAccelerate(5, SCROLL_ZOOM_ACCELERATION, SCROLL_MAX_ZOOM_ACCELERATION);
    m_pageWall->ZoomOut(m_scrollAcceRate);
    RedrawPage();
}
