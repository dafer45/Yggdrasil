#include "IggStream.h"
#include "URLBuffer.h"

#include <iostream>

using namespace std;

namespace Ygg{

IggStream::IggStream() : istream(nullptr){
}

IggStream::~IggStream(){
}

void IggStream::open(){
	const string& store = getStore();
	const string& resource = getResource();

	if(store.compare("url") == 0){
		streambuf *oldBuffer = rdbuf(new URLBuffer(resource));
		if(oldBuffer != nullptr)
			delete oldBuffer;
	}
	else{
		cout << "Error in IggStream::open(): Unknwon store '" << store << "'.";
		exit(1);
	}
}

};	//End of namespace Ygg
