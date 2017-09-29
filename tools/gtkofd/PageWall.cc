#include <assert.h>
#include "PageWall.h"
#include "utils/logger.h"

using namespace ofd;

// defined in OFDRender.cc
extern double g_resolutionX; // = 240.0; // 120, 160, 240
extern double g_resolutionY; // = 240.0;

PageFrame::PageFrame(ofd::PagePtr page, const FrameRect &frameRect):
    m_page(page), m_frameRect(frameRect){
}
PageFrame::~PageFrame(){
}

void PageFrame::Rebuild(double wallScaling){
    Rect canvasRect = m_frameRect.GetCanvasRect();
    int canvasWidth = canvasRect.width * wallScaling;
    int canvasHeight = canvasRect.height * wallScaling;
    if (m_ofdRender != nullptr){
        m_ofdRender->RebuildBackgroundImage(canvasWidth, canvasHeight);
    } else {
        m_ofdRender = std::make_shared<ofd::OFDRender>(canvasWidth, canvasHeight);
    }
}

void PageFrame::drawBackgroundSample(cairo_t *backgroundContext, int canvasWidth, int canvasHeight){
    cairo_set_source_rgba(backgroundContext, 0, 0, 1.0, 1.0);
    cairo_move_to(backgroundContext, 0, 0);
    cairo_line_to(backgroundContext, canvasWidth, 0);
    cairo_line_to(backgroundContext, canvasWidth, canvasHeight);
    cairo_line_to(backgroundContext, 0, canvasHeight);
    cairo_line_to(backgroundContext, 0, 0);
    cairo_close_path(backgroundContext);
    cairo_stroke(backgroundContext);

    cairo_set_source_rgba(backgroundContext, 1.0, 0, 0, 1.0);
    cairo_new_path(backgroundContext);
    double rate = 0.2;
    cairo_move_to(backgroundContext, canvasWidth * rate, 0);
    cairo_line_to(backgroundContext, canvasWidth * rate, canvasHeight);
    cairo_move_to(backgroundContext, 0, canvasHeight * rate);
    cairo_line_to(backgroundContext, canvasWidth, canvasHeight * rate);
    cairo_stroke(backgroundContext);

    rate = 0.4;
    cairo_move_to(backgroundContext, canvasWidth * rate, 0);
    cairo_line_to(backgroundContext, canvasWidth * rate, canvasHeight);
    cairo_move_to(backgroundContext, 0, canvasHeight * rate);
    cairo_line_to(backgroundContext, canvasWidth, canvasHeight * rate);
    cairo_stroke(backgroundContext);

    cairo_set_source_rgba(backgroundContext, 0, 1.0, 0, 1.0);
    cairo_new_path(backgroundContext);
    rate = 0.5;
    cairo_move_to(backgroundContext, canvasWidth * rate, 0);
    cairo_line_to(backgroundContext, canvasWidth * rate, canvasHeight);
    cairo_move_to(backgroundContext, 0, canvasHeight * rate);
    cairo_line_to(backgroundContext, canvasWidth, canvasHeight * rate);
    cairo_stroke(backgroundContext);
}

void PageFrame::RenderFrame(cairo_t *cr, Point originPoint, double scaling){
    Rect canvasRect = m_frameRect.GetCanvasRect();
    RenderFrame(cr, originPoint, canvasRect, scaling);
}

void PageFrame::RenderFrame(cairo_t *cr, Point originPoint, Rect canvasRect, double scaling){
    if (m_ofdRender != nullptr){
        if (m_bRedraw){
            m_ofdRender->RenderBackgroundImage(m_page, 0, 0, scaling);
            LOG_DEBUG("%s", "RenderFrame() redraw.");
            m_bRedraw = false;
        }

        cairo_surface_t *backgroundSurface = m_ofdRender->GetCairoRender()->GetCairoSurface();
        //Rect canvasRect = m_frameRect.GetCanvasRect();
        //int canvasWidth = canvasRect.width;
        //int canvasHeight = canvasRect.height;
        //cairo_surface_t *backgroundSurface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, canvasWidth, canvasHeight);
        //cairo_t *backgroundContext = cairo_create(backgroundSurface);
        //drawBackgroundSample(backgroundContext, canvasWidth, canvasHeight);

        cairo_save(cr);

        //int x0 = canvasRect.x;
        //int y0 = canvasRect.y;
        //int x1 = x0 + canvasRect.width;
        //int y1 = y0 + canvasRect.height;
        //cairo_move_to(cr, x0, y0);
        //cairo_line_to(cr, x0, y1);
        //cairo_line_to(cr, x1, y1);
        //cairo_line_to(cr, x0, y1);
        //cairo_line_to(cr, x0, y0);
        //cairo_close_path(cr);
        //cairo_clip(cr);

        cairo_set_source_surface(cr, backgroundSurface, originPoint.x, originPoint.y);
        cairo_paint(cr);
        cairo_restore(cr);

        //cairo_destroy(backgroundContext);
        //cairo_surface_destroy(backgroundSurface);
    }
}


void PageFrame::ZoomIn(){
    if (m_ofdRender != nullptr){
        m_ofdRender->ZoomIn();
        m_bRedraw = true;
    }
}

void PageFrame::ZoomOut(){
    if (m_ofdRender != nullptr){
        m_ofdRender->ZoomOut();
        m_bRedraw = true;
    }
}

void PageFrame::ZoomFitBest(){
    if (m_ofdRender != nullptr){
        m_ofdRender->ZoomFitBest();
        m_bRedraw = true;
    }
}

void PageFrame::ZoomOriginal(){
}

// ===================== PageWall ====================

PageWall::PageWall(int screenWidth, int screenHeight, int screenBPP){
    Rebuild(screenWidth, screenHeight, screenBPP);
}

PageWall::~PageWall(){
}

void PageWall::Rebuild(int screenWidth, int screenHeight, int screenBPP){
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;
    m_screenBPP = screenBPP;
}

void PageWall::SetWallViewArea(int wallOffsetX, int wallOffsetY, double wallScaling){
    m_wallOffsetX = wallOffsetX;
    m_wallOffsetY = wallOffsetY;
    m_wallScaling = wallScaling;
}

//double ZOOM_BASE = exp(1.0);
//double ZOOM_STEP = 0.1;
//double X_STEP = 20;
//double Y_STEP = 20;

void PageWall::MoveUp(){
    m_wallOffsetY -= Y_STEP;
}

void PageWall::MoveDown(){
    m_wallOffsetY += Y_STEP;
}

void PageWall::MoveLeft(){
    m_wallOffsetX -= X_STEP;
}

void PageWall::MoveRight(){
    m_wallOffsetX += X_STEP;
}

void PageWall::ZoomIn(){
    m_zoomFactor += ZOOM_STEP;
    InitZoomScaling();
    for (auto pageFrame : m_pageFrames){
        pageFrame->ZoomIn();
    }
}

void PageWall::ZoomOut(){
    m_zoomFactor -= ZOOM_STEP;
    InitZoomScaling();
    for (auto pageFrame : m_pageFrames){
        pageFrame->ZoomOut();
    }
}

void PageWall::ZoomFitBest(){
    m_zoomFactor = ZOOM_BASE;
    m_wallOffsetX = 0;
    m_wallOffsetY = 0;
    InitZoomScaling();
    for (auto pageFrame : m_pageFrames){
        pageFrame->ZoomFitBest();
    }
}

void PageWall::ZoomOriginal(){
    for (auto pageFrame : m_pageFrames){
        pageFrame->ZoomOriginal();
    }
}

void PageWall::RebuildWall(ofd::DocumentPtr document, int rowPages){

    m_document = document;
    m_rowPages = rowPages;
    m_pageFrames.clear();

    size_t totalPages = m_document->GetNumPages();
    if ( totalPages == 0 ){
        return;
    }

    // from document.
    // pixel unit in current scaling.
    int pageMaxWidth = 0;
    int pageMaxHeight = 0;
    std::tie(pageMaxWidth, pageMaxHeight) = document->GetPageMaxPxielSize(g_resolutionX, g_resolutionY);
    //int pageWidth = 0;
    //int pageHeight = 0;

    // -------- PageWall rows and cols --------
    m_wallCols = m_rowPages;
    m_wallRows = (int)(totalPages / m_wallCols);
    if (totalPages - m_rowPages * m_wallRows > 0){
        m_wallRows++;
    }

    // -------- PageFrame Margins --------
    m_frameLeftMargin = 5;
    m_frameTopMargin = 5;
    m_frameRightMargin = 0;
    m_frameBottomMargin = 0;

    m_frameRect = FrameRect(0, 0, pageMaxWidth, pageMaxHeight);
    m_frameRect.margins.left = 5;
    m_frameRect.margins.top = 5;
    m_frameRect.margins.right = 0;
    m_frameRect.margins.bottom = 0;

    // -------- PageWall Margins --------
    m_wallLeftMargin = 10;
    m_wallRightMargin = 10;
    m_wallTopMargin = 10;
    m_wallBottomMargin = 10;

    int pageFrameWidth = pageMaxWidth + m_frameRect.margins.left + m_frameRect.margins.right;
    int pageFrameHeight = pageMaxHeight + m_frameRect.margins.top + m_frameRect.margins.bottom;
    m_wallRect = FrameRect(0, 0, pageFrameWidth * m_wallCols, pageFrameHeight * m_wallRows);
    m_wallRect.margins.left = m_wallLeftMargin;
    m_wallRect.margins.top = m_wallTopMargin;
    m_wallRect.margins.right = m_wallRightMargin;
    m_wallRect.margins.bottom = m_wallBottomMargin;

    InitZoomScaling();

    // -------- Page frames in document ----------
    for (size_t i = 0 ; i < totalPages ; i++){
        PagePtr page = m_document->GetPage(i);
        assert(page != nullptr);

        PageFramePtr pageFrame = std::make_shared<PageFrame>(page, m_frameRect);
        pageFrame->Rebuild(m_wallScaling);
        m_pageFrames.push_back(pageFrame);
    }

}

void PageWall::InitZoomScaling(){

    if (m_pageFrames.size() == 0) return;

    PagePtr page = m_pageFrames[0]->GetPage();
    double fitScaling = page->GetFitScaling(m_screenWidth, m_screenHeight, g_resolutionX, g_resolutionY);

    double factor = 1.0;
    double delta = m_zoomFactor - ZOOM_BASE;
    if ( delta >= 0.0 ){
        factor = log(m_zoomFactor);
    } else {
        factor = 1.0 - (1.0 / ( 1.0 + exp(delta)) - 0.5) * 2;
    }
    double scaling = fitScaling * factor;
    //scaling = fitScaling;
    m_wallScaling = scaling;
}

void PageWall::RenderWall(cairo_t *cr){
    if (m_document == nullptr){
        LOG_WARN("%s", "m_document == nullptr in RenderWall()");
        return;
    }
    // 判断当前屏幕上需要绘制哪些页面
    int frameHeight = m_frameRect.GetFrameHeight();
    int startRow = (m_wallOffsetY - m_wallTopMargin) / frameHeight;
    int endRow = (m_wallOffsetY - m_wallTopMargin + m_screenHeight) / frameHeight;
    int maxRows = (GetTotalPages() / m_rowPages) + 1;
    if (endRow >= maxRows) endRow = maxRows - 1;

    int frameWidth = m_frameRect.GetFrameWidth();
    int startCol = (m_wallOffsetX - m_wallLeftMargin) / frameWidth;
    int endCol = (m_wallOffsetX - m_wallLeftMargin + m_screenWidth) / frameWidth;
    if (endCol >= m_rowPages) endCol = m_rowPages - 1;

    LOG_DEBUG("RenderWall() rows(%d, %d) cols(%d, %d)", startRow, endRow, startCol, endCol);

    for ( auto i = startCol ; i <= endCol ; i++ ){
        for ( auto j = startRow ; j <= endRow ; j++ ){
            size_t pageIndex = j * m_rowPages + i;
            if (pageIndex >= GetTotalPages()) break;
            PageFramePtr pageFrame = m_pageFrames[pageIndex];
            Rect canvasRect = getPageCanvasRect(j, i);
            Point originPoint(canvasRect.x - m_wallOffsetX, canvasRect.y - m_wallOffsetY);

            cairo_save(cr);
            cairo_set_source_rgba(cr, 0.8, 0.8, 0.8, 0.8);
            double x0 = (double)originPoint.x - 0.5;
            double y0 = (double)originPoint.y - 0.5;
            double x1 = x0 + (double)canvasRect.width;
            double y1 = y0 + (double)canvasRect.height;
            cairo_move_to(cr, x0, y0);
            cairo_line_to(cr, x1, y0);
            cairo_line_to(cr, x1, y1);
            cairo_line_to(cr, x0, y1);
            cairo_line_to(cr, x0, y0);
            cairo_close_path(cr);
            cairo_stroke(cr);
            cairo_restore(cr);
            pageFrame->RenderFrame(cr, originPoint, canvasRect, m_wallScaling);
        }
    }
    // TODO
    //PageFramePtr pageFrame = m_pageFrames[0];
    //Rect canvasRect = getPageCanvasRect(0, 0);
    //Point originPoint(canvasRect.x - m_wallOffsetX, canvasRect.y - m_wallOffsetY);
    //pageFrame->RenderFrame(cr, originPoint, canvasRect, m_wallScaling);
}


int PageWall::getPageIndexFromWallPoint(int x, int y) const{
    assert(x>=0 && y >=0);

    int row = (y - m_wallRect.margins.top) / (m_frameRect.GetFrameHeight());
    int col = (x - m_wallRect.margins.left) / (m_frameRect.GetFrameWidth());

    int pageIndex = m_rowPages * row + col;

    return pageIndex;
}

// 对PageWall而言，PageFrame的FrameSize的像素尺寸
Rect PageWall::getPageFrameRect(int row, int col){
    int frameWidth = m_frameRect.GetFrameWidth() * m_wallScaling;
    int frameHeight = m_frameRect.GetFrameHeight() * m_wallScaling;
    int x = m_wallLeftMargin + frameWidth * col;
    int y = m_wallTopMargin + frameHeight * row;
    return Rect(x, y, frameWidth, frameHeight);
}

// 对PageWall而言，PageFrame的CanvasSize的像素尺寸
Rect PageWall::getPageCanvasRect(int row, int col){
    Rect frameRect = getPageFrameRect(row, col);
    int x = frameRect.x + m_frameRect.margins.left;
    int y = frameRect.y + m_frameRect.margins.top;
    int canvasWidth = m_frameRect.GetCanvasWidth() * m_wallScaling;
    int canvasHeight = m_frameRect.GetCanvasHeight() * m_wallScaling;
    return Rect(x, y, canvasWidth, canvasHeight);
}
