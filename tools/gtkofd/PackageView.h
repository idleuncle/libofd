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

        PackageViewPtr GetSelf(){return shared_from_this();};
        DocumentViewPtr CreateNewDocumentView();

        PackagePtr OpenOFDFile(const std::string &filename);
        bool SaveOFDFile(const std::string &filename) const;
        void CloseOFDFile();

        GtkWindow *m_mainWindow = nullptr;
        GtkToolbar *m_mainToolbar = nullptr;
        GResource *resource = nullptr;
        GtkWidget *drawingArea = nullptr;
        GtkWidget *infobar = nullptr;
        GtkWidget *message = nullptr;
        GtkWindow *m_renderingWindow = nullptr;

        void RedrawPackageView();
        void CmdFileOpen();
        void CmdFileClose();

    private:
        ofd::PackagePtr m_package = nullptr; 
        std::vector<DocumentViewPtr> m_documentViews;

    }; // class PackageView

}; // namespace ofd

#endif // __OFD_PACKAGEVIEW_H__
