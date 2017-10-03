#ifndef __PAGEWALL_H__
#define __PAGEWALL_H__
#include <cairo/cairo.h>
#include "ofd/Document.h"
#include "ofd/Page.h"
#include "OFDRender.h"

typedef struct Point{
    double x = 0;
    double y = 0;

    Point(){};
    Point(double _x, double _y):x(_x), y(_y){};
    void Offset(double cx, double cy){x+=cx; y+=cy;};

} *Point_t;

typedef struct Size{
    double width = 0;
    double height = 0;

    Size(){};
    Size(double _width, double _height):width(_width), height(_height){};

    void Zoom(double scaling){
        width = width * scaling;
        height = height * scaling;
    }
} *Size_t;

typedef struct Rect{
    double x = 0;
    double y = 0;
    double width = 0;
    double height = 0;

    Rect(){};
    Rect(double _x, double _y, double _width, double _height):
        x(_x), y(_y), width(_width), height(_height){
    }

    Point GetLeftTop() const {return Point(x, y);};
    Point GetLeftBottom() const {return Point(x, y+height);};
    Point GetRightTop() const {return Point(x+width, y);};
    Point GetRightBottom() const {return Point(x+width, y+height);};
    Point GetCenter() const {return Point(x + width / 2, y + height / 2);};
    void Offset(double cx, double cy){x+=cx; y+=cy;};

    bool InRect(double _x, double _y) const{
        return _x >= x && _x < x + width &&
            _y >= y && _y < y + height;
    }

} *Rect_t;

// 带边沿的矩形
struct FrameRect : public Rect{
    FrameRect(){};
    FrameRect(double _x, double _y, double _width, double _height):
        Rect(_x, _y, _width, _height)
    {}

    struct Margins{
        int left = 0;
        int top = 0;
        int right = 0;
        int bottom = 0;

    }; // struct Margins
    Margins margins;

    Rect GetCanvasRect() const{
        return Rect(x, y, width, height);
    }
    Rect GetFrameRect() const{
        return Rect(x-margins.left, y-margins.top, width + margins.left + margins.right, height + margins.top + margins.bottom);
    }
    double GetCanvasWidth() const{
        return width;
    }
    double GetCanvasHeight() const{
        return height;
    }
    double GetFrameWidth() const{
        return width + margins.left + margins.right;
    }
    double GetFrameHeight() const{
        return height + margins.top + margins.bottom;
    }
    std::tuple<double, double> GetCanvasOriginPoint() const{
        return std::make_tuple(x, y);
    }
    std::tuple<double, double> GetFrameOriginPoint() const{
        return std::make_tuple(x-margins.left, y-margins.top);
    }
    bool InCanvasRect(double _x, double _y) const{
        return GetCanvasRect().InRect(_x, _y);
    }
    bool InFrameRect(double _x, double _y) const{
        return GetFrameRect().InRect(_x, _y);
    }

}; // struct FrameRect

// 页面框
class PageFrame{
public:
    PageFrame(ofd::PagePtr page, const FrameRect &frameRect);
    virtual ~PageFrame();

    void RebuildFrame(double wallScaling);
    void drawBackgroundSample(cairo_t *backgroundContext, int canvasWidth, int canvasHeight);

    void RenderFrame(cairo_t *cr, Point originPoint, double scaling);
    void RenderFrame(cairo_t *cr, Point originPoint, Rect canvasRect, double scaling);

    ofd::PagePtr GetPage() const {return m_page;};

    void Zoom(double scaling);

    void SetRedraw(bool bRedraw){m_bRedraw = true;};
private:
    ofd::PagePtr m_page = nullptr;
    //__attribute__((unused)) int m_pageIndex = 0;

    // width and height available only.
    FrameRect m_frameRect;
    Size m_pageSize;


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

    void Rebuild(ofd::DocumentPtr document = nullptr, int rowPages = -1);
    double CalculateBestZoomScaling() const;
    double GetZoomScaling();

    void SetWallViewArea(int wallOffsetX, int wallOffsetY, double wallScaling);

    std::tuple<int, int, double> GetWallViewArea() const{
        return std::make_tuple(m_wallOffsetX, m_wallOffsetY, m_wallScaling);
    }

    void OnSize(int screenWidth, int screenHeight);
    void RebuildWall();
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
    double X_STEP = 24;
    double Y_STEP = 24;

    void MoveUp(double accelerateRate=1.0);
    void MoveDown(double accelerateRate=1.0);
    void MoveLeft(double accelerateRate=1.0);
    void MoveRight(double accelerateRate=1.0);
    void ZoomIn(double accelerateRate=1.0);
    void ZoomOut(double accelerateRate=1.0);
    void ZoomFitBest();
    void ZoomOriginal();

private:
    Size getPagePixelSize() const;
    int getPageIndexFromWallPoint(int x, int y) const;

    // 对PageWall而言，PageFrame的FrameSize的像素尺寸
    Rect getPageFrameRect(int row, int col);

    // 对PageWall而言，PageFrame的CanvasSize的像素尺寸
    Rect getPageCanvasRect(int row, int col);

    void rebuildWallRect();
private:
    // pixel unit.
    int m_screenWidth = 0;
    int m_screenHeight = 0;
    int m_screenBPP = 32;

    // Document
    ofd::DocumentPtr m_document = nullptr;
    //int m_totalPages = 0;
    int m_rowPages = 1;

    // changed by m_document and m_rowPages
    Size m_documentPixelSize;
    int m_wallRows = 0;
    int m_wallCols = 0;
    FrameRect m_wallRect;
    double m_fitBestScaling = 1.0;

    // PageWall
    int m_wallOffsetX = 0;
    int m_wallOffsetY = 0;
    double m_wallScaling = 1.0;

    double m_zoomFactor = exp(1.0);
    //bool m_bFitBest = true;

    const int m_wallLeftMargin = 8;
    const int m_wallRightMargin = 8;
    const int m_wallTopMargin = 0;
    const int m_wallBottomMargin = 12;

    double m_frameLeftMargin = 0;
    double m_frameRightMargin = 0;
    double m_frameTopMargin = 12;
    double m_frameBottomMargin = 0;

    // PageFrame
    FrameRect m_frameRect;
    std::vector<PageFramePtr> m_pageFrames;

    void ShowProperties() const;
}; // class PageWall
typedef std::shared_ptr<PageWall> PageWallPtr;


#endif // __PAGEWALL_H__
