#include <iostream>
#include <wx/wxprec.h>
#include <wx/app.h>
#include <wx/cmdline.h>

#include "MainFrame.h"

// -- application --

class MyApp : public wxApp {
public:
    virtual bool OnInit() override;
    virtual int OnExit() override;
    virtual int OnRun() override;
    virtual void OnInitCmdLine(wxCmdLineParser& parser) override;
    virtual bool OnCmdLineParsed(wxCmdLineParser& parser) override;
};

DECLARE_APP(MyApp)
IMPLEMENT_APP(MyApp)

bool MyApp::OnInit() {
    std::cout << wxApp::argc << std::endl;
    for (auto i = 0 ; i < wxApp::argc ; i++){
        std::cout << wxApp::argv[i] << std::endl;
    }

    if ( !wxApp::OnInit() )
        return false;


    wxFrame* frame = new MainFrame(NULL,
                                 wxID_ANY,
                                 wxT("OFD阅读器"),
                                 wxDefaultPosition,
                                 wxSize(860, 1075));
    frame->Show();

    return true;
}

int MyApp::OnExit(){
    return 0;
}

int MyApp::OnRun(){
    int exitCode = wxApp::OnRun();
    return exitCode;
}

static const wxCmdLineEntryDesc g_cmdLineDesc [] = {
     //{ wxCMD_LINE_SWITCH, "h", "help", "displays help on the command line parameters",
          //wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
     //{ wxCMD_LINE_SWITCH, "t", "test", "test switch",
          //wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_MANDATORY  },
     //{ wxCMD_LINE_SWITCH, "s", "silent", "disables the GUI" },
 
     { wxCMD_LINE_NONE }
};

void MyApp::OnInitCmdLine(wxCmdLineParser& parser) {
    parser.SetDesc (g_cmdLineDesc);
    // must refuse '/' as parameter starter or cannot use "/path" style paths
    //parser.SetSwitchChars (wxT("-"));
}

bool MyApp::OnCmdLineParsed(wxCmdLineParser& parser) {
    //silent_mode = parser.Found(wxT("s"));
 
    // to get at your unnamed parameters use
    wxArrayString args;
    for (int i = 0; i < parser.GetParamCount(); i++) {
        wxString arg = parser.GetParam(i); 
        std::cout << arg << std::endl; 
        args.Add(arg);
    }
 
    return true;
} 

