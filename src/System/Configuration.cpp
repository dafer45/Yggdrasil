#include "Configuration.h"
#include "ReadException.h"
#include "YGGMacros.h"

#include <fstream>
#include <string>

using namespace std;
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

void Configuration::load(){
	ensureConfigurationFileExists();
	try{
		string homeDir = getenv("HOME");
		string filename = homeDir + "/.ygg/config";
		ifstream fin(filename);
		if(!fin){
			throw ReadException(
				"Configuration::load()",
				YGGWhere,
				"Unable to open configuration file.",
				""
			);
		}
		string content(istream_iterator<char>(fin), {});
		fin.close();

		json configuration = json::parse(content);

		try{
			currentLibrary = configuration.at("currentLibrary").dump();
			currentLibrary.erase(currentLibrary.size()-1, 1).erase(0, 1);
		}
		catch(json::exception e){
			//It is OK for currentLibrary to not exist.
		}
	}
	catch(json::exception e){
		throw ReadException(
			"Configuration::load()",
			YGGWhere,
			"Unable to parse cofiguration file.",
			""
		);
	}
}

json Configuration::getJSON() const{
	json j;
	j["currentLibrary"] = currentLibrary;

	return j;
}

void Configuration::ensureConfigurationFileExists(){
	string homeDir = getenv("HOME");
	string filename = homeDir += "/.ygg/config";
	ifstream fin(filename);
	if(!fin){
		ofstream fout(filename);
		if(!fout){
			throw ReadException(
				"Configuration::load()",
				YGGWhere,
				"Unable to create configuration file.",
				""
			);
		}
		fout << "{}";
		fout.close();
	}
	else{
		fin.close();
	}
}

};	//End of namespace Ygg
