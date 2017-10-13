#ifndef __OFD_VPREFERENCES_H__
#define __OFD_VPREFERENCES_H__

#include <string>
#include "utils/xml.h"
using namespace utils;

namespace ofd{

    typedef struct CT_VPreferences{
        std::string PageMode = "None";
        std::string PageLayout = "OneColumn";
        std::string TabDisplay = "FileName";
        bool HideToolbar = false;
        bool HideMenubar = false;
        bool HideWindowUI = false;
        std::string ZoomMode = "Default";
        double Zoom = 0.0;
    } *CT_VPreferences_t;

    class VPreferences : public XMLable{
    public:
        VPreferences();

        CT_VPreferences ToCTVPreferences()const;
        bool FromCTVPreferences(CT_VPreferences vPreferences);

        virtual void GenerateXML(XMLWriter &writer) const override;
        virtual bool FromXML(XMLElementPtr objectElement) override;

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
        double Zoom = 0.0;

    }; // class VPreferences

}; // namespace ofd

#endif // __OFD_VPREFERENCES_H__
