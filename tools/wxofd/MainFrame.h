#ifndef __MAINFRAME_H__
#define __MAINFRAME_H__

#include <wx/frame.h>
#include <wx/aui/aui.h>

class MainFrame : public wxFrame {

    enum {
        ID_USER_BEGIN = wxID_HIGHEST+1,

        ID_CustomizeToolbar,
        ID_CreatePerspective,

        ID_SampleItem, // must be last

        ID_FirstPerspective = ID_CreatePerspective+1000
    };

public:
    MainFrame(wxWindow* parent,
            wxWindowID id,
            const wxString& title,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxDEFAULT_FRAME_STYLE | wxSUNKEN_BORDER);

    ~MainFrame();

    wxAuiDockArt* GetDockArt() {return m_mgr.GetArtProvider();}
    void DoUpdate() {return m_mgr.Update();}

private:
    void OnExit(wxCommandEvent& evt);
    void OnAbout(wxCommandEvent& evt);
    void OnEraseBackground(wxEraseEvent& evt);
    void OnSize(wxSizeEvent& evt);
    void OnUpdateUI(wxUpdateUIEvent& evt);
    void OnRestorePerspective(wxCommandEvent& evt); 

private:
    wxWindow* CreateOFDWindow();
    wxAuiToolBar *CreateMainToolbar();

private:
    wxAuiManager m_mgr;
    wxArrayString m_perspectives;
    long m_notebook_style;
    long m_notebook_theme;


    wxDECLARE_EVENT_TABLE();
};


#endif // __MAINFRAME_H__
