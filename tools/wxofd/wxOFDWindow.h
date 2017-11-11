#ifndef __OFD_WXOFDWINDOW_H__
#define __OFD_WXOFDWINDOW_H__

#include <memory>
#include <wx/aui/aui.h>

class wxOFDWindow : public wxAuiNotebook{
public:
    wxOFDWindow();

    wxOFDWindow(wxWindow* parent,
                  wxWindowID id = wxID_ANY,
                  const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize,
                  long style = wxAUI_NB_DEFAULT_STYLE);

    virtual ~wxOFDWindow();

private:
    void AddFirstDocument();
    void OpenFirstOFDFile(); 


    class ImplCls;
    std::unique_ptr<ImplCls> m_impl;

}; // class wxOFDWindow

#endif // __OFD_WXOFDWINDOW_H__
