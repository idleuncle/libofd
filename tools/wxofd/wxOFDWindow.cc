#include "wxOFDWindow.h"
#include <wx/artprov.h>
#include <wx/wxhtml.h>

#include "Global.h"


class wxOFDWindow::ImplCls{
public:
    ImplCls(wxOFDWindow *parentWindow) : m_parentWindow(parentWindow){
    }

private:
    __attribute__((unused)) wxOFDWindow *m_parentWindow;

}; // class wxOFDWindow::ImplCls

wxOFDWindow::wxOFDWindow(){
    m_impl = std::unique_ptr<wxOFDWindow::ImplCls>(new wxOFDWindow::ImplCls(this));
}

wxOFDWindow::wxOFDWindow(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style) :
    wxAuiNotebook(parent, id, pos, size, style)
{

    m_impl = std::unique_ptr<wxOFDWindow::ImplCls>(new wxOFDWindow::ImplCls(this));

    AddDocument();
}

wxOFDWindow::~wxOFDWindow(){
}

wxString GetIntroText() {
    return wxString::FromAscii(sample_intro_text);
}

wxHtmlWindow* CreateHTMLCtrl(wxWindow* parent){
    assert(parent != nullptr);

    wxHtmlWindow* ctrl = new wxHtmlWindow(parent, wxID_ANY,
                                   wxDefaultPosition,
                                   wxSize(400,300));
    ctrl->SetPage(GetIntroText());
    return ctrl;
}

void wxOFDWindow::AddDocument(){
   this->Freeze();

   wxBitmap page_bmp = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16,16));

   this->AddPage(CreateHTMLCtrl(this), wxT("OFD Panel") , false, page_bmp);
   this->SetPageToolTip(0, "Welcome to wxAUI (this is a page tooltip)");

   this->Thaw();
}
