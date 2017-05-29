#include "IggStream.h"
#include "InFileBuffer.h"
#include "URLBuffer.h"

#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

namespace Ygg{

IggStream::IggStream() : istream(nullptr){
}

IggStream::~IggStream(){
	if(rdbuf() != nullptr)
		delete rdbuf();
}

void IggStream::open(){
	const string& store = getStore();
	const string& resource = getResource();

	if(store.compare("url") == 0){
		streambuf *oldBuffer = rdbuf(new URLBuffer(resource));
		if(oldBuffer != nullptr)
			delete oldBuffer;
	}
	else if(store.compare("file") == 0){
		ifstream fin;
		fin.open(resource);
/*		stringstream ss;
		ss << fin.rdbuf();
		fin.close();
		streambuf *oldBuffer = rdbuf(ss.rdbuf(nullptr));
		if(oldBuffer != nullptr)
			delete oldBuffer;*/
		streambuf *oldBuffer = rdbuf(new InFileBuffer(resource));
		if(oldBuffer != nullptr)
			delete oldBuffer;
	}
	else{
		cout << "Error in IggStream::open(): Unknwon store '" << store << "'.";
		exit(1);
	}
}

};	//End of namespace Ygg
