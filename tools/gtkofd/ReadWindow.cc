#include <assert.h>
#include "ReadWindow.h"
#include "utils/logger.h"

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
    m_pageWall = std::make_shared<PageWall>(width, height);

    if (m_document != nullptr){
        m_pageWall->RebuildWall(m_document, 1);
    }
}

void ReadWindow::OnDraw(cairo_t *cr){
    GdkRGBA rgba;
    rgba.red = 1.0;
    rgba.green = 1.0;
    rgba.blue = 1.0;
    rgba.alpha = 1.0;
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
}

void ReadWindow::LastPage(){
    if (m_document == nullptr) return;
    size_t total_pages = m_document->GetNumPages();
    m_pageIndex = total_pages - 1;
    //m_ofdRender->SetOffsetX(0.0);
    //m_ofdRender->SetOffsetY(0.0);
    LOG_DEBUG("Page %d/%d", m_pageIndex + 1, total_pages);
}

void ReadWindow::NextPage(){
    if (m_document == nullptr) return;
    size_t total_pages = m_document->GetNumPages();
    if (m_pageIndex < total_pages - 1){
        m_pageIndex++;
    } else {
        m_pageIndex = 0;
    }
    //m_ofdRender->SetOffsetX(0.0);
    //m_ofdRender->SetOffsetY(0.0);
    LOG_DEBUG("Page %d/%d", m_pageIndex + 1, total_pages);
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
