#include "ofd/Outline.h"
using namespace ofd;
using namespace utils;

OutlineeElem::OutlineElem(){
}

OutlineeElem::~OutlineElem(){
}

void OutlineElem::GenerateXML(XMLWriter &writer) const{
    writer.StartElement("OutlineElem");{

        writer.WriteAttribute("Name", strType);
        Dest.GenerateXML(writer);

    } writer.EndElement();
}

bool OutlineElem::FromXML(XMLElementPtr element){
    std::tie(Name, exist) = childElement->GetStringAttribute("OutlineElem");
    if (!exist){
        LOG_ERROR("Name attribute must be defined in Bookmark.");
        return false;
    }
    BEGIN_XML_CHILD_ELEMENTS_LOOP{
        
        Dest.FromXML(element);
        break;

    } END_XML_CHILD_ELEMENTS_LOOP;

    return true;
}
