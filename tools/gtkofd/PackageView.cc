#include <assert.h>
#include "utils/logger.h"
#include "PackageView.h"

using namespace ofd;

PackageView::PackageView(){
}

PackageView::~PackageView(){
}

DocumentViewPtr PackageView::CreateNewDocumentView(){
    DocumentViewPtr documentView = std::make_shared<DocumentView>(GetSelf());
    m_documentViews.push_back(documentView);
    return documentView;
}

PackagePtr PackageView::OpenOFDFile(const std::string &filename){
    m_package = std::make_shared<ofd::Package>();
    if ( !m_package->Open(filename) ){
        LOG_ERROR("OFDPackage::Open() failed. filename:%s", filename.c_str());
        return nullptr;
    }

    for (size_t i = 0 ; i < m_package->GetDocumentsCount() ; i++){
        DocumentPtr document = m_package->GetDocument(i);
        assert(document != nullptr);
        LOG_DEBUG(document->to_string().c_str());

        DocumentViewPtr documentView = CreateNewDocumentView();
        assert(documentView != nullptr);

        bool bOpened = document->Open();
        if ( !bOpened ){
            LOG_ERROR("Open OFD Document failed. %s index:%d", filename.c_str(), i);
            return nullptr;
        }

        documentView->SetDocument(document);
    }

    return m_package;
}

bool PackageView::SaveOFDFile(const std::string &filename) const {
    if (m_package == nullptr){
        LOG_WARN("No OFD package opened.");
        return false;
    }

    return m_package->Save(filename);
}

void PackageView::CloseOFDFile(){
    m_documentViews.clear();
    m_package = nullptr;
}

static std::string choose_file(GtkWindow *parentWindow){
    std::string fileName;

    GtkWidget *dialog = gtk_file_chooser_dialog_new("Open File",
            parentWindow,
            GTK_FILE_CHOOSER_ACTION_OPEN,
            "_Cancel",
            GTK_RESPONSE_CANCEL,
            "_Open",
            GTK_RESPONSE_ACCEPT,
            nullptr);
    gint res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT){
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        char *filename = gtk_file_chooser_get_filename(chooser);
        fileName = filename;
        g_free(filename);
    }

    gtk_widget_destroy(dialog);

    return fileName;
}

void PackageView::CmdFileOpen(){
    std::string fileName = choose_file(m_mainWindow);
    if (!fileName.empty()){
        OpenOFDFile(fileName);
        //DocumentPtr document = OpenOFDFile(fileName);
        //if (document != nullptr){
            //m_document = document;
            //m_pageIndex = 0;
            //m_pageWall->Rebuild(document, 1);
            //m_pageWall->RebuildWall();

            //CmdZoomFitBest();
            for (auto documentView : m_documentViews){
                documentView->CmdZoomFitBest();
            }
        //}
    }
}

void PackageView::CmdFileClose(){
    CloseOFDFile();
}

void PackageView::RedrawPackageView(){
    gdk_window_invalidate_rect(gtk_widget_get_window(drawingArea), nullptr, false);
}

