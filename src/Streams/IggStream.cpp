#include "ReadException.h"
#include "IggStream.h"
#include "InFileBuffer.h"
#include "InStringBuffer.h"
#include "URLBuffer.h"
#include "YGGMacros.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include <openssl/sha.h>

using namespace std;

namespace Ygg{

IggStream::IggStream() : istream(nullptr){
}

IggStream::~IggStream(){
	if(rdbuf() != nullptr)
		delete rdbuf();
}

void IggStream::open(){
	locateResource();

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
		streambuf *oldBuffer = rdbuf(new InFileBuffer(resource));
		if(oldBuffer != nullptr)
			delete oldBuffer;
	}
	else if(store.compare("string") == 0){
		streambuf *oldBuffer = rdbuf(new InStringBuffer(resource));
		if(oldBuffer != nullptr)
			delete oldBuffer;
	}
	else{
		cout << "Error in IggStream::open(): Unknwon store '" << store << "'.\n";
		exit(1);
	}

	switch(getHashType()){
	case HashType::None:
		break;
	case HashType::SHA1:
		verifyHashSHA1();
		break;
	default:
		throw ReadException(
			"IggStream::open()",
			YGGWhere,
			"Unknown HashType.",
			"This should never happen, contact the developer."
		);
	}
}

bool IggStream::verifyHashSHA1(){
	SHA_CTX context;
	if(!SHA1_Init(&context)){
		throw ReadException(
			"IggStream::verifyHash()",
			YGGWhere,
			"Unable to initialize SHA1.",
			""
		);
	}
	while(!eof()){
		//This implementation is suboptimal. Should read larger chunks,
		//but first have to properly handle that the final chunk would
		//not necessarily be full size.
		const int CHUNK_SIZE = 1;
		string chunk(CHUNK_SIZE, ' ');
		read(&chunk[0], CHUNK_SIZE);
		if(eof())
			break;
		if(!SHA1_Update(&context, (unsigned char*)chunk.c_str(), CHUNK_SIZE)){
			throw ReadException(
				"IggStream::verifyHash()",
				YGGWhere,
				"An error occured while calculating the SHA1 hash.",
				""
			);
		}
	}
	unsigned char result[SHA_DIGEST_LENGTH];
	if(!SHA1_Final(result, &context)){
		throw ReadException(
			"IggStream::verifyHash()",
			YGGWhere,
			"An error occured when finalizing the SHA1 calculation.",
			""
		);
	}

	string hashString;
	for(unsigned int n = 0; n < SHA_DIGEST_LENGTH; n++){
		unsigned int ms = (unsigned int)(result[n]/16);
		unsigned int ls = (unsigned int)(result[n]%16);
		if(ms > 9)
			hashString += (char)(ms + 87);
		else
			hashString += (char)(ms + 48);
		if(ls > 9)
			hashString += (char)(ls + 87);
		else
			hashString += (char)(ls + 48);
	}

	if(hashString.compare(getHash()) != 0){
		throw ReadException(
			"IggStream::verifyHash()",
			YGGWhere,
			"The SHA1 of the incomming data does not match the"
			+ string(" specified hash."),
			"Use IggStream::setHash() to set the expected hash."
		);
	}
	clear();
	seekg(0);

	return true;
}

};	//End of namespace Ygg
