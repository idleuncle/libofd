#include "ofd/Bookmark.h"
#include "utils/logger.h"
using namespace ofd;
using namespace utils;

Bookmark::Bookmark(){
}

Bookmark::~Bookmark(){
}

void Bookmark::GenerateXML(XMLWriter &writer) const{
    writer.StartElement("Bookmark");{

        writer.WriteAttribute("Name", Name);
        Dest.GenerateXML(writer);

    } writer.EndElement();
}

bool Bookmark::FromXML(XMLElementPtr element){
    bool exist = false;
    std::tie(Name, exist) = element->GetStringAttribute("Name");
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
