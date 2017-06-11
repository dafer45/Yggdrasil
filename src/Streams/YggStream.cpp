#include "IggStream.h"
#include "ReadException.h"
#include "RSA.h"
#include "YGGMacros.h"
#include "YggStream.h"

#include <iostream>

#include "json.hpp"

#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>

using namespace std;
using namespace nlohmann;

namespace Ygg{

YggStream::YggStream(){
	hashType = HashType::None;
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
		iggStream.setHashType(hashType);
		iggStream.setHash(hash);
		iggStream.open();

		hashType = HashType::None;
		hash = "";

		string library(istream_iterator<char>(iggStream), {});
		try{
			json j = json::parse(library);
			json content = j.at("content");
			if(publicKey.compare("") != 0)
				verifySignature(j.at("signature"), content);
			json libraryEntry = content.at("entries").at(resource);
			store = libraryEntry.at("store");
			try{
				resource = libraryEntry.at("resource");
			}
			catch(json::exception e){
				//It is OK to not have a resource name.
			}
			try{
				string hashTypeString = libraryEntry.at("hashType");
				if(toLowerCase(hashTypeString).compare("sha1") == 0){
					hashType = HashType::SHA1;
				}
				else{
					throw ReadException(
						"YggStream::locateResource()",
						YGGWhere,
						string("Unknown hash type: ") + hashTypeString,
						""
					);
				}
			}
			catch(json::exception e){
				//It is OK to not have a hashType.
			}

			if(hashType != HashType::None)
				hash = libraryEntry.at("hash");
		}
		catch(json::exception e){
			throw ReadException(
				"YggStream.cpp",
				YGGWhere,
				"Unable to dereference resource.",
				""
			);
/*			cout << "Error in YggStream::getStore(): " << e.what() << "\n";
			exit(1);*/
		}
	}
}

const string YggStream::getResource() const{
	return resource;
}

void YggStream::verifySignature(const json& signature, const json& content){
	string s = signature.dump();
	string c = content.dump();

	RSA rsa;
	s.erase(0, 1);
	s.erase(s.size()-1, 1);
	size_t pos;
	while((pos = s.find("\\n")) != string::npos){
		s.erase(pos, 1);
		s.replace(pos, 1, "\n");
	}
	s = rsa.decode64(s);
	rsa.setPublicKey(publicKey);
	if(!rsa.verifySignature(s, c)){
		throw ReadException(
			"YggStream::locateResource()",
			YGGWhere,
			"Invalid signautre.",
			""
		);
	}
}

};	//End of namespace Ygg
