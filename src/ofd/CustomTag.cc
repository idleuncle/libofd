#include "ofd/CustomTag.h"

using namespace ofd;

// ================ class CustomTag ================

CustomTag::CustomTag(){

}

CustomTag::~CustomTag(){

}

void CustomTag::GenerateXML(utils::XMLWriter &writer) const{

} 

bool CustomTag::FromXML(utils::XMLElementPtr element){
	return true;
} 

// ================ class CustomTags ================

CustomTags::CustomTags(){

}

CustomTags::~CustomTags(){

}

void CustomTags::GenerateXML(utils::XMLWriter &writer) const{

} 

bool CustomTags::FromXML(utils::XMLElementPtr element){
	return true;
} 
