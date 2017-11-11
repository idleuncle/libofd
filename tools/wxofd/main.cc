#include <iostream>
#include <wx/wxprec.h>
#include <wx/app.h>
#include <wx/cmdline.h>
#include "ofd/OFDCmdLine.h"
#include "utils/logger.h"
#include "ofd/reader/PackageView.h"
#include "ofd/reader/DocumentView.h"

#include "MainFrame.h"

using namespace ofd;

CmdParameters *g_cmdParameters = nullptr;

bool cmd_open(const CmdOpenParameters &parameters){
//bool cmd_open(const CmdOpenParameters &parameters, int argc, char *argv[]){
    LOG_DEBUG("Do Open. file:%s", parameters.m_filename.c_str());
    //run_app(argc, argv);
    return true;
}

bool cmd_saveas(const CmdSaveAsParameters &parameters){
    PackageViewPtr packageView = std::make_shared<ofd::PackageView>();
    if (packageView == nullptr){
        LOG_WARN("packageView is nullptr.");
        return false;
    }

    std::string destFilename = parameters.m_destFilename;
    std::string srcFilename = parameters.m_filename;
    assert(!srcFilename.empty());
    if (!destFilename.empty()){
        LOG_DEBUG("Do SaveAS. file:%s", destFilename.c_str());
        if (packageView->OpenOFDFile(parameters.m_filename)){
            if (packageView->SaveOFDFile(destFilename)){
                LOG_NOTICE("Save OFD file %s success.", destFilename.c_str());
            } else {
                LOG_ERROR("Save OFD file %s failed.", destFilename.c_str());
            }
        } else {
            LOG_ERROR("Open OFD file %s failed.", srcFilename.c_str());
        }
    } else {
        LOG_WARN("Save as file name must be given. /a [file=xxx.ofd]");
    }

    return false;
}

bool cmd_export(const CmdExportParameters &parameters){
    PackageViewPtr packageView = std::make_shared<ofd::PackageView>();
    if (packageView == nullptr){
        LOG_WARN("packageView is nullptr.");
        return false;
    }

    std::string srcFilename = parameters.m_filename;
    assert(!srcFilename.empty());
    LOG_DEBUG("Do Export. src file:%s", srcFilename.c_str());
    if (packageView->OpenOFDFile(parameters.m_filename)){
        PackagePtr package = packageView->GetPackage();
        assert(package != nullptr);
        if (!parameters.m_textFilename.empty()){
            // Export text
            package->ExportText(parameters.m_textFilename);
        } else if (!parameters.m_dir.empty()){
            // Export image
            package->ExportImage(parameters.m_dir,
                    parameters.m_dpi,
                    parameters.m_format,
                    parameters.m_outputLayers);
        } else {
            LOG_WARN("file or dir parameter must be given. /e [file=xxx.txt|dir=xxx]");
        }
    } else {
        LOG_ERROR("Open OFD file %s failed.", srcFilename.c_str());
    }

    return false;
}

bool cmd_print(const CmdPrintParameters &parameters){

    PackageViewPtr packageView = std::make_shared<ofd::PackageView>();
    DocumentViewPtr documentView = packageView->GetCurrentDocumentView();
    if (documentView == nullptr){
        LOG_WARN("documentView == nullptr");
        return false;
    }

    LOG_DEBUG("Do print. printer:%s copies:%d", parameters.m_printer.c_str(), parameters.m_copies);
    LOG_DEBUG("autorotate:%d autozoom:%d zoomratio:%.3f layers:0x%x", 
            parameters.m_autoRotate, parameters.m_autoZoom,
            parameters.m_zoomRatio, parameters.m_outputLayers);

    documentView->DoPrint();

    return false;
}

int console_main(){

    bool bContinue = false;
    switch (g_cmdParameters->m_cmd){
        case CmdType::OPEN:
            bContinue = cmd_open(*((CmdOpenParameters*)g_cmdParameters));
            break;
        case CmdType::SAVEAS:
            bContinue = cmd_saveas(*((CmdSaveAsParameters*)g_cmdParameters));

            break;
        case CmdType::EXPORT:
            bContinue = cmd_export(*((CmdExportParameters*)g_cmdParameters));

            break;
        case CmdType::PRINT:
            bContinue = cmd_print(*((CmdPrintParameters*)g_cmdParameters));

            break;
    };

    return bContinue;
}

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
    utils::Logger::Initialize(1);

    std::cout << wxApp::argc << std::endl;
    for (auto i = 0 ; i < wxApp::argc ; i++){
        std::cout << wxApp::argv[i] << std::endl;
    }

    OFDCmdLine cmdline;
    g_cmdParameters = cmdline.ParseCmdLine(argc, argv);
    if (g_cmdParameters == nullptr){
        return false;
    }

    if (!console_main()){
        delete g_cmdParameters;
        return false;
    }

    //if ( !wxApp::OnInit() ){
        //delete g_cmdParameters;
        //return false;
    //}

    wxFrame* frame = new MainFrame(NULL,
                                 wxID_ANY,
                                 wxT("OFD阅读器"),
                                 wxDefaultPosition,
                                 wxSize(860, 1075));
    frame->Show();

    delete g_cmdParameters;

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
    for (unsigned int i = 0; i < parser.GetParamCount(); i++) {
        wxString arg = parser.GetParam(i); 
        std::cout << arg << std::endl; 
        args.Add(arg);
    }
 
    return true;
} 

