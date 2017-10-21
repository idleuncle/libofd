#include <string>
#include "ofd/Outline.h"
#include "utils/logger.h"
using namespace ofd;
using namespace utils;

// ================ class OutlineElem ================

OutlineElem::OutlineElem(){
}

OutlineElem::~OutlineElem(){
}

void OutlineElem::GenerateXML(XMLWriter &writer) const{
    writer.StartElement("OutlineElem");{

        writer.WriteAttribute("Title", this->Title);
        writer.WriteAttribute("Count", this->Count);
        writer.WriteAttribute("Expanded", this->Expanded);
        // TODO Actions

    } writer.EndElement();
}

bool OutlineElem::FromXML(XMLElementPtr element){
    bool exist = false;
    std::tie(this->Title, exist) = element->GetStringAttribute("Title");
    if (!exist){
        LOG_ERROR("Title attribute of OutlineElem must be given.");
        return false;
    }
    std::tie(this->Count, std::ignore) = element->GetIntAttribute("Count");
    std::tie(this->Expanded, std::ignore) = element->GetBooleanAttribute("Expanded");

    // TODO Actions
    //
    //BEGIN_XML_CHILD_ELEMENTS_LOOP{
        
        //Dest.FromXML(element);
        //break;

    //} END_XML_CHILD_ELEMENTS_LOOP;

    return true;
}

// ================ class Outlines ================

Outlines::Outlines(){

}

Outlines::~Outlines(){

}

void Outlines::GenerateXML(utils::XMLWriter &writer) const{

} 

bool Outlines::FromXML(utils::XMLElementPtr element){
    return true; 
} 
