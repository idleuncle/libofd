#ifndef __OFD_PACKAGEVIEW_H__
#define __OFD_PACKAGEVIEW_H__

#include <string>
#include <vector>
#include "ofd/Package.h"
#include "ofd/Document.h"
#include "DocumentView.h"

namespace ofd{

    class PackageView;
    typedef std::shared_ptr<PackageView> PackageViewPtr;
    class DocumentView;
    typedef std::shared_ptr<DocumentView> DocumentViewPtr;

    class PackageView : public std::enable_shared_from_this<PackageView>{
    public:
        PackageView();
        virtual ~PackageView();
        PackagePtr GetPackage() const {return m_package;};

        PackageViewPtr GetSelf(){return shared_from_this();};
        DocumentViewPtr CreateNewDocumentView();
        DocumentViewPtr GetCurrentDocumentView() const;
        int m_documentViewIndex = 0;

        PackagePtr OpenOFDFile(const std::string &filename);
        bool SaveOFDFile(const std::string &filename) const;
        void CloseOFDFile();

        //GtkWindow *m_mainWindow = nullptr;
        //GtkToolbar *m_mainToolbar = nullptr;
        //GResource *m_resource = nullptr;
        //GtkWidget *m_drawingArea = nullptr;
        //GtkWidget *m_infobar = nullptr;
        //GtkWidget *m_message = nullptr;
        //GtkWindow *m_renderingWindow = nullptr;

        void OnSize(int width, int height);
        void OnDraw(cairo_t *cr);

        void RedrawPackageView();
        void CmdFileOpen();
        void CmdFileClose();

    private:
        ofd::PackagePtr m_package = nullptr; 
        std::vector<DocumentViewPtr> m_documentViews;

        int m_viewingAreaWidth = 0;
        int m_viewingAreaHeight = 0;
    }; // class PackageView

}; // namespace ofd

#endif // __OFD_PACKAGEVIEW_H__
