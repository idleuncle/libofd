#include <assert.h>
#include "wxOFDWindow.h"
#include <wx/artprov.h>
#include <wx/wxhtml.h>

#include "Global.h"


#include "ofd/reader/PackageView.h"
#include "ofd/reader/DocumentView.h"

#include "ofd/OFDCmdLine.h"
// defined in main.cc
extern CmdParameters *g_cmdParameters;

class wxOFDWindow::ImplCls{
public:
    ImplCls(wxOFDWindow *parentWindow) : m_parentWindow(parentWindow){
        m_packageView = std::make_shared<ofd::PackageView>();
    }

    ofd::PackageViewPtr getPackageView(){
        assert(m_packageView != nullptr);
        return m_packageView;
    }

    ofd::DocumentViewPtr getDocumentView(){
        assert(m_packageView != nullptr);
        return m_packageView->GetCurrentDocumentView();
    }

    void OpenFirstOFDFile(); 

private:
    __attribute__((unused)) wxOFDWindow *m_parentWindow;

    ofd::PackageViewPtr m_packageView = nullptr;

}; // class wxOFDWindow::ImplCls

wxOFDWindow::wxOFDWindow(){
    m_impl = std::unique_ptr<wxOFDWindow::ImplCls>(new wxOFDWindow::ImplCls(this));
}

wxOFDWindow::wxOFDWindow(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style) :
    wxAuiNotebook(parent, id, pos, size, style)
{

    m_impl = std::unique_ptr<wxOFDWindow::ImplCls>(new wxOFDWindow::ImplCls(this));

    AddFirstDocument();
}

wxOFDWindow::~wxOFDWindow(){
}

wxString GetIntroText() {
    return wxString::FromAscii(global_sample_intro_text);
}

wxHtmlWindow* CreateHTMLCtrl(wxWindow* parent){
    assert(parent != nullptr);

    wxHtmlWindow* ctrl = new wxHtmlWindow(parent, wxID_ANY,
                                   wxDefaultPosition,
                                   wxSize(400,300));
    ctrl->SetPage(GetIntroText());
    return ctrl;
}

void wxOFDWindow::OpenFirstOFDFile() {
    m_impl->OpenFirstOFDFile();
}
void wxOFDWindow::ImplCls::OpenFirstOFDFile() {
    std::string filename = g_cmdParameters->m_filename;
    if (!filename.empty()){
        getPackageView()->OpenOFDFile(filename);
    }
    PackageViewPtr packageView = getPackageView();
    assert(packageView != nullptr);
    DocumentViewPtr documentView = packageView->GetCurrentDocumentView();
    if (documentView != nullptr)
        documentView->CmdZoomFitBest();
}

void wxOFDWindow::AddFirstDocument(){
   this->Freeze();

   wxBitmap page_bmp = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16,16));

   this->AddPage(CreateHTMLCtrl(this), wxT("OFD Panel") , false, page_bmp);
   this->SetPageToolTip(0, "Welcome to wxAUI (this is a page tooltip)");

   OpenFirstOFDFile();

   this->Thaw();
}
