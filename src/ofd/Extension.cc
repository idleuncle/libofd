#include "ofd/Extension.h"

using namespace ofd;

// ================ class Extension ================

Extension::Extension(){

}

Extension::~Extension(){

}

void Extension::GenerateXML(utils::XMLWriter &writer) const{

} 

bool Extension::FromXML(utils::XMLElementPtr element){
	return true;
} 

// ================ class Extensions ================

Extensions::Extensions(){

}

Extensions::~Extensions(){

}

void Extensions::GenerateXML(utils::XMLWriter &writer) const{

} 

bool Extensions::FromXML(utils::XMLElementPtr element){
	return true;
} 
