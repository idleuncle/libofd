#include "ofd/Permission.h"
#include "utils/logger.h"

using namespace ofd;
using namespace utils;

Permission::Permission(){
}

Permission::~Permission(){
}

void Permission::GenerateXML(utils::XMLWriter &writer) const{
    writer.StartElement("Permissions");{

        if (!this->Edit){
            writer.WriteElement("Edit", false);
        }
        if (!this->Annot){
            writer.WriteElement("Annot", false);
        }
        if (!this->Export){
            writer.WriteElement("Export", false);
        }
        if (!this->Signature){
            writer.WriteElement("Signature", false);
        }
        if (!this->Watermark){
            writer.WriteElement("Watermark", false);
        }
        if (!this->PrintScreen){
            writer.WriteElement("PrintScreen", false);
        }
        if (this->PrintRestriction){
            writer.StartElement("Print");{
                writer.WriteAttribute("Printable", this->Printable);
                writer.WriteAttribute("Copies", this->Copies);
            } writer.EndElement();
        }
        if (this->ValidPeriodRestriction){
            writer.StartElement("ValidPeriod");{
                writer.WriteAttribute("StartDate", this->StartDate);
                writer.WriteAttribute("EndDDate", this->EndDate);
            } writer.EndElement();
        }

    } writer.EndElement();
}

bool Permission::FromXML(utils::XMLElementPtr objectElement){

    XMLElementPtr childElement = objectElement->GetFirstChildElement();
    while (childElement != nullptr){
        std::string childName = childElement->GetName();
        __attribute__((unused)) bool exist = false;

        if (childName == "Edit"){
            std::tie(this->Edit, std::ignore) = childElement->GetBooleanValue();
        } else if (childName == "Annot"){
            std::tie(this->Annot, std::ignore) = childElement->GetBooleanValue();
        } else if (childName == "Export"){
            std::tie(this->Export, std::ignore) = childElement->GetBooleanValue();
        } else if (childName == "Signature"){
            std::tie(this->Signature, std::ignore) = childElement->GetBooleanValue();
        } else if (childName == "Watermark"){
            std::tie(this->Watermark, std::ignore) = childElement->GetBooleanValue();
        } else if (childName == "PrintScreen"){
            std::tie(this->PrintScreen, std::ignore) = childElement->GetBooleanValue();
        } else if (childName == "Print"){
            std::tie(this->Printable, exist) = childElement->GetBooleanAttribute("Printable");
            if (!exist){
                LOG_ERROR("Printable must be defined in Print element of Permission.");
            }
            std::tie(this->Copies, std::ignore) = childElement->GetIntAttribute("Copies");
        } else if (childName == "ValidPeriod"){
            std::tie(this->StartDate, std::ignore) = childElement->GetStringAttribute("StartDate");
            std::tie(this->EndDate, std::ignore) = childElement->GetStringAttribute("EndDate");
        }

        childElement = childElement->GetNextSiblingElement();
    };

    return true;
}
