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
    enum class WindowStatus{
        NORMAL = 0,
        SELECTANNOTATION,
        SELECTTEXT,
        SNAPSHOT,
        DRAWLINE,
        DRAWRECT,
        DRAWPOLYLINE
    };

    class Action{
    public:
        enum class Type{
            NORMAL = 0,
            SELECTANNOTATION,
            SELECTTEXT,
            SNAPSHOT,
            DRAWLINE,
            DRAWRECT,
            DRAWPOLYLINE
        }; // class Action::Type

        class KeyStatus{
        public:
            bool m_ctrlPressing = false;
            bool m_shiftPressing = false;
            bool m_altPressing = false;
            bool m_metaPressing = false;
            int m_keyCode = 0;

            KeyStatus(bool ctrlPressing = false, bool shiftPressing = false, bool altPressing = false, bool metaPressing = false, int keyCode = 0) :
                m_ctrlPressing(ctrlPressing), 
                m_shiftPressing(shiftPressing),
                m_altPressing(altPressing),
                m_metaPressing(metaPressing),
                m_keyCode(keyCode)
            {
            }
        }; // class KeyStatus;
    public:
        Action(Type actionType):m_type(actionType){
        }
        virtual ~Action(){};

        virtual bool OnLButtonPress(double x, double y, KeyStatus keyStatus, void *userData){
            m_x0 = x;
            m_y0 = y;
            m_bLButtonPressed = true;
            return false;
        }
        virtual bool OnLButtonRelease(double x, double y, KeyStatus keyStatus, void *userData){
            m_bLButtonPressed = false;
            return false;
        }
        virtual bool OnRButtonPress(double x, double y, KeyStatus keyStatus, void *userData){
            m_x0 = x;
            m_y0 = y;
            m_bRButtonPressed = true;
            return false;
        }
        virtual bool OnRButtonRelease(double x, double y, KeyStatus keyStatus, void *userData){
            m_bRButtonPressed = false;
            return false;
        }
        virtual bool OnMotionNotify(double x, double y, KeyStatus keyStatus, void *userData){return false;};

        Type GetType() const {return m_type;};

        Type m_type = Type::NORMAL;

        double m_x0 = 0;
        double m_y0 = 0;
        bool m_bLButtonPressed = false;
        bool m_bRButtonPressed = false;

    }; // class Action
    typedef std::shared_ptr<Action> ActionPtr;

    void ChangeAction(Action::Type actionType){m_action = ActionFactory::CreateAction(actionType);};

    class ActionFactory{
    public:
        static ActionPtr CreateAction(Action::Type actionType);
    }; // class ActionFactory

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
    void CmdGotoPage();
    void CmdZoomIn();
    void CmdZoomOut();
    void CmdZoomFitBest();
    void CmdZoomOriginal();
    void CmdMoveUp();
    void CmdMoveDown();
    void CmdMoveLeft();
    void CmdMoveRight();

    void CmdFileOpen();
    void CmdFileSave();
    void CmdFileSaveAs();

    void CmdToolsNormal();
    void CmdToolsSelectAnnotation();
    void CmdToolsSelectText();
    void CmdToolsSnapshot();
    void CmdToolsDrawLine();
    void CmdToolsDrawRect();
    void CmdToolsDrawPolyline();

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

    void OffsetPageWall(double offsetX, double offsetY);
    void SelectText(double x0, double y0, double x1, double y1);

    void ScrollUp();
    void ScrollDown();
    void ScrollLeft();
    void ScrollRight();
    void ScrollIn();
    void ScrollOut();
    int m_scrollAction = -1;
    unsigned long long m_lastScrollTime = 0;
    double m_scrollAcceRate = 1.0;
    uint32_t m_scrollCount = 0;
private:
    double applyScrollAccelerate(int scrollAction, double acceleration, double maxAcceleration);

public:
    ActionPtr GetAction() const {return m_action;};

private:
    ActionPtr m_action = ActionFactory::CreateAction(Action::Type::NORMAL);
    //ActionPtr m_action = ActionFactory::CreateAction(Action::Type::SELECTTEXT);

}; // class ReadWindow
typedef std::shared_ptr<ReadWindow> ReadWindowPtr;

#endif // __GTKOFD_READWINDOWN_H__
