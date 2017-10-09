#include <cctype>
#include <algorithm>
#include <iostream>
#include "ofd/OFDCmdLine.h"
#include "utils/logger.h"
#include "utils/utils.h"

using namespace ofd;

VPreferences::VPreferences(){
}

CT_VPreferences VPreferences::ToCTVPreferences()const{
    CT_VPreferences vps;
    if (PageMode == PageModeType::NONE){
        vps.PageMode = "None";
    } else if (PageMode == PageModeType::FULLSCREEN){
        vps.PageMode = "FullScreen";
    } else if (PageMode == PageModeType::USEOUTLINES){
        vps.PageMode = "UseOutlines";
    } else if (PageMode == PageModeType::USETHUMBS){
        vps.PageMode = "UseThumbs";
    } else if (PageMode == PageModeType::USECUSTOMTAGS){
        vps.PageMode = "UseCustomTags";
    } else if (PageMode == PageModeType::USELAYERS){
        vps.PageMode = "UseLayers";
    } else if (PageMode == PageModeType::USEATTACHS){
        vps.PageMode = "UseAttachs";
    } else if (PageMode == PageModeType::USEBOOKMARKS){
        vps.PageMode = "UseBookmarks";
    } else {
        LOG_ERROR("Wrong PageModeType: %d", (int)PageMode);
    }

    if (PageLayout == PageLayoutType::ONEPAGE){
        vps.PageLayout = "OnePage";
    } else if (PageLayout == PageLayoutType::ONECOLUMN){
        vps.PageLayout = "OneColumn";
    } else if (PageLayout == PageLayoutType::TWOPAGEL){
        vps.PageLayout = "TwoPageL";
    } else if (PageLayout == PageLayoutType::TWOCOLUMNL){
        vps.PageLayout = "TwoColumnL";
    } else if (PageLayout == PageLayoutType::TWOPAGER){
        vps.PageLayout = "TwoPageR";
    } else if (PageLayout == PageLayoutType::TWOCOLUMNR){
        vps.PageLayout = "TwoColumnR";
    } else{
        LOG_ERROR("Wrong PageLayoutType: %d", (int)PageLayout);
    }

    if (TabDisplay == TabDisplayType::FILENAME){
        vps.TabDisplay = "FileName";
    } else if (TabDisplay == TabDisplayType::DOCTITLE){
        vps.TabDisplay = "DocTitle";
    } else {
        LOG_ERROR("Wrong TabDisplayType: %d", (int)TabDisplay);
    }

    vps.HideToolbar = HideToolbar;
    vps.HideMenubar = HideMenubar;
    vps.HideWindowUI = HideWindowUI;

    if (ZoomMode == ZoomModeType::DEFAULT){
        vps.ZoomMode = "Default";
    } else if (ZoomMode == ZoomModeType::FITHEIGHT){
        vps.ZoomMode = "FitHeight";
    } else if (ZoomMode == ZoomModeType::FITWIDTH){
        vps.ZoomMode = "FitWidth";
    } else if (ZoomMode == ZoomModeType::FITRECT){
        vps.ZoomMode = "FitRect";
    } else {
        LOG_ERROR("Wrong ZoomModeType: %d", (int)ZoomMode);
    }

    vps.Zoom = Zoom;

    return vps;
}

bool VPreferences::FromCTVPreferences(CT_VPreferences vps){
    bool bOK = true;

    if (vps.PageMode == "None"){
        PageMode = PageModeType::NONE;
    } else if (vps.PageMode == "FullScreen"){
        PageMode = PageModeType::FULLSCREEN;
    } else if (vps.PageMode == "UseOutlines"){
        PageMode = PageModeType::USEOUTLINES;
    } else if (vps.PageMode == "UseThumbs"){
        PageMode = PageModeType::USETHUMBS;
    } else if (vps.PageMode == "UseCustomTags"){
        PageMode = PageModeType::USECUSTOMTAGS;
    } else if (vps.PageMode == "UseLayers"){
        PageMode = PageModeType::USELAYERS;
    } else if (vps.PageMode == "UseAttachs"){
        PageMode = PageModeType::USEATTACHS;
    } else if (vps.PageMode == "UseBookmarks"){
        PageMode = PageModeType::USEBOOKMARKS;
    } else{
        LOG_ERROR("Wrong PageModeType: %s", vps.PageMode.c_str());
        bOK = false;
    }

    if (vps.PageLayout == "OnePage"){
        PageLayout = PageLayoutType::ONEPAGE;
    } else if (vps.PageLayout == "OneColumn"){
        PageLayout = PageLayoutType::ONECOLUMN;
    } else if (vps.PageLayout == "TwoPageL"){
        PageLayout = PageLayoutType::TWOPAGEL;
    } else if (vps.PageLayout == "TwoColumnL"){
        PageLayout = PageLayoutType::TWOCOLUMNL;
    } else if (vps.PageLayout == "TwoPageR"){
        PageLayout = PageLayoutType::TWOPAGER;
    } else if (vps.PageLayout == "TwoColumnR"){
        PageLayout = PageLayoutType::TWOCOLUMNR;
    } else {
        LOG_ERROR("Wrong PageLayoutType: %s", vps.PageLayout.c_str());
        bOK = false;
    }

    if (vps.TabDisplay == "FileName"){
        TabDisplay = TabDisplayType::FILENAME;
    } else if (vps.TabDisplay == "DocTitle"){
        TabDisplay = TabDisplayType::DOCTITLE;
    } else {
        LOG_ERROR("Wrong TabDisplayType: %s", vps.TabDisplay.c_str());
        bOK = false;
    }

    HideToolbar = vps.HideToolbar;
    HideMenubar = vps.HideMenubar;
    HideWindowUI = vps.HideWindowUI;

    if (vps.ZoomMode == "Default"){
        ZoomMode = ZoomModeType::DEFAULT;
    } else if (vps.ZoomMode == "FitHeight"){
        ZoomMode = ZoomModeType::FITHEIGHT;
    } else if (vps.ZoomMode == "FitWidth"){
        ZoomMode = ZoomModeType::FITWIDTH;
    } else if (vps.ZoomMode == "FitRect"){
        ZoomMode = ZoomModeType::FITRECT;
    } else {
        LOG_ERROR("Wrong ZoomModeType: %s", vps.ZoomMode.c_str());
        bOK = false;
    }

    Zoom = vps.Zoom;

    return bOK;
}

OFDCmdLine::OFDCmdLine(){
}

//#include <gflags/gflags.h>
//DEFINE_int32(v, 0, "Logger level.");
//DEFINE_string(owner_password, "", "The owner password of PDF file.");
//DEFINE_string(user_password, "", "The user password of PDF file.");

void usage(const std::string& appname){
    std::cout << "Usage: " << appname << " <filename> [cmd] [options]" << std::endl;
}

bool OFDCmdLine::ParseCmdLine(int argc, char *argv[]){
    //gflags::SetVersionString("1.0.0");
    //gflags::SetUsageMessage("Usage: ofdviewer <pdffile>");
    //gflags::ParseCommandLineFlags(&argc, &argv, true);

    if (argc <= 1){
        usage(argv[0]);
        return false;
    }

    m_filename = argv[1];
    if (!utils::FileExist(m_filename)){
        LOG_WARN("File name must be given.");
        usage(argv[0]);
        return false;
    }

    if (argc <= 2){
        usage(argv[0]);
        return false;
    }

    std::string cmd = argv[2];
    std::string options;
    if (argc >= 3){
        options = argv[3];
    }

    if (cmd == "/a"){
        return ParseCmdSaveAs(argv[0], options);
    } else if (cmd == "/e"){
        return ParseCmdExport(argv[0], options);
    } else if (cmd == "/p"){
        return ParseCmdPrint(argv[0], options);
    } else {
        options = cmd;
        return ParseCmdOpen(argv[0], options);
    }

    return true;
}

void usage_open(const std::string& appname){
    std::cout << "Usage: " << appname << " <filename> [options]" << std::endl;
}

std::pair<std::string, std::string> parse_option(const std::string &str){
    size_t pos = str.find('=');
    if (pos != std::string::npos){
        return std::make_pair(str.substr(0, pos), str.substr(pos+1, str.length()-pos-1));
    } else {
        return std::make_pair("", "");
    }
}

bool OFDCmdLine::ParseCmdOpen(const std::string &appname, const std::string &options){
    m_cmd = CmdType::OPEN;

    size_t len = options.length();
    if (len < 2 || options.substr(0, 1) != "[" || options.substr(len-1, 1) != "]"){
        usage_open(appname);
    }
    std::string cmd_options = options.substr(1,len-2);

    CT_VPreferences vps;
    std::vector<std::string> tokens = utils::SplitString(cmd_options);
    for (auto t : tokens){
        std::string optName;
        std::string optValue;
        std::tie(optName, optValue) = parse_option(t);
        std::transform(optName.begin(), optName.end(), optName.begin(), ::tolower);
        if (optName == "pagemode"){
            vps.PageMode = optValue;
        } else if (optName == "pagelayout"){
            vps.PageLayout = optValue;
        } else if (optName == "tabdisplay"){
            vps.TabDisplay = optValue;
        } else if (optName == "hidetoolbar"){
            std::transform(optValue.begin(), optValue.end(), optValue.begin(), ::tolower);
            if (optValue == "true")
                vps.HideToolbar = true;
            else
                vps.HideToolbar = false;
        } else if (optName == "hidemenubar"){
            std::transform(optValue.begin(), optValue.end(), optValue.begin(), ::tolower);
            if (optValue == "true")
                vps.HideMenubar = true;
            else
                vps.HideMenubar = false;
        } else if (optName == "hidewindowui"){
            std::transform(optValue.begin(), optValue.end(), optValue.begin(), ::tolower);
            if (optValue == "true")
                vps.HideWindowUI = true;
            else
                vps.HideWindowUI = false;
        } else if (optName == "zoommode"){
            vps.ZoomMode = optValue;
        } else if (optName == "zoom"){
            vps.Zoom = atof(optValue.c_str());
        } else if (optName == "singlemode"){
            std::transform(optValue.begin(), optValue.end(), optValue.begin(), ::tolower);
            if (optValue == "true")
                m_singleMode = true;
            else
                m_singleMode = false;
        } else if (optName == "safemode"){
            std::transform(optValue.begin(), optValue.end(), optValue.begin(), ::tolower);
            if (optValue == "true")
                m_safeMode = true;
            else
                m_safeMode = false;
        }
    }

    m_appPreferences.FromCTVPreferences(vps);

    return true;
}

void usage_saveas(const std::string& appname){
    std::cout << "Usage: " << appname << " <filename> /a [options]" << std::endl;
}
bool OFDCmdLine::ParseCmdSaveAs(const std::string &appname, const std::string &options){
    m_cmd = CmdType::SAVEAS;

    size_t len = options.length();
    if (len < 2 || options.substr(0, 1) != "[" || options.substr(len-1, 1) != "]"){
        usage_saveas(appname);
    }
    std::string cmd_options = options.substr(1,len-2);

    return true;
}

void usage_export(const std::string& appname){
    std::cout << "Usage: " << appname << " <filename> /e [options]" << std::endl;
}
bool OFDCmdLine::ParseCmdExport(const std::string &appname, const std::string &options){
    m_cmd = CmdType::EXPORT;

    size_t len = options.length();
    if (len < 2 || options.substr(0, 1) != "[" || options.substr(len-1, 1) != "]"){
        usage_export(appname);
    }
    std::string cmd_options = options.substr(1,len-2);

    return true;
}

void usage_print(const std::string& appname){
    std::cout << "Usage: " << appname << " <filename> /p [options]" << std::endl;
}
bool OFDCmdLine::ParseCmdPrint(const std::string &appname, const std::string &options){
    m_cmd = CmdType::PRINT;

    size_t len = options.length();
    if (len < 2 || options.substr(0, 1) != "[" || options.substr(len-1, 1) != "]"){
        usage_print(appname);
    }
    std::string cmd_options = options.substr(1,len-2);

    return true;
}
