#include <assert.h>
#include "utils/logger.h"
#include "ofd/reader/PackageView.h"

using namespace ofd;

PackageView::PackageView(){
}

PackageView::~PackageView(){
    m_documentViews.clear();
}

DocumentViewPtr PackageView::CreateNewDocumentView(){
    DocumentViewPtr documentView = std::make_shared<DocumentView>(GetSelf());
    m_documentViews.push_back(documentView);
    return documentView;
}

DocumentViewPtr PackageView::GetCurrentDocumentView() const{
    if (m_documentViews.size() > 0){
        if (m_documentViewIndex >= 0 && (size_t)m_documentViewIndex < m_documentViews.size()){
            return m_documentViews[m_documentViewIndex];
        }
    }
    return nullptr;
}

PackagePtr PackageView::OpenOFDFile(const std::string &filename){
    m_documentViews.clear();
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
        //documentView->m_drawingArea = m_drawingArea;

        bool bOpened = document->Open();
        if ( !bOpened ){
            LOG_ERROR("Open OFD Document failed. %s index:%d", filename.c_str(), i);
            return nullptr;
        }

        documentView->SetDocument(document);
        size_t numPages = document->GetPagesCount();
        LOG_DEBUG("Document %d Total pages:%d", i, numPages);

        if (m_viewingAreaWidth > 0 && m_viewingAreaHeight > 0){
            documentView->OnSize(m_viewingAreaWidth, m_viewingAreaHeight);
        }

        //documentView->Rebuild();
    }
    m_documentViewIndex = 0;

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

void PackageView::CmdFileOpen(){
    std::string fileName; // = choose_file(m_mainWindow);
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
    //assert(m_drawingArea != nullptr);
    //gdk_window_invalidate_rect(gtk_widget_get_window(m_drawingArea), nullptr, false);
}

void PackageView::OnSize(int width, int height){
    m_viewingAreaWidth = width;
    m_viewingAreaHeight = height;
    for (auto documentView : m_documentViews){
        documentView->OnSize(width, height);
    }
}

void PackageView::OnDraw(cairo_t *cr){
}
