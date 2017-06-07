#include "Configuration.h"
#include "CryptographicException.h"
#include "LibraryEntry.h"
#include "RSA.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <string>

#include <stdlib.h>
#include <getopt.h>

#include "json.hpp"

using namespace std;
using namespace nlohmann;
using namespace Ygg;

void addLibraryEntry(const LibraryEntry& libraryEntry);
void addPublicKey(const string& publicKeyFilename);
string argumentToKey(const string& argument);
string argumentToStore(const string& argument);
string argumentToResource(const string& argument);
void configure(const Configuration& configuration);
void createLibrary(const string& filename);
void ensureConfigurationFileExists();
Configuration getConfiguration();
string getKey(const string& filename);
void list();
void printUsageInfo();
json readJSON(const string& filename);
json readLibrary();
void removeLibraryEntry(const LibraryEntry& libraryEntry);
void sign(const string& privateKey);
void updateLibraryEntry(const LibraryEntry& libraryEntry);
void verify(const string& publicKey);
void writeConfiguration(const Configuration& configuration);
void writeJSON(const string& filename, const json& j);
void writeLibrary(const json& j);

int main(int argc, char **argv){
	if(argc < 2)
		printUsageInfo();

	string command = argv[1];

	string argument;
	if(argc > 2){
		argument = argv[2];
		if(argument.substr(0, 2).compare("--") == 0)
			argument = "";
	}

	string privateKeyFilename = "";
	string publicKeyFilename = "";

//	string currentLibrary = "";

	string key;
	string store;
	string resource;

	while(true){
		static struct option longOptions[] = {
			//Sets flags.
			//Does not set flags.
			{"key",			required_argument,	0,	'k'},
			{"private-key",		required_argument,	0,	'P'},
			{"public-key",		required_argument,	0,	'p'},
			{"resource",		required_argument,	0,	'r'},
			{"store",		required_argument,	0,	's'},
			{0,			0,			0,	0}
		};

		int optionIndex = 0;
		int c = getopt_long(argc, argv, "k:P:p:r:s:", longOptions, &optionIndex);
		if(c == -1)
			break;

		switch(c){
		case 0:
			if(longOptions[optionIndex].flag != 0)
				break;
			cerr << "Option " << longOptions[optionIndex].name;
			if(optarg)
				cerr << " with argument " << optarg;
			cerr << "\n";
			break;
		case 'k':
			key = optarg;
			break;
		case 'P':
			privateKeyFilename = optarg;
			break;
		case 'p':
			publicKeyFilename = optarg;
			break;
		case 'r':
			resource = optarg;
			break;
		case 's':
			store = optarg;
			break;
		default:
			cerr << "Unknown argument.\n";
			exit(1);
		}
	}

	if(command.compare("add") == 0){
		LibraryEntry libraryEntry;
		if(argument.size() != 0){
			libraryEntry.setKey(argumentToKey(argument));
			libraryEntry.setStore(argumentToStore(argument));
			libraryEntry.setResource(argumentToResource(argument));
		}

		if(key.size() != 0)
			libraryEntry.setKey(key);
		if(store.size() != 0)
			libraryEntry.setStore(store);
		if(resource.size() != 0)
			libraryEntry.setResource(resource);
		addLibraryEntry(libraryEntry);
	}
	else if(command.compare("add-public-key") == 0){
		addPublicKey(publicKeyFilename);
	}
	else if(command.compare("create") == 0){
		createLibrary(argument);
	}
	else if(command.compare("info") == 0){
		Configuration configuration = getConfiguration();
		configuration.print();
	}
	else if(command.compare("ls") == 0){
		list();
	}
	else if(command.compare("release-library") == 0){
		Configuration configuration = getConfiguration();
		configuration.setCurrentLibrary("");
		writeConfiguration(configuration);
	}
	else if(command.compare("remove") == 0){
		LibraryEntry libraryEntry;
		if(argument.size() != 0){
			libraryEntry.setKey(argument);
		}
		else{
			cout << "remove requires one argument.\n";
			exit(1);
		}
		removeLibraryEntry(libraryEntry);
	}
	else if(command.compare("set-library") == 0){
		Configuration configuration;
		if(argument.size() != 0){
			configuration.setCurrentLibrary(argument);
			configure(configuration);
		}
		else{
			cout << "set-library requires one argument.\n";
			exit(1);
		}
	}
	else if(command.compare("sign") == 0){
		sign(privateKeyFilename);
	}
	else if(command.compare("update") == 0){
		LibraryEntry libraryEntry;
		if(argument.compare("") != 0){
			libraryEntry.setKey(argument);
		}
		else{
			cout << "update requires one argument.\n";
			exit(1);
		}
		if(store.size() != 0)
			libraryEntry.setStore(store);
		if(resource.size() != 0)
			libraryEntry.setResource(resource);
		updateLibraryEntry(libraryEntry);
	}
	else if(command.compare("verify") == 0){
		verify(publicKeyFilename);
	}
	else{
		printUsageInfo();
	}

	return 0;
}

void addLibraryEntry(
	const LibraryEntry& libraryEntry
){
	json library = readLibrary();
	try{
		library.at("content")[libraryEntry.getKey()] = libraryEntry.getJSONValue();
	}
	catch(...){
		cout << "Unable to add library entry.\n";
	}
	writeLibrary(library);
}

void addPublicKey(
	const string& publicKeyFilename
){
	string publicKey = getKey(publicKeyFilename);

	json library = readLibrary();
	try{
		library["publicKey"] = publicKey;
		writeLibrary(library);
	}
	catch(json::exception e){
		cout << "Unable to parse json file.\n";
		exit(1);
	}
}

string argumentToKey(const string& argument){
	size_t pos = argument.find("://");
	if(pos != string::npos)
		return argument.substr(pos + 3, argument.size() - pos - 3);

	return argument;
}

string argumentToStore(const string& argument){
	size_t pos = argument.find("://");
	if(pos != string::npos)
		return "URL";

	return "File";
}

string argumentToResource(const string& argument){
	size_t pos = argument.find("://");
	if(pos != string::npos)
		return argument;

	string currentWorkingDirectory = getenv("PWD");
	return currentWorkingDirectory + "/" + argument;
}

void configure(const Configuration& configuration){
	Configuration c = getConfiguration();
	string currentLibrary = configuration.getCurrentLibrary();
	if(currentLibrary.compare("") != 0){
		string filename;

		size_t pos = currentLibrary.find("global:");
		if(pos == 0){
			filename = string(getenv("HOME")) + "/.ygg/"
				+ currentLibrary.substr(
					7,
					currentLibrary.size()-7
				)
				+ "/Library.json";
		}
		else{
			filename = string(getenv("PWD")) + "/"
				+ currentLibrary;
		}

		ifstream fin(filename);
		if(!fin){
			cout << "Library does not exist.\n";
			exit(1);
		}
		c.setCurrentLibrary(filename);
	}

	writeConfiguration(c);
}

void createLibrary(const string& filename){
	string f = filename;
	if(f.compare("") == 0)
		f = "Library.json";

	size_t pos = filename.find("global:");
	if(pos == 0){
		string homeDir = getenv("HOME");
		string libDir = homeDir + "/.ygg/" + filename.substr(7, filename.size()-7) + "/";
		f = libDir + "Library.json";
		system(("mkdir " + libDir).c_str());
	}
	ofstream fout(f);
	if(!fout){
		cout << "Unable to open file '" << filename << "'.";
		exit(1);
	}
	fout << "{\"content\":{}}";
	fout.close();
}

void ensureConfigurationFileExists(){
	string homeDir = getenv("HOME");
	string filename = homeDir + "/.ygg/config";
	ifstream fin(filename);
	if(!fin){
		ofstream fout(filename);
		if(!fout){
			cout << "Unable to initialize configuration file.\n";
			exit(1);
		}
		fout << "{}";
		fout.close();
	}
	else{
		fin.close();
	}
}

Configuration getConfiguration(){
	ensureConfigurationFileExists();
	try{
		string homeDir = getenv("HOME");
		string filename = homeDir + "/.ygg/config";
		return Configuration(readJSON(filename));
	}
	catch(...){
		cout << "Unable to open global library file.\n";
	}
}

string getKey(const string& filename){
	string key;
	if(filename.compare("") == 0){
		while(true){
			string line;
			getline(cin, line);
			if(cin.eof())
				break;
			if(
				line.compare("exit") == 0
				|| line.compare("exit()") == 0
				|| line.compare("quit") == 0
				|| line.compare("quit()") == 0
			){
				break;
			}
			key += line + "\n";
		}
	}
	else{
		ifstream fin(filename);
		if(!fin){
			cout << "Unable to open file '" << filename
				<< "'.";
			exit(1);
		}

		while(!fin.eof()){
			string line;
			getline(fin, line);
			key += line + "\n";
		}
	}

	return key;
}

void list(){
	json library = readLibrary();
	try{
		json content = library.at("content");
		cout << left;
		cout << setw(20) << "Key" << setw(20) << "Store" << setw(80) << "Resource" << "\n";
		for(json::iterator it = content.begin(); it != content.end(); ++it){
			string key = it.key();
			string store = it->at("store");
			string resource;
			try{
				resource = it->at("resource");
			}
			catch(json::exception e){
			}
			cout << setw(20) << key << setw(20) << store << setw(80) << resource << "\n";
		}
	}
	catch(...){
		cout << "An error occured while listing the library content.\n";
		exit(1);
	}
}

void printUsageInfo(){
	const int INDENT = 4;
	cout << left;
	cout << "Usage:\n";
	cout << setw(INDENT) << "" << "ygg <command>\n";
	cout << "\n";
	cout << "Commands:\n";
	cout << setw(INDENT) << "" << setw(20) << "add"			<< "Add library entry.\n";
	cout << setw(INDENT) << "" << setw(20) << "add-public-key"	<< "Add public key.\n";
	cout << setw(INDENT) << "" << setw(20) << "create"		<< "Create library file.\n";
	cout << setw(INDENT) << "" << setw(20) << "info"		<< "Create configuration information.\n";
	cout << setw(INDENT) << "" << setw(20) << "ls"			<< "Display content.\n";
	cout << setw(INDENT) << "" << setw(20) << "release-library"	<< "Release the current library.\n";
	cout << setw(INDENT) << "" << setw(20) << "set-library"		<< "Set the current library.\n";
	cout << setw(INDENT) << "" << setw(20) << "sign"		<< "Sign file with private key.\n";
	cout << setw(INDENT) << "" << setw(20) << "verify"		<< "Verify file with public key.\n";
	cout << "\n";
	cout << "Flags:\n";
	cout << setw(INDENT) << "" << setw(20) << "--key"		<< "\n";
	cout << setw(INDENT) << "" << setw(20) << "--library"		<< "\n";
	cout << setw(INDENT) << "" << setw(20) << "--private-key"	<< "\n";
	cout << setw(INDENT) << "" << setw(20) << "--public-key"	<< "\n";
	cout << setw(INDENT) << "" << setw(20) << "--resource"		<< "\n";
	cout << setw(INDENT) << "" << setw(20) << "--store"		<< "\n";

	exit(1);
}

json readJSON(const std::string& filename){
	ifstream fin(filename);
	if(!fin){
		cout << "Unable to open file '" << filename << "'.";
		exit(1);
	}
	string j(istream_iterator<char>(fin), {});
	fin.close();

	try{
		return json::parse(j);
	}
	catch(...){
		cout << "Unable to parse json file.\n";
		exit(1);
	}
}

json readLibrary(){
	try{
		Configuration configuration = getConfiguration();

		string currentLibrary = configuration.getCurrentLibrary();
		if(currentLibrary.size() != 0)
			return readJSON(currentLibrary);
	}
	catch(...){
		cout << "Unable to open global library file.\n";
	}

	string currentDir = getenv("PWD") + string("/");
	while(currentDir.size() > 0){
		string f = currentDir + "Library.json";
		ifstream fin(f);
		if(fin){
			fin.close();
			return readJSON(f);
		}
		currentDir.erase(currentDir.size()-1, 1);
		size_t pos = currentDir.find_last_of("/");
		currentDir = currentDir.substr(0, pos+1);
	}

}

void removeLibraryEntry(
	const LibraryEntry& libraryEntry
){
	json library = readLibrary();
	try{
		json content = library.at("content");
		content.erase(libraryEntry.getKey());
		library["content"] = content;
		writeLibrary(library);
	}
	catch(...){
		cout << "Unable to modify library entry.\n";
	}
}

void sign(const string& privateKeyFilename){
	json library = readLibrary();
	try{
		json content = library.at("content");
		RSA rsa;
		rsa.setPrivateKey(getKey(privateKeyFilename));
		string signature = rsa.getSignature(content.dump());
		library["signature"] = rsa.encode64(signature);
		writeLibrary(library);
	}
	catch(...){
		cout << "Unable to sign file.\n";
		exit(1);
	}
}

void updateLibraryEntry(
	const LibraryEntry& libraryEntry
){
	json library = readLibrary();
	try{
		string key = libraryEntry.getKey();
		string store = libraryEntry.getStore();
		string resource = libraryEntry.getResource();

		json content = library.at("content");
		if(store.compare("") != 0)
			content[libraryEntry.getKey()]["store"] = store;
		if(resource.compare("") != 0)
			content[libraryEntry.getKey()]["resource"] = resource;
		library["content"] = content;
		writeLibrary(library);
	}
	catch(...){
		cout << "Unable to modify library entry.\n";
	}
}

void verify(const string& publicKeyFilename){
	json library = readLibrary();
	try{
		RSA rsa;
		string content = library.at("content").dump();
		string signature = library.at("signature").dump();
		signature.erase(signature.size()-1, 1).erase(0, 1);
		signature = rsa.decode64(signature);
		rsa.setPublicKey(getKey(publicKeyFilename));
		cout << getKey(publicKeyFilename);
		if(rsa.verifySignature(signature, content)){
			cout << "Verification succeeded.\n";
			exit(1);
		}
		else{
			cout << "Verification failed.\n";
			exit(1);
		}
	}
	catch(CryptographicException e){
		cout << "Unable to verify file. Did you specify the correct"
			<< " public key file using --public-key?\n";
		e.print();
		exit(1);
	}
	catch(...){
		cout << "Unable to verify file.\n";
		exit(1);
	}
}

void writeConfiguration(const Configuration& configuration){
	try{
		string homeDir = getenv("HOME");
		string filename = homeDir + "/.ygg/config";
		return writeJSON(filename, configuration.getJSON());
	}
	catch(...){
		cout << "Unable to write global library file.\n";
	}
}

void writeJSON(const string& filename, const json& j){
	ofstream fout(filename);
	if(!fout){
		cout << "Unable to open file '" << filename << "'.";
		exit(1);
	}

	fout << j.dump();
	fout.close();
}

void writeLibrary(const json& j){
	try{
		Configuration configuration = getConfiguration();

		string currentLibrary = configuration.getCurrentLibrary();
		if(currentLibrary.size() != 0){
			writeJSON(currentLibrary, j);
			return;
		}
	}
	catch(...){
		cout << "Unable to open global library file.\n";
	}

	ifstream fin("Library.json");
	if(fin){
		fin.close();
		return writeJSON("Library.json", j);
	}
}
