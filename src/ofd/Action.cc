#include "ofd/Action.h"

using namespace ofd;

// ================ class Action ================

Action::Action(){

}

Action::~Action(){

}

void Action::GenerateXML(utils::XMLWriter &writer) const{

} 

bool Action::FromXML(utils::XMLElementPtr element){
	return true;
} 

// ================ class Actions ================

Actions::Actions(){

}

Actions::~Actions(){

}

void Actions::GenerateXML(utils::XMLWriter &writer) const{

} 

bool Actions::FromXML(utils::XMLElementPtr element){
	return true;
} 
