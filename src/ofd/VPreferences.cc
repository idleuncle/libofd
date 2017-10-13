#include "ofd/VPreferences.h"
#include "utils/logger.h"

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

    //typedef struct CT_VPreferences{
        //std::string PageMode = "None";
        //std::string PageLayout = "OneColumn";
        //std::string TabDisplay = "FileName";
        //bool HideToolbar = false;
        //bool HideMenubar = false;
        //bool HideWindowUI = false;
        //std::string ZoomMode = "Default";
        //double Zoom;
    //} *CT_VPreferences_t;
void VPreferences::GenerateXML(utils::XMLWriter &writer) const{
    CT_VPreferences vps = ToCTVPreferences();

    writer.StartElement("VPreferences");{
        if (PageMode != PageModeType::NONE)
            writer.WriteElement("PageMode", vps.PageMode);
        if (PageLayout != PageLayoutType::ONECOLUMN)
            writer.WriteElement("PageLayout", vps.PageLayout);
        if (TabDisplay != TabDisplayType::FILENAME)
            writer.WriteElement("TabDisplay", vps.TabDisplay);
        if (HideToolbar)
            writer.WriteElement("HideToolbar", vps.HideToolbar);
        if (HideMenubar)
            writer.WriteElement("HideMenubar", vps.HideMenubar);
        if (HideWindowUI)
            writer.WriteElement("HideWindowUI", vps.HideWindowUI);
        if (ZoomMode != ZoomModeType::DEFAULT)
            writer.WriteElement("ZoomMode", vps.ZoomMode);
        if (Zoom > 0.0){
            writer.WriteElement("Zoom", vps.Zoom);
        }
        
    } writer.EndElement();
}


bool VPreferences::FromXML(utils::XMLElementPtr element){
    CT_VPreferences vps;

    BEGIN_XML_CHILD_ELEMENTS_LOOP{

        XML_CHILD_ELEMENT_GET_OPTINAL_STRING("PageMode", vps.PageMode);
        XML_CHILD_ELEMENT_GET_OPTINAL_STRING("PageLayout", vps.PageLayout);
        XML_CHILD_ELEMENT_GET_OPTINAL_STRING("TabDisplay", vps.TabDisplay);
        XML_CHILD_ELEMENT_GET_OPTINAL_BOOLEAN("HideToolbar", vps.HideToolbar);
        XML_CHILD_ELEMENT_GET_OPTINAL_BOOLEAN("HideMenubar", vps.HideMenubar);
        XML_CHILD_ELEMENT_GET_OPTINAL_BOOLEAN("HideWindowUI", vps.HideWindowUI);
        XML_CHILD_ELEMENT_GET_OPTINAL_STRING("ZoomMode", vps.ZoomMode);
        XML_CHILD_ELEMENT_GET_OPTINAL_FLOAT("Zoom", vps.Zoom);

    } END_XML_CHILD_ELEMENTS_LOOP;

    FromCTVPreferences(vps);
    
    return true;
}
