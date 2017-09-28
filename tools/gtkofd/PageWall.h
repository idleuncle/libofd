#ifndef __PAGEWALL_H__
#define __PAGEWALL_H__
#include <cairo/cairo.h>
#include "ofd/Document.h"
#include "ofd/Page.h"
#include "OFDRender.h"

typedef struct Point{
    int x = 0;
    int y = 0;

    Point(){};
    Point(int _x, int _y):x(_x), y(_y){};
    void Offset(int cx, int cy){x+=cx; y+=cy;};

} *Point_t;

typedef struct Rect{
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;

    Rect(){};
    Rect(int _x, int _y, int _width, int _height):
        x(_x), y(_y), width(_width), height(_height){
    }

    Point GetLeftTop() const {return Point(x, y);};
    Point GetLeftBottom() const {return Point(x, y+height);};
    Point GetRightTop() const {return Point(x+width, y);};
    Point GetRightBottom() const {return Point(x+width, y+height);};
    Point GetCenter() const {return Point(x + width / 2, y + height / 2);};
    void Offset(int cx, int cy){x+=cx; y+=cy;};

    bool InRect(int _x, int _y) const{
        return _x >= x && _x < x + width &&
            _y >= y && _y < y + height;
    }

} *Rect_t;

// 带边沿的矩形
struct FrameRect : public Rect{
    FrameRect(){};
    FrameRect(int _x, int _y, int _width, int _height):
        Rect(_x, _y, _width, _height)
    {}

    struct Margins{
        int left = 0;
        int top = 0;
        int right = 0;
        int bottom = 0;

    }; // struct Margins
    Margins margins;

    //int getValidMargin(int margin, int size = -1, double rate = 0.0, int minMargin = -1, int maxMargin = -1)){
        //int validMargin = margin;
        //if ( margin < 0 ){
            //validMargin = (double)size * rate;
            //if (minMargin >= 0 && validMargin < minMargin){
                //validMargin = minMargin;
            //} 
            //if (maxMargin >= 0 && validMargin > maxMargin){
                //validMargin = maxMargin;
            //}
        //}
        //return validMargin;
    //}

    //void SetLeftMargin(int margin, double rate = 0.0, int minMargin = -1, int maxMargin = -1){
        //margins.left = getValidMargin(margin, width, rate, minMargin, maxMargin);
    //}
    //void SetTopMargin(int margin, double rate = 0.0, int minMargin = -1, int maxMargin = -1){
        //margins.top = getValidMargin(margin, height, rate, minMargin, maxMargin);
    //}
    //void SetRightMargin(int margin, double rate = 0.0, int minMargin = -1, int maxMargin = -1){
        //margins.right = getValidMargin(margin, width, rate, minMargin, maxMargin);
    //}
    //void SetBottomMargin(int margin, double rate = 0.0, int minMargin = -1, int maxMargin = -1){
        //margins.bottom = getValidMargin(margin, height, rate, minMargin, maxMargin);
    //}

    Rect GetCanvasRect() const{
        return Rect(x, y, width, height);
    }
    Rect GetFrameRect() const{
        return Rect(x-margins.left, y-margins.top, width + margins.left + margins.right, height + margins.top + margins.bottom);
    }
    int GetCanvasWidth() const{
        return width;
    }
    int GetCanvasHeight() const{
        return height;
    }
    int GetFrameWidth() const{
        return width + margins.left + margins.right;
    }
    int GetFrameHeight() const{
        return height + margins.top + margins.bottom;
    }
    std::tuple<int, int> GetCanvasOriginPoint() const{
        return std::make_tuple(x, y);
    }
    std::tuple<int, int> GetFrameOriginPoint() const{
        return std::make_tuple(x-margins.left, y-margins.top);
    }
    bool InCanvasRect(int _x, int _y) const{
        return GetCanvasRect().InRect(_x, _y);
    }
    bool InFrameRect(int _x, int _y) const{
        return GetFrameRect().InRect(_x, _y);
    }
    


}; // struct FrameRect

// 页面框
class PageFrame{
public:
    PageFrame(ofd::PagePtr page, int pageIndex, const FrameRect &frameRect);
    virtual ~PageFrame();

    void Rebuild(double wallScaling);
    void drawBackgroundSample(cairo_t *backgroundContext, int canvasWidth, int canvasHeight);

    void RenderFrame(cairo_t *cr, Point originPoint, double scaling);
    void RenderFrame(cairo_t *cr, Point originPoint, Rect canvasRect, double scaling);

    ofd::PagePtr GetPage() const {return m_page;};

    void ZoomIn();
    void ZoomOut();
    void ZoomFitBest();

    void SetRedraw(bool bRedraw){m_bRedraw = true;};
private:
    ofd::PagePtr m_page = nullptr;
    __attribute__((unused)) int m_pageIndex = 0;

    // width and height available only.
    FrameRect m_frameRect;

    std::shared_ptr<ofd::OFDRender> m_ofdRender = nullptr;
    bool m_bRedraw = true;

}; // class PageFrame
typedef std::shared_ptr<PageFrame> PageFramePtr;

// 页面墙
// 矩阵排列显示多个页面
// 空间无限扩展，显示窗口(screenWidth, screenHeight)。像素单位。
class PageWall{
public:
    PageWall(int screenWidth, int screenHeight, int screenBPP = 32);
    virtual ~PageWall();

    void Rebuild(int screenWidth, int screenHeight, int screenBPP);
    void SetWallViewArea(int wallOffsetX, int wallOffsetY, double wallScaling);

    std::tuple<int, int, double> GetWallViewArea() const{
        return std::make_tuple(m_wallOffsetX, m_wallOffsetY, m_wallScaling);
    }

    void RebuildWall(ofd::DocumentPtr document, int rowPages);

    void InitZoomScaling();

    void RenderWall(cairo_t *cr);

    int GetWallWidth() const {return m_wallRect.width;};
    int GetWallHeight() const {return m_wallRect.height;};
    size_t GetTotalPages() const{
        if (m_document != nullptr){
            return m_document->GetNumPages();
        } else {
            return 0;
        }
    }

    double ZOOM_BASE = exp(1.0);
    double ZOOM_STEP = 0.1;
    double X_STEP = 20;
    double Y_STEP = 20;

    void MoveUp();
    void MoveDown();
    void MoveLeft();
    void MoveRight();
    void ZoomIn();
    void ZoomOut();
    void ZoomFitBest();

private:
    int getPageIndexFromWallPoint(int x, int y) const;

    // 对PageWall而言，PageFrame的FrameSize的像素尺寸
    Rect getPageFrameRect(int row, int col);

    // 对PageWall而言，PageFrame的CanvasSize的像素尺寸
    Rect getPageCanvasRect(int row, int col);

private:
    // pixel unit.
    int m_screenWidth = 0;
    int m_screenHeight = 0;
    int m_screenBPP = 32;

    // Document
    ofd::DocumentPtr m_document = nullptr;
    //int m_totalPages = 0;
    int m_rowPages = 1;


    // PageWall
    int m_wallOffsetX = 0;
    int m_wallOffsetY = 0;
    double m_wallScaling = 1.0;

    double m_zoomFactor = exp(1.0);

    int m_wallRows = 0;
    int m_wallCols = 0;
    FrameRect m_wallRect;
    int m_wallLeftMargin = 10;
    int m_wallRightMargin = 10;
    int m_wallTopMargin = 10;
    int m_wallBottomMargin = 10;

    // PageFrame
    FrameRect m_frameRect;
    int m_frameLeftMargin = 5;
    int m_frameTopMargin = 5;
    int m_frameRightMargin = 0;
    int m_frameBottomMargin = 0;
    std::vector<PageFramePtr> m_pageFrames;

}; // class PageWall
typedef std::shared_ptr<PageWall> PageWallPtr;


#endif // __PAGEWALL_H__
