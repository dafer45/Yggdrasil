#include "Configuration.h"

using namespace nlohmann;

namespace Ygg{

Configuration::Configuration(){
}

Configuration::Configuration(const json& configuration){
	try{
		currentLibrary = configuration.at("currentLibrary").dump();
		currentLibrary.erase(currentLibrary.size()-1, 1).erase(0, 1);
	}
	catch(...){
	}
}

Configuration::~Configuration(){
}

json Configuration::getJSON() const{
	json j;
	j["currentLibrary"] = currentLibrary;

	return j;
}

};	//End of namespace Ygg
