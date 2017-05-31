#include "IggStream.h"
#include "YggStream.h"

#include <iostream>

using namespace std;

namespace Ygg{

YggStream::YggStream(){
}

YggStream::~YggStream(){
}

const string YggStream::getStore() const{
	size_t position = store.find("<");
	if(position == string::npos)
		return toLowerCase(store);

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
	while(!iggStream.eof()){
		string line;
		getline(iggStream, line);
		cout << line << "\n";
	}
	exit(1);
}

const string YggStream::getResource() const{
	return resource;
}

};	//End of namespace Ygg
