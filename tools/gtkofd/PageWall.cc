#include <iostream>
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

void PageFrame::RebuildFrame(double wallScaling){
    Rect canvasRect = m_frameRect.GetCanvasRect();
    double canvasWidth = canvasRect.width * wallScaling;
    double canvasHeight = canvasRect.height * wallScaling;
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
            //LOG_DEBUG("RenderFrame() redraw.");
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


void PageFrame::Zoom(double scaling){
    RebuildFrame(scaling);
    if (m_ofdRender != nullptr){
        m_ofdRender->SetScaling(scaling);
        m_bRedraw = true;
    }
}

// ===================== PageWall ====================

PageWall::PageWall(int screenWidth, int screenHeight, int screenBPP){
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;
    m_screenBPP = screenBPP;
}

PageWall::~PageWall(){
}

double get_fit_scaling(double pageWidth, double pageHeight, double screenWidth, double screenHeight){
    double scalingX = screenWidth / pageWidth;
    return scalingX;
    //double scalingY = screenHeight / pageHeight;
    //double scaling = scalingX >= scalingY ? scalingX : scalingY;
    //return scaling;
}

Size get_document_pixel_size(ofd::DocumentPtr document){
    if (document != nullptr){
        int pageMaxWidth = 0;
        int pageMaxHeight = 0;
        std::tie(pageMaxWidth, pageMaxHeight) = document->GetPageMaxPxielSize(g_resolutionX, g_resolutionY);
        return Size(pageMaxWidth, pageMaxHeight);

    } else {
        return Size();
    }
}


void PageWall::OnSize(int screenWidth, int screenHeight){
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;
    //m_bFitBest = true;

    double fitBestScaling = CalculateBestZoomScaling();
    m_zoomFactor *= m_fitBestScaling / fitBestScaling;
    m_fitBestScaling = fitBestScaling;
    m_wallScaling = GetZoomScaling();

    //rebuildWallRect();

    RebuildWall();
}

void PageWall::Rebuild(ofd::DocumentPtr document, int rowPages){
    if (document != nullptr){
        m_document = document;
    }
    if (rowPages > 0){
        m_rowPages = rowPages;
    }

    if (m_document != nullptr){
        m_documentPixelSize = get_document_pixel_size(m_document);
    } else {
        m_documentPixelSize = Size(0,0);
    }

    size_t totalPages = 0;
    if (m_document != nullptr){
        totalPages = m_document->GetNumPages();
    }

    // -------- PageFrame Margins --------
    m_frameRect = FrameRect(0, 0, m_documentPixelSize.width, m_documentPixelSize.height);
    m_frameRect.margins.left = m_frameLeftMargin;
    m_frameRect.margins.top = m_frameTopMargin;
    m_frameRect.margins.right = m_frameRightMargin;
    m_frameRect.margins.bottom = m_frameBottomMargin;

    // -------- PageWall rows and cols --------
    if (totalPages > 0){
        m_wallCols = m_rowPages;
        m_wallRows = (int)(totalPages / m_wallCols);
        if (totalPages - m_rowPages * m_wallRows > 0){
            m_wallRows++;
        }
    } else {
        m_wallRows = 0;
        m_wallCols = 0;
    }

    rebuildWallRect();
    RebuildWall();
}

Size PageWall::getPagePixelSize() const{
    double pagePixelWidth = m_frameLeftMargin + m_documentPixelSize.width + m_frameRightMargin;
    double pagePixelHeight = m_frameTopMargin + m_documentPixelSize.height + m_frameBottomMargin;

    return Size(pagePixelWidth, pagePixelHeight);
}

void PageWall::rebuildWallRect(){
    Size pagePixelSize = getPagePixelSize();
    double pagePixelWidth = pagePixelSize.width;
    double pagePixelHeight = pagePixelSize.height;

    m_wallRect = FrameRect(0, 0, pagePixelWidth * m_wallCols, pagePixelHeight * m_wallRows);
    m_wallRect.margins.left = m_wallLeftMargin;
    m_wallRect.margins.top = m_wallTopMargin;
    m_wallRect.margins.right = m_wallRightMargin;
    m_wallRect.margins.bottom = m_wallBottomMargin;
}


double PageWall::CalculateBestZoomScaling() const{
    if (m_document == nullptr){
        return 1.0;
    }

    Size pagePixelSize = getPagePixelSize();
    double pagePixelWidth = pagePixelSize.width;
    double pagePixelHeight = pagePixelSize.height;

    double fitScaling = get_fit_scaling(pagePixelWidth * m_rowPages, pagePixelHeight,
            m_screenWidth - m_wallRect.margins.left - m_wallRect.margins.right, 
            m_screenHeight - m_wallRect.margins.top - m_wallRect.margins.bottom);
    LOG_DEBUG("CalculateBestZoomScaling() pagePixelSize(%.2f, %.2f), screenSize(%.2f, %.2f), fitScaling:%.3f", pagePixelWidth * m_rowPages, pagePixelHeight, (double)m_screenWidth, (double)m_screenHeight, fitScaling); 

    return fitScaling;
}

double PageWall::GetZoomScaling(){
    double fitScaling;;
    //if (m_bFitBest){
        //fitScaling = CalculateBestZoomScaling();
        //m_bFitBest = false;
    //} else {
        fitScaling = m_fitBestScaling;
        //double factor = 1.0;
        //double delta = m_zoomFactor - ZOOM_BASE;
        //if ( delta >= 0.0 ){
            //factor = log(m_zoomFactor);
        //} else {
            //factor = 1.0 - (1.0 / ( 1.0 + exp(delta)) - 0.5) * 2;
        //}
        //double scaling = fitScaling * factor;
    //}
    double scaling = fitScaling * m_zoomFactor;

    return scaling;
}

void PageWall::SetWallViewArea(int wallOffsetX, int wallOffsetY, double wallScaling){
    if (wallOffsetX >=0)
        m_wallOffsetX = wallOffsetX;
    if (wallOffsetY >= 0)
        m_wallOffsetY = wallOffsetY;
    if (wallScaling > 0)
        m_wallScaling = wallScaling;
}

//double ZOOM_BASE = exp(1.0);
//double ZOOM_STEP = 0.1;
//double X_STEP = 20;
//double Y_STEP = 20;

void PageWall::MoveUp(double accelerateRate){
    double offset = Y_STEP * accelerateRate;
    m_wallOffsetY -= offset;
    if (m_wallOffsetY < 0) m_wallOffsetY = 0;
    ShowProperties();
}

void PageWall::MoveDown(double accelerateRate){
    double screenPixels = m_screenHeight / m_wallScaling;
    //if (m_wallRect.height - m_wallOffsetY <= screenPixels) return;

    double offset = Y_STEP * accelerateRate;
    //LOG_DEBUG("MoveDown() m_wallOffsetY:%.2f offset:%.2f", m_wallOffsetY, offset);
    //LOG_DEBUG("Y_STEP * accelerateRate:(%.2f * %.2f)", Y_STEP, accelerateRate);
    //LOG_DEBUG("Result m_wallOffsetY:%.2f", m_wallOffsetY + offset);
    m_wallOffsetY += offset;

    double wallFrameHeight = m_wallRect.GetFrameHeight();
    if (wallFrameHeight - screenPixels < m_wallOffsetY){ 
        if (wallFrameHeight - m_wallOffsetY < screenPixels){
            m_wallOffsetY = wallFrameHeight - screenPixels;
        }
    }
    ShowProperties();
}

void PageWall::MoveLeft(double accelerateRate){
    double offset = X_STEP * accelerateRate;
    LOG_DEBUG("MoveLeft() m_wallOffsetX:%.2f offset:%.2f (%.2f * %.2f) Result m_wallOffsetX:%.2f", m_wallOffsetX, offset, X_STEP, accelerateRate, m_wallOffsetX - offset);
    m_wallOffsetX -= offset;
    if (m_wallOffsetX < 0) m_wallOffsetX = 0;
    ShowProperties();
}

void PageWall::MoveRight(double accelerateRate){
    double screenPixels = m_screenWidth / m_wallScaling;
    //if (m_wallRect.width - m_wallOffsetX <= screenPixels) return;

    double offset = X_STEP * accelerateRate;
    //LOG_DEBUG("MoveRight() m_wallOffsetX:%.2f offset:%.2f", m_wallOffsetX, offset);
    //LOG_DEBUG("X_STEP * accelerateRate:(%.2f * %.2f)", X_STEP, accelerateRate);
    //LOG_DEBUG("Result m_wallOffsetX:%.2f", m_wallOffsetX + offset);
    m_wallOffsetX += offset;

    double wallFrameWidth = m_wallRect.GetFrameWidth();
    if (wallFrameWidth - screenPixels < m_wallOffsetX ){
        if (wallFrameWidth - m_wallOffsetX < screenPixels){
            m_wallOffsetX = wallFrameWidth - screenPixels;
        }
    }
    ShowProperties();
}

void PageWall::ZoomIn(double accelerateRate){
    m_zoomFactor += ZOOM_STEP * accelerateRate;
    m_wallScaling = GetZoomScaling();
    for (auto pageFrame : m_pageFrames){
        //pageFrame->ZoomIn();
        pageFrame->Zoom(m_wallScaling);
    }
}

void PageWall::ZoomOut(double accelerateRate){
    m_zoomFactor -= ZOOM_STEP * accelerateRate;
    m_wallScaling = GetZoomScaling();
    for (auto pageFrame : m_pageFrames){
        //pageFrame->ZoomOut();
        pageFrame->Zoom(m_wallScaling);
    }
}

void PageWall::ZoomFitBest(){
    //m_zoomFactor = ZOOM_BASE;
    m_zoomFactor = 1.0;
    m_wallOffsetX = 0;

    m_fitBestScaling= CalculateBestZoomScaling();
    m_wallScaling = GetZoomScaling();

    LOG_NOTICE("ZoomFitBest() m_frameRect(%.2f, %.2f, %.2f, %.2f) margins(%.2f, %.2f, %.2f, %.2f)", m_frameRect.x, m_frameRect.y, m_frameRect.width, m_frameRect.height, m_frameLeftMargin, m_frameTopMargin, m_frameRightMargin, m_frameBottomMargin);
    LOG_NOTICE("ZoomFitBest() m_wallRect:(%.2f, %.2f, %.2f, %.2f) m_wallScaling:%.3f", m_wallRect.x, m_wallRect.y, m_wallRect.width, m_wallRect.height, m_wallScaling);
    LOG_NOTICE("ZoomFitBest() m_screenWidth:%d m_screenHeight:%d)", m_screenWidth, m_screenHeight);
    for (auto pageFrame : m_pageFrames){
        //pageFrame->ZoomFitBest();
        pageFrame->Zoom(m_wallScaling);
    }

    ShowProperties();
}

void PageWall::ZoomOriginal(){
    m_zoomFactor = 1.0;
    m_wallScaling = 1.0;
    for (auto pageFrame : m_pageFrames){
        //pageFrame->ZoomOriginal();
        pageFrame->Zoom(m_wallScaling);
    }

    ShowProperties();
}

void PageWall::RebuildWall(){

    m_pageFrames.clear();
    //m_wallScaling = GetZoomScaling();

    if (m_document == nullptr) return;

    size_t totalPages = m_document->GetNumPages();
    if ( totalPages == 0 ){
        return;
    }

    // -------- Page frames in document ----------
    for (size_t i = 0 ; i < totalPages ; i++){
        PagePtr page = m_document->GetPage(i);
        assert(page != nullptr);

        PageFramePtr pageFrame = std::make_shared<PageFrame>(page, m_frameRect);
        pageFrame->RebuildFrame(m_wallScaling);
        m_pageFrames.push_back(pageFrame);
    }

}


void PageWall::RenderWall(cairo_t *cr){
    if (m_document == nullptr){
        LOG_WARN("m_document == nullptr in RenderWall()");
        return;
    }

    size_t totalPages = GetTotalPages();
    // 判断当前屏幕上需要绘制哪些页面
    int frameHeight = m_frameRect.GetFrameHeight() * m_wallScaling;
    int startRow = (m_wallOffsetY * m_wallScaling - m_wallRect.margins.top) / frameHeight;
    int endRow = (m_wallOffsetY * m_wallScaling - m_wallRect.margins.top + m_screenHeight) / frameHeight;
    int maxRows = (totalPages / m_rowPages);
    if (maxRows * m_rowPages - totalPages > 0){
        maxRows++;
    }
    if (endRow >= maxRows) endRow = maxRows - 1;

    int frameWidth = m_frameRect.GetFrameWidth() * m_wallScaling;
    int startCol = (m_wallOffsetX * m_wallScaling - m_wallRect.margins.left) / frameWidth;
    int endCol = (m_wallOffsetX * m_wallScaling- m_wallRect.margins.left + m_screenWidth) / frameWidth;
    if (endCol >= m_rowPages) endCol = m_rowPages - 1;

    LOG_DEBUG("RenderWall() rows(%d, %d) cols(%d, %d) origin:(%d, %d) scaling:%.3f", startRow, endRow, startCol, endCol, m_wallOffsetX, m_wallOffsetY, m_wallScaling);

    for ( auto j = startRow ; j <= endRow ; j++ ){
        for ( auto i = startCol ; i <= endCol ; i++ ){
            size_t pageIndex = j * m_rowPages + i;
            if (pageIndex >= totalPages) break;

            PageFramePtr pageFrame = m_pageFrames[pageIndex];
            Rect canvasRect = getPageCanvasRect(j, i);
            Point originPoint(canvasRect.x - m_wallOffsetX * m_wallScaling, canvasRect.y - m_wallOffsetY * m_wallScaling);

            cairo_save(cr);
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
            cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 1.0);
            cairo_fill(cr);
            cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 1.0);
            cairo_stroke(cr);
            cairo_restore(cr);

            pageFrame->RenderFrame(cr, originPoint, canvasRect, m_wallScaling);
        }
    }
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
    int x = m_wallRect.margins.left + frameWidth * col;
    int y = m_wallRect.margins.top + frameHeight * row;
    return Rect(x, y, frameWidth, frameHeight);
}

// 对PageWall而言，PageFrame的CanvasSize的像素尺寸
Rect PageWall::getPageCanvasRect(int row, int col){
    Rect frameRect = getPageFrameRect(row, col);
    int x = frameRect.x + m_frameRect.margins.left * m_wallScaling;
    int y = frameRect.y + m_frameRect.margins.top * m_wallScaling;
    int canvasWidth = m_frameRect.GetCanvasWidth() * m_wallScaling;
    int canvasHeight = m_frameRect.GetCanvasHeight() * m_wallScaling;
    return Rect(x, y, canvasWidth, canvasHeight);
}

void PageWall::ShowProperties() const{
    LOG_NOTICE("================ PageWall ================");
    std::cout << "\n";
    std::cout << "\tScreen Size:\t" << "(" << m_screenWidth << ", " << m_screenHeight << ")" << " " << "BPP: " << m_screenBPP << "\n";
    std::cout << "\tDocument:\t" << "Pixels Size: (" << m_documentPixelSize.width << ", " << m_documentPixelSize.height << ") " << " " << m_rowPages << " pages/row (" << m_wallRows << " rows " << m_wallCols << " cols)" << "\n";
    std::cout << "\tPages:\t" << "Total pages: " << GetTotalPages() << " Page frames size: " << m_pageFrames.size() << "\n";
    std::cout << "\tScaling:\t" << "FitBestScaling: " << m_fitBestScaling << " ZoomFactor: " << m_zoomFactor << "\n";
    std::cout << "\tWall Offset:\t" << "(" << m_wallOffsetX << ", " << m_wallOffsetY << ") " << " Wall Scaling: " << m_wallScaling << "\n"; 
    std::cout << "\tWall Rect:\t" << "(" << m_wallRect.x << ", " << m_wallRect.y << ") (" << m_wallRect.width << ", " << m_wallRect.height << ")\n";
    std::cout << "\tWall Margins:\t" << "(" << m_wallRect.margins.left << ", " << m_wallRect.margins.top << ", " << m_wallRect.margins.right << ", " << m_wallRect.margins.bottom << ")\n";
    std::cout << "\tFrame Rect:\t" << "(" << m_frameRect.x << ", " << m_frameRect.y << ") (" << m_frameRect.width << ", " << m_frameRect.height << ")\n";
    std::cout << "\tFrame Margins:\t" << "(" << m_frameRect.margins.left << ", " << m_frameRect.margins.top << ", " << m_frameRect.margins.right << ", " << m_frameRect.margins.bottom << ")\n";
    std::cout << "\n";
}
