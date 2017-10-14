#ifndef __OFD_CMDLINE_H__
#define __OFD_CMDLINE_H__

#include <string>
#include "ofd/VPreferences.h"
#include "ofd/Common.h"

namespace ofd{


    enum class CmdType{
        OPEN=0,
        SAVEAS,
        EXPORT,
        PRINT
    };

    struct CmdParameters{
        CmdParameters(CmdType cmdType):m_cmd(cmdType){
        }

        CmdType m_cmd = CmdType::OPEN;
        std::string m_filename;
    };

    struct CmdOpenParameters : public CmdParameters{
        CmdOpenParameters() : CmdParameters(CmdType::OPEN){
        }

        VPreferences m_appPreferences;
        bool m_singleMode = false;
        bool m_safeMode = false;
    };

    struct CmdSaveAsParameters : public CmdParameters{
        CmdSaveAsParameters() : CmdParameters(CmdType::SAVEAS){
        }
        std::string m_destFilename;
        bool m_embed = false;
        bool m_linearize = false;
    };

    struct CmdExportParameters : public CmdParameters{
        CmdExportParameters() : CmdParameters(CmdType::EXPORT){
        }
        // Cmd Export Image

        std::string m_dir;
        int m_dpi = 96;
        ExportFormatType m_format = ExportFormatType::BMP;
        uint32_t m_outputLayers = 
            (uint32_t)OutputLayerType::TEMPLATES |
            (uint32_t)OutputLayerType::CONTENTS |
            (uint32_t)OutputLayerType::ANNOTS |
            (uint32_t)OutputLayerType::SEALS;

        // Cmd Export Text
        std::string  m_textFilename;
    };

    struct CmdPrintParameters : public CmdParameters{
        CmdPrintParameters() : CmdParameters(CmdType::PRINT){
        }
        std::string m_printer;
        size_t m_copies = 1;
        bool m_autoRotate = true;
        bool m_autoZoom = true;
        double m_zoomRatio = 1.0;
        uint32_t m_outputLayers = 
            (uint32_t)OutputLayerType::TEMPLATES |
            (uint32_t)OutputLayerType::CONTENTS |
            (uint32_t)OutputLayerType::ANNOTS |
            (uint32_t)OutputLayerType::SEALS;
    };

    class OFDCmdLine{
    public:
        OFDCmdLine();

        CmdParameters* ParseCmdLine(int argc, char *argv[]);

    private:
        CmdParameters* ParseCmdOpen(const std::string &appname, const std::string &options);
        CmdParameters* ParseCmdSaveAs(const std::string &appname, const std::string &options);
        CmdParameters* ParseCmdExport(const std::string &appname, const std::string &options);
        CmdParameters* ParseCmdPrint(const std::string &appname, const std::string &options);

    }; // class OFDCmdLine

}; // namespace ofd

#endif // __OFD_CMDLINE_H__

