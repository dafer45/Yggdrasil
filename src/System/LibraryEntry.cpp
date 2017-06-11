#include "LibraryEntry.h"

using namespace nlohmann;

namespace Ygg{

LibraryEntry::LibraryEntry(){
}

LibraryEntry::LibraryEntry(const nlohmann::json& libraryEntry){
	try{
		store = libraryEntry.at("store");
	}
	catch(json::exception e){
	}

	try{
		resource = libraryEntry.at("resource");
	}
	catch(json::exception e){
	}

	try{
		hash = libraryEntry.at("hash");
	}
	catch(json::exception e){
	}

	try{
		hashType = libraryEntry.at("hashType");
	}
	catch(json::exception e){
	}
}

LibraryEntry::~LibraryEntry(){
}

};	//End of namespace Ygg
