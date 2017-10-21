#include "ofd/Attachment.h"
#include "utils/logger.h"
using namespace ofd;
using namespace utils;

// ================ class Attachment ================

Attachment::Attachment(){
}

Attachment::~Attachment(){
}

void Attachment::GenerateXML(XMLWriter &writer) const{
    writer.StartElement("Attachment");{

        writer.WriteAttribute("ID", this->ID);
        writer.WriteAttribute("Name", this->Name);
        if (!this->Format.empty())
            writer.WriteAttribute("Format", this->Format);
        if (!this->CreationDate.empty())
            writer.WriteAttribute("CreationDate", this->CreationDate);
        if (!this->ModDate.empty())
            writer.WriteAttribute("ModDate", this->ModDate);
        if (this->Size > 0.0)
            writer.WriteAttribute("Size", this->Size);
        if (!this->Visible)
            writer.WriteAttribute("Visible", this->Visible);
        if (!this->Usage.empty())
            writer.WriteAttribute("Usage", this->Usage);
        if (!this->FileLoc.empty())
            writer.WriteAttribute("FileLoc", this->FileLoc);

    } writer.EndElement();
}

bool Attachment::FromXML(XMLElementPtr element){
    bool exist = false;
    std::tie(this->ID, exist) = element->GetIntAttribute("ID");
    if (!exist){
        LOG_ERROR("ID attribute must be defined in Attachment.");
        return false;
    }
    std::tie(this->Name, exist) = element->GetStringAttribute("NAME");
    if (!exist){
        LOG_ERROR("Name attribute must be defined in Attachment.");
        return false;
    }
    std::tie(this->Format, std::ignore) = element->GetStringAttribute("Format");
    std::tie(this->CreationDate, std::ignore) = element->GetStringAttribute("CreationDate");
    std::tie(this->ModDate, std::ignore) = element->GetStringAttribute("ModDate");
    std::tie(this->Size, std::ignore) = element->GetFloatAttribute("Size");
    std::tie(this->Visible, std::ignore) = element->GetBooleanAttribute("Visible");
    std::tie(this->Usage, std::ignore) = element->GetStringAttribute("Usage");


    BEGIN_XML_CHILD_ELEMENTS_LOOP{
        
        if ( childName == "FileLoc" ) {
            std::tie(this->FileLoc, std::ignore) = childElement->GetStringValue();
            break;
        }

    } END_XML_CHILD_ELEMENTS_LOOP;

    return true;
}

// ================ class Attachments ================

Attachments::Attachments(){

}

Attachments::~Attachments(){

}

void Attachments::GenerateXML(utils::XMLWriter &writer) const{

} 

bool Attachments::FromXML(utils::XMLElementPtr element){
    return true;
} 
