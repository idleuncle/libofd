#ifndef __OFD_CMDLINE_H__
#define __OFD_CMDLINE_H__

#include <string>

namespace ofd{

    typedef struct CT_VPreferences{
        std::string PageMode = "None";
        std::string PageLayout = "OneColumn";
        std::string TabDisplay = "FileName";
        bool HideToolbar = false;
        bool HideMenubar = false;
        bool HideWindowUI = false;
        std::string ZoomMode = "Default";
        double Zoom;
    } *CT_VPreferences_t;

    class VPreferences{
    public:
        VPreferences();

        CT_VPreferences ToCTVPreferences()const;
        bool FromCTVPreferences(CT_VPreferences vPreferences);

        enum class PageModeType{
            NONE=0,
            FULLSCREEN,
            USEOUTLINES,
            USETHUMBS,
            USECUSTOMTAGS,
            USELAYERS,
            USEATTACHS,
            USEBOOKMARKS
        };

        enum class PageLayoutType{
            ONEPAGE=0,
            ONECOLUMN,
            TWOPAGEL,
            TWOCOLUMNL,
            TWOPAGER,
            TWOCOLUMNR
        };

        enum class TabDisplayType{
            FILENAME=0,
            DOCTITLE
        }; 

        enum class ZoomModeType{
            DEFAULT=0,
            FITHEIGHT,
            FITWIDTH,
            FITRECT
        };

    public:
        PageModeType PageMode = PageModeType::NONE;
        PageLayoutType PageLayout = PageLayoutType::ONECOLUMN;
        TabDisplayType TabDisplay = TabDisplayType::FILENAME;
        bool HideToolbar = false;
        bool HideMenubar = false;
        bool HideWindowUI = false;
        ZoomModeType ZoomMode = ZoomModeType::DEFAULT;
        double Zoom = 1.0;

    }; // class VPreferences

    class AppParameters{
    public:
        AppParameters();

    }; // AppParameters;

    class OFDCmdLine{
    public:
        OFDCmdLine();

        bool ParseCmdLine(int argc, char *argv[]);

        enum class CmdType{
            OPEN=0,
            SAVEAS,
            EXPORT,
            PRINT
        };

        CmdType m_cmd = CmdType::OPEN;
        std::string m_filename;
        // Cmd Open
        VPreferences m_appPreferences;
        bool m_singleMode = false;
        bool m_safeMode = false;

        // Cmd SaveAs
        std::string m_destFilename;
        bool m_embed = false;
        bool linearize = false;

        // Cmd Export Image
        enum class FormatType{
            BMP=0,
            JPG,
            PNG
        }; 
        enum class LayerType{
            TEMPLATES=0x01,
            CONTENTS=0x10,
            ANNOTS=0x100,
            SEALS=0x1000
        };

        std::string m_dir;
        int m_dpi = 96;
        FormatType m_format = FormatType::BMP;
        uint32_t m_layer = 
            (uint32_t)LayerType::TEMPLATES |
            (uint32_t)LayerType::CONTENTS |
            (uint32_t)LayerType::ANNOTS |
            (uint32_t)LayerType::SEALS;

        // Cmd Export Text
        std::string  m_textFilename;

        // Cmd Print
        std::string m_printName;
        size_t m_copies;
        bool m_autoRotate = true;
        bool autoZoom = true;
        double m_zoomRatio = 1.0;
        // LayerType m_layer

    private:
        bool ParseCmdOpen(const std::string &appname, const std::string &options);
        bool ParseCmdSaveAs(const std::string &appname, const std::string &options);
        bool ParseCmdExport(const std::string &appname, const std::string &options);
        bool ParseCmdPrint(const std::string &appname, const std::string &options);

    }; // class OFDCmdLine

}; // namespace ofd

#endif // __OFD_CMDLINE_H__

