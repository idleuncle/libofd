#ifndef __GTKOFD_READWINDOWN_H__
#define __GTKOFD_READWINDOWN_H__

#include <string>
#include <gtk/gtk.h>
#include <cairo/cairo.h>
#include "ofd/Package.h"
#include "ofd/Document.h"
#include "ofd/Page.h"
using namespace ofd;
#include "OFDRender.h"
#include "PageWall.h"

class ReadWindow{
public:
    ReadWindow();
    virtual ~ReadWindow();

    DocumentPtr OpenOFDFile(const std::string &filename);
    void RedrawPage();

    void OnSize(int width, int height);
    void OnDraw(cairo_t *cr);

    void FirstPage();
    void NextPage();
    void PreviousPage();
    void LastPage();
    void ZoomIn();
    void ZoomOut();
    void ZoomFitBest();
    void ZoomOriginal();
    void MoveUp();
    void MoveDown();
    void MoveLeft();
    void MoveRight();

    void CmdFirstPage();
    void CmdNextPage();
    void CmdPreviousPage();
    void CmdLastPage();
    void CmdJumpPage();
    void CmdZoomIn();
    void CmdZoomOut();
    void CmdZoomFitBest();
    void CmdZoomOriginal();
    void CmdMoveUp();
    void CmdMoveDown();
    void CmdMoveLeft();
    void CmdMoveRight();

    void CmdFileOpen();

    GtkWindow *m_mainWindow = nullptr;
    GtkToolbar *m_mainToolbar = nullptr;
    GResource *resource = nullptr;
    GtkWidget *drawingArea = nullptr;
    GtkWidget *infobar = nullptr;
    GtkWidget *message = nullptr;
    GtkWindow *m_renderingWindow = nullptr;

    std::shared_ptr<ofd::OFDRender> m_ofdRender = nullptr;

    ofd::PackagePtr m_package = nullptr; 
    DocumentPtr m_document = nullptr;
    size_t m_pageIndex = 0;
    int m_rowPages = 1;


    PageWallPtr m_pageWall = nullptr;

    void ScrollUp();
    void ScrollDown();
    void ScrollLeft();
    void ScrollRight();
    void ScrollIn();
    void ScrollOut();
    int m_scrollAction = -1;
    unsigned long long m_lastScrollTime = 0;
    double m_scrollAcceRate = 1.0;
private:
    double applyScrollAccelerate(int scrollAction, double acceleration, double maxAcceleration);
}; // class ReadWindow
typedef std::shared_ptr<ReadWindow> ReadWindowPtr;

#endif // __GTKOFD_READWINDOWN_H__
