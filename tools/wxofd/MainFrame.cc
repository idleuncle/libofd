#include <wx/wxprec.h>
#include <wx/grid.h>
#include <wx/treectrl.h>
#include <wx/spinctrl.h>
#include <wx/artprov.h>
#include <wx/clipbrd.h>
#include <wx/image.h>
#include <wx/colordlg.h>
#include <wx/wxhtml.h>
#include <wx/imaglist.h>
#include <wx/dataobj.h>
#include <wx/dcclient.h>
#include <wx/bmpbuttn.h>
#include <wx/menu.h>
#include <wx/toolbar.h>
#include <wx/statusbr.h>
#include <wx/msgdlg.h>
#include <wx/textdlg.h>

#include "MainFrame.h"
#include "wxOFDWindow.h"
#include "Global.h"

// defined in menu.cc
wxMenuBar *CreateMainMenubar();

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)

    EVT_ERASE_BACKGROUND(MainFrame::OnEraseBackground)
    EVT_SIZE(MainFrame::OnSize)

wxEND_EVENT_TABLE()

#include "icons/document_open.xpm"
#include "icons/document_save.xpm"
#include "icons/document_properties.xpm"
#include "icons/document_print.xpm"
#include "icons/go_first.xpm"
#include "icons/go_previous.xpm"
#include "icons/go_next.xpm"
#include "icons/go_last.xpm"
#include "icons/go_jump.xpm"
#include "icons/zoom_in.xpm"
#include "icons/zoom_out.xpm"
#include "icons/zoom_fit.xpm"
#include "icons/zoom_original.xpm"
#include "icons/edit_copy.xpm"
#include "icons/edit_find.xpm"
#include "icons/application_exit.xpm"

__attribute__((unused)) void InitToolBar(wxToolBar* toolBar) {
    wxBitmap bitmaps[16];

    bitmaps[0] = wxBitmap(document_open_xpm);
    bitmaps[1] = wxBitmap(document_save_xpm);
    bitmaps[2] = wxBitmap(document_properties_xpm);
    bitmaps[3] = wxBitmap(document_print_xpm);

    bitmaps[4] = wxBitmap(go_first_xpm);
    bitmaps[5] = wxBitmap(go_previous_xpm);
    bitmaps[6] = wxBitmap(go_next_xpm);
    bitmaps[7] = wxBitmap(go_last_xpm);
    bitmaps[8] = wxBitmap(go_jump_xpm);

    bitmaps[9] = wxBitmap(zoom_in_xpm);
    bitmaps[10] = wxBitmap(zoom_out_xpm);
    bitmaps[11] = wxBitmap(zoom_fit_xpm);
    bitmaps[12] = wxBitmap(zoom_original_xpm);

    bitmaps[13] = wxBitmap(edit_copy_xpm);
    bitmaps[14] = wxBitmap(edit_find_xpm);

    bitmaps[15] = wxBitmap(application_exit_xpm);

    toolBar->AddTool(wxID_OPEN, "打开", bitmaps[0], "打开OFD文件");
    toolBar->AddTool(wxID_SAVE, "保存", bitmaps[1], "保存OFD文件");
    toolBar->AddTool(wxID_PROPERTIES, "属性", bitmaps[2], "OFD文件属性");
    toolBar->AddTool(wxID_PRINT, "打印", bitmaps[3], "打印OFD文件");

    toolBar->AddSeparator();

    toolBar->AddTool(wxID_FIRST, "首页", bitmaps[4], "首页");
    toolBar->AddTool(wxID_FIRST, "上页", bitmaps[5], "上一页");
    toolBar->AddTool(wxID_FIRST, "下页", bitmaps[6], "下一页");
    toolBar->AddTool(wxID_LAST, "末页", bitmaps[7], "末页");
    toolBar->AddTool(wxID_JUMP_TO, "跳至", bitmaps[8], "跳至");

    toolBar->AddSeparator();

    toolBar->AddTool(wxID_ZOOM_IN, "放大", bitmaps[9], "缩小");
    toolBar->AddTool(wxID_ZOOM_OUT, "缩小", bitmaps[10], "放大");
    toolBar->AddTool(wxID_ZOOM_FIT, "合适", bitmaps[11], "合适");
    toolBar->AddTool(wxID_ZOOM_100, "原始", bitmaps[12], "原始");

    toolBar->AddSeparator();

    toolBar->AddTool(wxID_COPY, "拷贝", bitmaps[13], "拷贝");
    toolBar->AddTool(wxID_FIND, "查找", bitmaps[14], "查找");

    toolBar->AddSeparator();

    toolBar->AddTool(wxID_EXIT, "退出", bitmaps[15], "退出");

    toolBar->Realize();
}

wxAuiToolBar *MainFrame::CreateMainToolbar(){
    // prepare a few custom overflow elements for the toolbars' overflow buttons
    wxAuiToolBarItemArray prepend_items;
    wxAuiToolBarItemArray append_items;
    wxAuiToolBarItem item;
    item.SetKind(wxITEM_SEPARATOR);
    append_items.Add(item);
    item.SetKind(wxITEM_NORMAL);
    item.SetId(ID_CustomizeToolbar);
    item.SetLabel(_("Customize..."));
    append_items.Add(item);

    wxAuiToolBar* tb1 = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                         wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_OVERFLOW);
    tb1->SetToolBitmapSize(wxSize(48,48));
    tb1->AddTool(ID_SampleItem+1, wxT("Test"), wxArtProvider::GetBitmap(wxART_ERROR));
    tb1->AddSeparator();
    tb1->AddTool(ID_SampleItem+2, wxT("Test"), wxArtProvider::GetBitmap(wxART_QUESTION));
    tb1->AddTool(ID_SampleItem+3, wxT("Test"), wxArtProvider::GetBitmap(wxART_INFORMATION));
    tb1->AddTool(ID_SampleItem+4, wxT("Test"), wxArtProvider::GetBitmap(wxART_WARNING));
    tb1->AddTool(ID_SampleItem+5, wxT("Test"), wxArtProvider::GetBitmap(wxART_MISSING_IMAGE));
    tb1->SetCustomOverflowItems(prepend_items, append_items);
    tb1->Realize();

    return tb1;
}

wxTreeCtrl* CreateTreeCtrl(wxWindow *parent) {
    wxTreeCtrl* tree = new wxTreeCtrl(parent, wxID_ANY,
                                      wxPoint(0,0), wxSize(160,250),
                                      wxTR_DEFAULT_STYLE | wxNO_BORDER);

    wxImageList* imglist = new wxImageList(16, 16, true, 2);
    imglist->Add(wxArtProvider::GetBitmap(wxART_FOLDER, wxART_OTHER, wxSize(16,16)));
    imglist->Add(wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16,16)));
    tree->AssignImageList(imglist);

    wxTreeItemId root = tree->AddRoot(wxT("wxAUI Project"), 0);
    wxArrayTreeItemIds items;

    items.Add(tree->AppendItem(root, wxT("Item 1"), 0));
    items.Add(tree->AppendItem(root, wxT("Item 2"), 0));
    items.Add(tree->AppendItem(root, wxT("Item 3"), 0));
    items.Add(tree->AppendItem(root, wxT("Item 4"), 0));
    items.Add(tree->AppendItem(root, wxT("Item 5"), 0));


    for (int i = 0, count = items.Count(); i < count; ++i){
        wxTreeItemId id = items.Item(i);
        tree->AppendItem(id, wxT("Subitem 1"), 1);
        tree->AppendItem(id, wxT("Subitem 2"), 1);
        tree->AppendItem(id, wxT("Subitem 3"), 1);
        tree->AppendItem(id, wxT("Subitem 4"), 1);
        tree->AppendItem(id, wxT("Subitem 5"), 1);
    }


    tree->Expand(root);

    return tree;
}

wxWindow* MainFrame::CreateOFDWindow(){
   wxSize client_size = GetClientSize();

   wxOFDWindow* ctrl = new wxOFDWindow(this, wxID_ANY,
                                    wxPoint(client_size.x, client_size.y),
                                    wxSize(430,200),
                                    m_notebook_style);
   return ctrl;
}

MainFrame::MainFrame(wxWindow* parent,
                 wxWindowID id,
                 const wxString& title,
                 const wxPoint& pos,
                 const wxSize& size,
                 long style)
        : wxFrame(parent, id, title, pos, size, style)
{

    m_mgr.SetManagedWindow(this);
    //SetIcon(wxIcon(sample_xpm));
    m_notebook_style = wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_TAB_EXTERNAL_MOVE | wxNO_BORDER;
    m_notebook_theme = 0;

    // -------------------- MenuBar --------------------   
    SetMenuBar(CreateMainMenubar());

    // -------------------- StatusBar --------------------   
    CreateStatusBar();
    GetStatusBar()->SetStatusText(_("Ready"));

    // -------------------- ToolBar --------------------   
    //CreateToolBar(wxNO_BORDER | wxTB_FLAT | wxTB_HORIZONTAL);
    //InitToolBar(GetToolBar());
    wxAuiToolBar* tb1 = this->CreateMainToolbar();
    m_mgr.AddPane(tb1, wxAuiPaneInfo().
                  Name(wxT("tb1")).Caption(wxT("Big Toolbar")).
                  ToolbarPane().Top());


    //// Give this pane an icon, too, just for testing.
    //int iconSize = m_mgr.GetArtProvider()->GetMetric(wxAUI_DOCKART_CAPTION_SIZE);

    //// Make it even to use 16 pixel icons with default 17 caption height.
    //iconSize &= ~1;

    // -------------------- Panels --------------------   
    m_mgr.AddPane(CreateTreeCtrl(this), wxAuiPaneInfo().
                  Name(wxT("test8")).Caption(wxT("Tree Pane")).
                  Left().Layer(1).Position(1).
                  CloseButton(true).MaximizeButton(true));
    m_mgr.AddPane(CreateOFDWindow(), wxAuiPaneInfo().Name(wxT("ofd_panel")).
                  CenterPane().PaneBorder(false));

    // -------------------- Perspectives --------------------   
    SetMinSize(wxSize(400, 600));
    
    wxString perspective_all = m_mgr.SavePerspective();
    wxAuiPaneInfoArray& all_panes = m_mgr.GetAllPanes();
    for (int i = 0, count = all_panes.GetCount(); i < count; ++i){
        if (!all_panes.Item(i).IsToolbar()) all_panes.Item(i).Hide();
    }
    //m_mgr.GetPane(wxT("tb1")).Hide();
    //m_mgr.GetPane(wxT("tb6")).Hide();
    m_mgr.GetPane(wxT("test8")).Show().Left().Layer(0).Row(0).Position(0);
    //m_mgr.GetPane(wxT("test10")).Show().Bottom().Layer(0).Row(0).Position(0);
    m_mgr.GetPane(wxT("ofd_panel")).Show();
    wxString perspective_default = m_mgr.SavePerspective();
    m_perspectives.Add(perspective_default);
    m_perspectives.Add(perspective_all);

    // "commit" all changes made to wxAuiManager
    m_mgr.Update();
}

MainFrame::~MainFrame(){
    m_mgr.UnInit();
}

void MainFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(_("wxAUI Demo\nAn advanced window management library for wxWidgets\n(c) Copyright 2005-2006, Kirix Corporation"), _("About wxAUI Demo"), wxOK, this);
}


void MainFrame::OnEraseBackground(wxEraseEvent& evt) {
    evt.Skip();
}

void MainFrame::OnSize(wxSizeEvent& evt) {
    evt.Skip();
}

void MainFrame::OnUpdateUI(wxUpdateUIEvent& evt) {
    //unsigned int flags = m_mgr.GetFlags();

    switch (evt.GetId()){
    default:
        break;
    };
}

void MainFrame::OnRestorePerspective(wxCommandEvent& evt) {
    m_mgr.LoadPerspective(m_perspectives.Item(evt.GetId() - ID_FirstPerspective));
}

