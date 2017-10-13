#include "ofd/Common.h"
using namespace ofd;
using namespace utils;

void ST_Dest::GenerateXML(XMLWriter &writer) const{
    writer.StartElement("Dest");{

        std::string strType = "XYZ";
        if (Type == DestType::XYZ){
            strType = "XYZ";
        } else if (Type == DestType::Fit){
            strType = "Fit";
        } else if (Type == DestType::FitH){
            strType = "FitH";
        } else if (Type == DestType::FitV){
            strType = "FitV";
        } else if (Type == DestType::FitR){
            strType = "FitR";
        }
        writer.WriteAttribute("Type", strType);
        writer.WriteAttribute("PageID", PageID);
        if (Type == DestType::XYZ){
            writer.WriteAttribute("Left", Left);
            writer.WriteAttribute("Top", Top);
            writer.WriteAttribute("Zoom", Zoom);
        } else if (Type == DestType::FitH){
            writer.WriteAttribute("Top", Top);
        } else if (Type == DestType::FitV){
            writer.WriteAttribute("Left", Left);
        } else if (Type == DestType::FitR){
            writer.WriteAttribute("Left", Left);
            writer.WriteAttribute("Top", Top);
            writer.WriteAttribute("Right", Right);
            writer.WriteAttribute("Bottom", Bottom);
        }

    } writer.EndElement();
}

bool ST_Dest::FromXML(XMLElementPtr element){
    BEGIN_XML_CHILD_ELEMENTS_LOOP{
        
        std::string strType = "XYZ";
        std::tie(strType, exist) = childElement->GetStringAttribute("Type");
        if (!exist){
            LOG_ERROR("Type attribute must be defined in ST_Dest.");
            return false;
        }
        if (strType == "XYZ"){
            this->Type = DestType::XYZ;
        else if (strType == "Fit"){
            this->Type = DestType::FIT;
        else if (strType == "FitH"){
            this->Type = DestType::FITH;
        else if (strType == "FitV"){
            this->Type = DestType::FITV;
        else if (strType == "FitR"){
            this->Type = DestType::FITR;
        }

        std::tie(this->PageID, exist) = childElement->GetIntAttribute("PageID");
        if (!exit){
            LOG_ERROR("PageID attribute must be defined in ST_Dest.");
            return false;
        }

        std::tie(this->Left, std::ignore) = childElement->GetFloatAttribute("Left");
        std::tie(this->Top, std::ignore) = childElement->GetFloatAttribute("Top");
        std::tie(this->Right, std::ignore) = childElement->GetFloatAttribute("Right");
        std::tie(this->Bottom, std::ignore) = childElement->GetFloatAttribute("Bottom");
        std::tie(this->Zoom, std::ignore) = childElement->GetFloatAttribute("Zoom");

    } END_XML_CHILD_ELEMENTS_LOOP;

    return true;
}
