#include <cctype>
#include <algorithm>
#include <iostream>
#include "ofd/OFDCmdLine.h"
#include "utils/logger.h"
#include "utils/utils.h"

using namespace ofd;

OFDCmdLine::OFDCmdLine(){
}

//#include <gflags/gflags.h>
//DEFINE_int32(v, 0, "Logger level.");
//DEFINE_string(owner_password, "", "The owner password of PDF file.");
//DEFINE_string(user_password, "", "The user password of PDF file.");

void usage(const std::string& appname){
    std::cout << "Usage: " << appname << " <filename> [cmd] [options]" << std::endl;
}

CmdParameters* OFDCmdLine::ParseCmdLine(int argc, char *argv[]){
    //gflags::SetVersionString("1.0.0");
    //gflags::SetUsageMessage("Usage: ofdviewer <pdffile>");
    //gflags::ParseCommandLineFlags(&argc, &argv, true);

    if (argc <= 1){
        // Open blank window.
        return new CmdOpenParameters();
        
        //LOG_WARN("File name must be given.");
        //usage(argv[0]);
        //return nullptr;
    }

    std::string filename = argv[1];
    if (!utils::FileExist(filename)){
        LOG_WARN("File %s is not exist.", filename.c_str());
        usage(argv[0]);
        return nullptr;
    }

    std::string cmd; 
    std::string options;
    if (argc >= 3) cmd = argv[2];
    if (argc >= 4) options = argv[3];
    //{
        //for ( int i = 3 ; i < argc ; i++ ){
            //options += argv[i];
            //if (i < argc - 1){
                //options += ";";
            //}
        //}
    //}

    CmdParameters* parameters = nullptr;
    if (cmd == "/a"){
        parameters = ParseCmdSaveAs(argv[0], options);
    } else if (cmd == "/e"){
        parameters = ParseCmdExport(argv[0], options);
    } else if (cmd == "/p"){
        parameters = ParseCmdPrint(argv[0], options);
    } else {
        options = cmd;
        parameters = ParseCmdOpen(argv[0], options);
    }
    if ( parameters != nullptr){
        parameters->m_filename = filename;
    }

    return parameters;
}

void usage_open(const std::string& appname){
    std::cout << "Usage: " << appname << " <filename> [options]" << std::endl;
}

std::pair<std::string, std::string> parse_option(const std::string &str){
    LOG_DEBUG("parse_option() %s", str.c_str());
    size_t pos = str.find('=');
    if (pos != std::string::npos){
        return std::make_pair(str.substr(0, pos), str.substr(pos+1, str.length()-pos));
    } else {
        return std::make_pair("", "");
    }
}

bool boolean_option(const std::string &_optValue){
    std::string optValue = _optValue;
    std::transform(optValue.begin(), optValue.end(), optValue.begin(), ::tolower);
    if (optValue == "true")
        return true;
    else
        return false;
}

CmdParameters* OFDCmdLine::ParseCmdOpen(const std::string &appname, const std::string &options){
    CmdOpenParameters *parameters = new CmdOpenParameters();

    size_t len = options.length();
    if (len < 2){
        return parameters;
    }
    if (options.substr(0, 1) != "[" || options.substr(len-1, 1) != "]"){
        usage_open(appname);
        return nullptr;
    }
    std::string cmd_options = options.substr(1,len-2);

    CT_VPreferences vps;
    std::vector<std::string> tokens = utils::SplitString(cmd_options, ";");
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
            vps.HideToolbar = boolean_option(optValue);
        } else if (optName == "hidemenubar"){
            vps.HideMenubar = boolean_option(optValue);
        } else if (optName == "hidewindowui"){
            vps.HideWindowUI = boolean_option(optValue);
        } else if (optName == "zoommode"){
            vps.ZoomMode = optValue;
        } else if (optName == "zoom"){
            vps.Zoom = atof(optValue.c_str());
        } else if (optName == "singlemode"){
            parameters->m_singleMode = boolean_option(optValue);
        } else if (optName == "safemode"){
            parameters->m_safeMode = boolean_option(optValue);
        }
    }

    parameters->m_appPreferences.FromCTVPreferences(vps);

    return parameters;
}

void usage_saveas(const std::string& appname){
    std::cout << "Usage: " << appname << " <filename> /e [file=xxx.txt]" << std::endl;
}
CmdParameters* OFDCmdLine::ParseCmdSaveAs(const std::string &appname, const std::string &options){
    CmdSaveAsParameters *parameters = new CmdSaveAsParameters();

    size_t len = options.length();
    if (len < 2) return parameters;
    if (options.substr(0, 1) != "[" || options.substr(len-1, 1) != "]"){
        usage_saveas(appname);
        return nullptr;
    }
    std::string cmd_options = options.substr(1,len-2);

    std::vector<std::string> tokens = utils::SplitString(cmd_options, ";");
    for (auto t : tokens){
        std::string optName;
        std::string optValue;
        std::tie(optName, optValue) = parse_option(t);
        std::transform(optName.begin(), optName.end(), optName.begin(), ::tolower);

        if (optName == "file"){
            parameters->m_destFilename = optValue;
        } else if (optName == "embed"){
            parameters->m_embed = boolean_option(optValue);
        } else if (optName == "linearize"){
            parameters->m_linearize = boolean_option(optValue);
        }
    }

    return parameters;
}

void usage_export(const std::string& appname){
    std::cout << "Usage: " << appname << " <filename> /e [file=xxx.txt|dir=xxx]" << std::endl;
}
CmdParameters* OFDCmdLine::ParseCmdExport(const std::string &appname, const std::string &options){
    CmdExportParameters *parameters = new CmdExportParameters();

    LOG_DEBUG("options=%s", options.c_str());
    size_t len = options.length();
    if (len < 2) return parameters;
    if (options.substr(0, 1) != "[" || options.substr(len-1, 1) != "]"){
        usage_export(appname);
        return nullptr;
    }
    std::string cmd_options = options.substr(1,len-2);
    LOG_DEBUG("cmd_options=%s", cmd_options.c_str());

    std::vector<std::string> tokens = utils::SplitString(cmd_options, ";");
    //LOG_DEBUG("Total tokens:%d", tokens.size());
    //for (auto t : tokens){
        //LOG_DEBUG("token:%s", t.c_str());
    //}

    for (auto t : tokens){
        std::string optName;
        std::string optValue;
        std::tie(optName, optValue) = parse_option(t);
        std::transform(optName.begin(), optName.end(), optName.begin(), ::tolower);

        if (optName == "dpi"){
            int dpi = atoi(optValue.c_str());
            if (dpi != 96 && dpi != 200 && dpi != 300){
                LOG_WARN("dpi must be 96/200/300, parameter = %d. Set to default value(96).", dpi);
                dpi = 96;
            }
            parameters->m_dpi = dpi;
        } else if (optName == "format") {
            std::transform(optValue.begin(), optValue.end(), optValue.begin(), ::tolower);
            std::string format = optValue;
            if (format != "bmp" && format != "jpg" && format != "png"){
                LOG_WARN("format must be bmp|jpg|png. parameter = %s", format.c_str());
                format = "bmp"; 
            }
            if (format == "bmp")
                parameters->m_format = ExportFormatType::BMP;
            else if (format == "jpg")
                parameters->m_format = ExportFormatType::JPG;
            else if (format == "png")
                parameters->m_format = ExportFormatType::PNG;
        } else if (optName == "dir") {
            parameters->m_dir = optValue;
        } else if (optName == "layer") {
            uint32_t outputLayers = 
                (uint32_t)OutputLayerType::TEMPLATES |
                (uint32_t)OutputLayerType::CONTENTS |
                (uint32_t)OutputLayerType::ANNOTS |
                (uint32_t)OutputLayerType::SEALS;
            parameters->m_outputLayers = outputLayers;
        } else if (optName == "file") {
            parameters->m_textFilename = optValue;
        }
    }

    return parameters;
}

void usage_print(const std::string& appname){
    std::cout << "Usage: " << appname << " <filename> /p [options]" << std::endl;
}
CmdParameters* OFDCmdLine::ParseCmdPrint(const std::string &appname, const std::string &options){
    CmdPrintParameters* parameters = new CmdPrintParameters();

    size_t len = options.length();
    if (len < 2) return parameters;
    if (options.substr(0, 1) != "[" || options.substr(len-1, 1) != "]"){
        usage_print(appname);
        return nullptr;
    }
    std::string cmd_options = options.substr(1,len-2);

    std::vector<std::string> tokens = utils::SplitString(cmd_options, ";");
    for (auto t : tokens){
        std::string optName;
        std::string optValue;
        std::tie(optName, optValue) = parse_option(t);
        std::transform(optName.begin(), optName.end(), optName.begin(), ::tolower);

        if (optName == "printer"){
            parameters->m_printer = optValue;
        } else if (optName == "copies") {
            parameters->m_copies = atoi(optValue.c_str());
        } else if (optName == "autorotate") {
            parameters->m_autoRotate = boolean_option(optValue);
        } else if (optName == "autozoom") {
            parameters->m_autoZoom = boolean_option(optValue);
        } else if (optName == "zoomratio") {
            parameters->m_zoomRatio = atof(optValue.c_str());
        } else if (optName == "layer") {
            uint32_t outputLayers = 
                (uint32_t)OutputLayerType::TEMPLATES |
                (uint32_t)OutputLayerType::CONTENTS |
                (uint32_t)OutputLayerType::ANNOTS |
                (uint32_t)OutputLayerType::SEALS;
            parameters->m_outputLayers = outputLayers;
        }
    }

    return parameters;
}
