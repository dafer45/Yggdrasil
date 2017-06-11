#include "CryptographicException.h"
#include "Configuration.h"
#include "IggStream.h"
#include "Library.h"
#include "LibraryEntry.h"
#include "ReadException.h"
#include "RSA.h"
#include "WriteException.h"

#include <fstream>

using namespace std;
using namespace nlohmann;

namespace Ygg{

Library::Library(){
}

Library::~Library(){
}

void Library::load(){
	Configuration configuration;
	configuration.load();

	string currentLibrary = configuration.getCurrentLibrary();
	if(currentLibrary.size() != 0){
		library = readJSON(currentLibrary);

		fileInfo.setStore("File");
		fileInfo.setResource(currentLibrary);

		return;
	}

	string currentDir = getenv("PWD") + string("/");
	while(currentDir.size() > 0){
		string filename = currentDir + "Library.json";
		ifstream fin(filename);
		if(fin){
			fin.close();
			library = readJSON(filename);

			fileInfo.setStore("File");
			fileInfo.setResource(filename);

			return;
		}
		currentDir.erase(currentDir.size()-1, 1);
		size_t pos = currentDir.find_last_of("/");
		currentDir = currentDir.substr(0, pos+1);
	}

	throw ReadException(
		"Library::load()",
		YGGWhere,
		"Unable to load library.",
		""
	);
}

void Library::load(const LibraryEntry& libraryEntry){
	string store = libraryEntry.getStore();

	size_t position = store.find("<");
	if(position == string::npos){
		throw ReadException(
			"Library::load()",
			YGGWhere,
			"The library entry is not a library file.",
			""
		);
	}

	string libraryStore = store.substr(0, position);
	string libraryResource = store.substr(position, store.size()-position);
	if(libraryResource.back() != '>'){
		throw ReadException(
			"Library::load()",
			YGGWhere,
			"The library entry's store entry appears malformated.",
			""
		);
	}
	libraryResource = libraryResource.substr(1, libraryResource.size()-2);

	IggStream iggStream;
	iggStream.setStore(libraryStore);
	iggStream.setResource(libraryResource);
	iggStream.setHash(libraryEntry.getHash());
	if(libraryEntry.getHashType().compare("") == 0){
	}
	else if(libraryEntry.getHashType().compare("sha1") == 0){
		iggStream.setHashType(YggStream::HashType::SHA1);
	}
	else{
		throw ReadException(
			"Library::load()",
			YGGWhere,
			"Hash type '" + libraryEntry.getHashType() + "'",
			""
		);
	}
	iggStream.open();

	string l(istream_iterator<char>(iggStream), {});
	library = json::parse(l);
	fileInfo.setStore(libraryStore);
	fileInfo.setResource(libraryResource);
/*	try{
		json j = json::parse(l);
		json content = j.at("content");
		for(json::iterator it = content.begin(); it != content.end(); ++it){
			string key = it.key();
			cout << key << "\n";
		}
	}
	catch(json::exception e){
		throw ReadException(
			"Library::load()",
			YGGWhere,
			"An error occured while parsing the library file.",
			""
		);
	}*/
}

void Library::save() const{
	if(fileInfo.getStore().compare("File") == 0){
		writeJSON(fileInfo.getResource(), library);
	}
	else{
		throw WriteException(
			"Library::save()",
			YGGWhere,
			"Unable to save library.",
			""
		);
	}

/*	Configuration configuration;
	configuration.load();

	string currentLibrary = configuration.getCurrentLibrary();
	if(currentLibrary.size() != 0){
		writeJSON(currentLibrary, library);
		return;
	}

	string currentDir = getenv("PWD") + string("/");
	while(currentDir.size() > 0){
		string filename = currentDir + "Library.json";
		ifstream fin(filename);
		if(fin){
			fin.close();
			writeJSON(filename, library);
			return;
		}
		currentDir.erase(currentDir.size()-1, 1);
		size_t pos = currentDir.find_last_of("/");
		currentDir = currentDir.substr(0, pos+1);
	}

	throw ReadException(
		"Library::save()",
		YGGWhere,
		"Unable to save library.",
		""
	);*/
}

void Library::addLibraryEntry(const LibraryEntry& libraryEntry){
	library.at("content").at("entries")[libraryEntry.getKey()] = libraryEntry.getJSONValue();
}

void Library::updateLibraryEntry(const LibraryEntry& libraryEntry){
	string key = libraryEntry.getKey();
	string store = libraryEntry.getStore();
	string resource = libraryEntry.getResource();
	string hash = libraryEntry.getHash();
	string hashType = libraryEntry.getHashType();

	json entries = library.at("content").at("entries");
	if(store.compare("") != 0)
		entries[key]["store"] = store;
	if(resource.compare("") != 0)
		entries[key]["resource"] = resource;
	if(hash.compare("") != 0)
		entries[key]["hash"] = hash;
	if(hashType.compare("") != 0)
		entries[key]["hashType"] = hashType;
	library["content"]["entries"] = entries;
}

void Library::removeLibraryEntry(const LibraryEntry& libraryEntry){
	json entries = library.at("content").at("entries");
	entries.erase(libraryEntry.getKey());
	library["content"]["entries"] = entries;
}

void Library::link(const Library& library){
	string publicKey;
	try{
		publicKey = library.library.at("publicKey");
	}
	catch(json::exception e){
		throw ReadException(
			"Library::link",
			YGGWhere,
			"Unable to link with library that does not have a"
				+ string(" public key."),
			""
		);
	}

	try{

		json publicKeys;
		try{
			publicKeys = this->library.at("content").at("publicKeys");
		}
		catch(json::exception e){
			this->library.at("content")["publicKeys"] = {};
			publicKeys = this->library.at("content").at("publicKeys");
		}
		publicKeys[library.fileInfo.getStore() + "<" + library.fileInfo.getResource() + ">"] = publicKey;
		this->library.at("content").at("publicKeys") = publicKeys;

		json entries = library.library.at("content").at("entries");
		for(json::iterator it = entries.begin(); it != entries.end(); ++it){
			LibraryEntry libraryEntry;
			libraryEntry.setKey(it.key());
			libraryEntry.setStore(
				library.fileInfo.getStore() + "<"
				+ library.fileInfo.getResource() + ">"
			);
			libraryEntry.setResource(it.key());

			addLibraryEntry(libraryEntry);
		}
	}
	catch(json::exception e){
		throw ReadException(
			"Library::load()",
			YGGWhere,
			"An error occured while parsing the library file.",
			""
		);
	}
}

void Library::setPublicKey(const std::string& publicKey){
	library["publicKey"] = publicKey;
}

void Library::sign(const std::string& privateKey){
	json content = library.at("content");
	RSA rsa;
	rsa.setPrivateKey(privateKey);
	string signature = rsa.getSignature(content.dump());
	library["signature"] = rsa.encode64(signature);
}

void Library::verify(const std::string& publicKey) const{
	string content = library.at("content").dump();
	string signature = library.at("signature").dump();
	signature.erase(signature.size()-1, 1).erase(0, 1);

	RSA rsa;
	signature = rsa.decode64(signature);
	rsa.setPublicKey(publicKey);
	if(!rsa.verifySignature(signature, content)){
		throw CryptographicException(
			"Library::verify()",
			YGGWhere,
			"Signature verification failed.",
			""
		);
	}
}

LibraryEntry Library::getLibraryEntry(const std::string& key) const{
	try{
		json entry = library.at("content").at("entries").at(key);
		return LibraryEntry(entry);
	}
	catch(json::exception e){
		throw ReadException(
			"Library::getLibraryEntry()",
			YGGWhere,
			"Unable to find library entry.",
			""
		);
	}
}

json Library::readJSON(const std::string& filename) const{
	ifstream fin(filename);
	if(!fin){
		throw ReadException(
			"Library::readJSON()",
			YGGWhere,
			"Unable to open file '" + filename + "'.",
			""
		);
	}
	string j(istream_iterator<char>(fin), {});
	fin.close();

	try{
		return json::parse(j);
	}
	catch(...){
		throw ReadException(
			"Library::readJSON()",
			YGGWhere,
			"Unable to parse file '" + filename + "'.",
			""
		);
	}
}

void Library::writeJSON(const std::string& filename, const json& j) const{
	ofstream fout(filename);
	if(!fout){
		throw ReadException(
			"Library::writeJSON()",
			YGGWhere,
			"Unable to open file '" + filename + "'.",
			""
		);
	}

	fout << library.dump();
	fout.close();
}

};	//End of namespace Ygg
