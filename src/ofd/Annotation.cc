#include "ofd/Annotation.h"

using namespace ofd;

// ================ class Annotation ================

Annotation::Annotation(){

}

Annotation::~Annotation(){

}

void Annotation::GenerateXML(utils::XMLWriter &writer) const{

} 

bool Annotation::FromXML(utils::XMLElementPtr element){
	return true;
} 

// ================ class Annotations ================

Annotations::Annotations(){

}

Annotations::~Annotations(){

}

void Annotations::GenerateXML(utils::XMLWriter &writer) const{

} 

bool Annotations::FromXML(utils::XMLElementPtr element){
	return true;
} 

// ================ class Annot ================

Annot::Annot(){

}

Annot::~Annot(){

}

void Annot::GenerateXML(utils::XMLWriter &writer) const{

} 

bool Annot::FromXML(utils::XMLElementPtr element){
	return true;
} 

// ================ class PageAnnot ================

PageAnnot::PageAnnot(){

}

PageAnnot::~PageAnnot(){

}

void PageAnnot::GenerateXML(utils::XMLWriter &writer) const{
    
} 

bool PageAnnot::FromXML(utils::XMLElementPtr element){
	return true;
} 
