#include "IggStream.h"
#include "URLBuffer.h"

#include <iostream>

using namespace std;

namespace Yggdrasil{

IggStream::IggStream(){
}

IggStream::~IggStream(){
}

void IggStream::open(){
	const string& store = getStore();
	const string& resource = getResource();

	if(store.compare("url") == 0){
		delete rdbuf(new URLBuffer(resource));
	}
	else{
		cout << "Error in IggStream::open(): Unknwon store '" << store << "'.";
		exit(1);
	}
}

};	//End of namespace Yggdrasil
