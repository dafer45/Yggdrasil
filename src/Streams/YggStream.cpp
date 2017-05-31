#include "IggStream.h"
#include "YggStream.h"

#include <iostream>

#include "json.hpp"

using namespace std;
using namespace nlohmann;

namespace Ygg{

YggStream::YggStream(){
}

YggStream::~YggStream(){
}

const string YggStream::getStore() const{
	return toLowerCase(store);
}

void YggStream::locateResource(){
	while(true){
		size_t position = store.find("<");
		if(position == string::npos)
			break;

		string libraryStore = store.substr(0, position);
		string libraryResource = store.substr(position, store.size()-position);
		if(libraryResource.back() != '>'){
			cout << "Error in YggStream::getStore(): Unable to parse tag '"
				<< libraryResource << "'.\n";
			exit(1);
		}
		libraryResource = libraryResource.substr(1, libraryResource.size()-2);

		IggStream iggStream;
		iggStream.setStore(libraryStore);
		iggStream.setResource(libraryResource);
		iggStream.open();

		string library(istream_iterator<char>(iggStream), {});
		try{
			json j = json::parse(library);
			json libraryEntry = j.at(resource);
			store = libraryEntry.at("store");
			try{
				resource = libraryEntry.at("resource");
			}
			catch(json::exception e){
				//It is OK to not have a resource name.
			}
		}
		catch(json::exception e){
			cout << "Error in YggStream::getStore(): " << e.what() << "\n";
			exit(1);
		}
	}
}

const string YggStream::getResource() const{
	return resource;
}

};	//End of namespace Ygg
