#include <assert.h>
#include "ofd/reader/DocumentView.h"

using namespace ofd;

class NormalAction : public DocumentView::Action {
public:
    NormalAction(): Action(DocumentView::Action::Type::NORMAL){};
    virtual ~NormalAction(){};
    
    virtual bool OnLButtonPress(double x, double y, KeyStatus keyStatus, void *userData) override {
        DocumentView::Action::OnLButtonPress(x, y, keyStatus, userData);
        return true;
    }
    virtual bool OnLButtonRelease(double x, double y, KeyStatus keyStatus, void *userData) override {
        DocumentView::Action::OnLButtonRelease(x, y, keyStatus, userData);
        return true;
    }
    virtual bool OnRButtonPress(double x, double y, KeyStatus keyStatus, void *userData) override {
        DocumentView::Action::OnRButtonPress(x, y, keyStatus, userData);
        return true;
    }
    virtual bool OnRButtonRelease(double x, double y, KeyStatus keyStatus, void *userData) override {
        DocumentView::Action::OnRButtonRelease(x, y, keyStatus, userData);
        return true;
    }
    virtual bool OnMotionNotify(double x, double y, KeyStatus keyStatus, void *userData) override {
        DocumentView *documentView = (DocumentView*)userData;
        if (m_bLButtonPressed){
            double offsetX = m_x0 - x;
            double offsetY = m_y0 - y;
            documentView->OffsetPageWall(offsetX, offsetY);
            //documentView->RedrawDocumentView();
            m_x0 = x;
            m_y0 = y;
            return true;
        }
        return false;
    }

}; // class NormalAction

class SelectAnnotationAction : public DocumentView::Action {
public:
    SelectAnnotationAction(): Action(DocumentView::Action::Type::SELECTANNOTATION){};
    virtual ~SelectAnnotationAction(){};
    
    virtual bool OnLButtonPress(double x, double y, KeyStatus keyStatus, void *userData){
        DocumentView::Action::OnLButtonPress(x, y, keyStatus, userData);
        return false;
    }
    virtual bool OnLButtonRelease(double x, double y, KeyStatus keyStatus, void *userData){return false;};
    virtual bool OnRButtonPress(double x, double y, KeyStatus keyStatus, void *userData){
        DocumentView::Action::OnRButtonPress(x, y, keyStatus, userData);
        return false;
    }
    virtual bool OnRButtonRelease(double x, double y, KeyStatus keyStatus, void *userData){return false;};
    virtual bool OnMotionNogify(double x, double y, KeyStatus keyStatus, void *userData){return false;};

}; // class SelectAnnotationAction

class SelectTextAction : public DocumentView::Action {
public:
    SelectTextAction(): Action(DocumentView::Action::Type::SELECTTEXT){};
    virtual ~SelectTextAction(){};
    
    virtual bool OnLButtonPress(double x, double y, KeyStatus keyStatus, void *userData) override {
        DocumentView::Action::OnLButtonPress(x, y, keyStatus, userData);
        return false;
    }
    virtual bool OnLButtonRelease(double x, double y, KeyStatus keyStatus, void *userData) override {
        return true;
    }
    virtual bool OnRButtonPress(double x, double y, KeyStatus keyStatus, void *userData) override {
        DocumentView::Action::OnRButtonPress(x, y, keyStatus, userData);
        return false;
    }
    virtual bool OnRButtonRelease(double x, double y, KeyStatus keyStatus, void *userData) override {
        return false;
    }
    virtual bool OnMotionNotify(double x, double y, KeyStatus keyStatus, void *userData) override {
        DocumentView *documentView = (DocumentView*)userData;
        if (m_bLButtonPressed){
            documentView->SelectText(m_x0, m_y0, x, y);
            documentView->RedrawDocumentView();
        }
        return true;
    }

}; // class SelectTextAction

class SnapshotAction : public DocumentView::Action {
public:
    SnapshotAction(): Action(DocumentView::Action::Type::SNAPSHOT){};
    virtual ~SnapshotAction(){};
    
    virtual bool OnLButtonPress(double x, double y, KeyStatus keyStatus, void *userData){
        DocumentView::Action::OnLButtonPress(x, y, keyStatus, userData);
        return false;
    }
    virtual bool OnLButtonRelease(double x, double y, KeyStatus keyStatus, void *userData){return false;};
    virtual bool OnRButtonPress(double x, double y, KeyStatus keyStatus, void *userData){
        DocumentView::Action::OnRButtonPress(x, y, keyStatus, userData);
        return false;
    }
    virtual bool OnRButtonRelease(double x, double y, KeyStatus keyStatus, void *userData){return false;};
    virtual bool OnMotionNogify(double x, double y, KeyStatus keyStatus, void *userData){return false;};

}; // class SnapshotAction

class DrawLineAction : public DocumentView::Action {
public:
    DrawLineAction(): Action(DocumentView::Action::Type::DRAWLINE){};
    virtual ~DrawLineAction(){};
    
    virtual bool OnLButtonPress(double x, double y, KeyStatus keyStatus, void *userData){
        DocumentView::Action::OnLButtonPress(x, y, keyStatus, userData);
        return false;
    }
    virtual bool OnLButtonRelease(double x, double y, KeyStatus keyStatus, void *userData){return false;};
    virtual bool OnRButtonPress(double x, double y, KeyStatus keyStatus, void *userData){
        DocumentView::Action::OnRButtonPress(x, y, keyStatus, userData);
        return false;
    }
    virtual bool OnRButtonRelease(double x, double y, KeyStatus keyStatus, void *userData){return false;};
    virtual bool OnMotionNogify(double x, double y, KeyStatus keyStatus, void *userData){return false;};

}; // class DrawLineAction

class DrawRectAction : public DocumentView::Action {
public:
    DrawRectAction(): Action(DocumentView::Action::Type::DRAWRECT){};
    virtual ~DrawRectAction(){};
    
    virtual bool OnLButtonPress(double x, double y, KeyStatus keyStatus, void *userData){
        DocumentView::Action::OnLButtonPress(x, y, keyStatus, userData);
        return false;
    }
    virtual bool OnLButtonRelease(double x, double y, KeyStatus keyStatus, void *userData){return false;};
    virtual bool OnRButtonPress(double x, double y, KeyStatus keyStatus, void *userData){
        DocumentView::Action::OnRButtonPress(x, y, keyStatus, userData);
        return false;
    }
    virtual bool OnRButtonRelease(double x, double y, KeyStatus keyStatus, void *userData){return false;};
    virtual bool OnMotionNogify(double x, double y, KeyStatus keyStatus, void *userData){return false;};

}; // class DrawRectAction

class DrawPolylineAction : public DocumentView::Action {
public:
    DrawPolylineAction(): Action(DocumentView::Action::Type::DRAWPOLYLINE){};
    virtual ~DrawPolylineAction(){};
    
    virtual bool OnLButtonPress(double x, double y, KeyStatus keyStatus, void *userData){
        DocumentView::Action::OnLButtonPress(x, y, keyStatus, userData);
        return false;
    }
    virtual bool OnLButtonRelease(double x, double y, KeyStatus keyStatus, void *userData){return false;};
    virtual bool OnRButtonPress(double x, double y, KeyStatus keyStatus, void *userData){
        DocumentView::Action::OnRButtonPress(x, y, keyStatus, userData);
        return false;
    }
    virtual bool OnRButtonRelease(double x, double y, KeyStatus keyStatus, void *userData){return false;};
    virtual bool OnMotionNogify(double x, double y, KeyStatus keyStatus, void *userData){return false;};

}; // class DrawPolylineAction

DocumentView::ActionPtr DocumentView::ActionFactory::CreateAction(Action::Type actionType){
    DocumentView::ActionPtr action = nullptr;

    if (actionType == DocumentView::Action::Type::NORMAL){
        action = std::shared_ptr<DocumentView::Action>(new NormalAction());
    }  else if (actionType == DocumentView::Action::Type::SELECTANNOTATION){
        action = std::shared_ptr<DocumentView::Action>(new SelectAnnotationAction());
    }  else if (actionType == DocumentView::Action::Type::SELECTTEXT){
        action = std::shared_ptr<DocumentView::Action>(new SelectTextAction());
    }  else if (actionType == DocumentView::Action::Type::SNAPSHOT){
        action = std::shared_ptr<DocumentView::Action>(new SnapshotAction());
    }  else if (actionType == DocumentView::Action::Type::DRAWLINE){
        action = std::shared_ptr<DocumentView::Action>(new DrawLineAction());
    }  else if (actionType == DocumentView::Action::Type::DRAWRECT){
        action = std::shared_ptr<DocumentView::Action>(new DrawRectAction());
    }  else if (actionType == DocumentView::Action::Type::DRAWPOLYLINE){
        action = std::shared_ptr<DocumentView::Action>(new DrawPolylineAction());
    }

    assert(action != nullptr);
    return action;
}
