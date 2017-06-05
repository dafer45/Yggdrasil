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

void addLibraryEntry(
	const string& libraryFilename,
	const LibraryEntry& libraryEntry
);
void addPublicKey(
	const string& libraryFilename,
	const string& publicKeyFilename
);
void configure(const Configuration& configuration);
void createLibrary(const string& filename);
string getKey(const string& filename);
void list(const string& libraryFilename);
void printUsageInfo();
json readJSON(const string& filename);
json readLibrary(const string& filename);
void sign(const string& libraryFilename, const string& privateKey);
void verify(const string& libraryFilename, const string& publicKey);
void writeJSON(const string& filename, const json& j);
void writeLibrary(const string& filename, const json& j);

int main(int argc, char **argv){
	if(argc < 2)
		printUsageInfo();

	string command = argv[1];

//	string libraryFilename = "Library.json";
	string libraryFilename = "";
	string privateKeyFilename = "";
	string publicKeyFilename = "";

	string currentLibrary = "";

	string key;
	string store;
	string resource;

	while(true){
		static struct option longOptions[] = {
			//Sets flags.
			//Does not set flags.
			{"current-library",	required_argument,	0,	'c'},
			{"key",			required_argument,	0,	'k'},
			{"library",		required_argument,	0,	'l'},
			{"private-key",		required_argument,	0,	'P'},
			{"public-key",		required_argument,	0,	'p'},
			{"resource",		required_argument,	0,	'r'},
			{"store",		required_argument,	0,	's'},
			{0,			0,			0,	0}
		};

		int optionIndex = 0;
		int c = getopt_long(argc, argv, "c:k:l:P:p:r:s:", longOptions, &optionIndex);
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
		case 'c':
			currentLibrary = optarg;
			break;
		case 'k':
			key = optarg;
			break;
		case 'l':
			libraryFilename = optarg;
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

	if(command.compare("create") == 0){
		createLibrary(libraryFilename);
	}
	else if(command.compare("config") == 0){
		Configuration configuration;
		configuration.setCurrentLibrary(currentLibrary);
		configure(configuration);
	}
	else if(command.compare("add-public-key") == 0){
		addPublicKey(libraryFilename, publicKeyFilename);
	}
	else if(command.compare("add") == 0){
		LibraryEntry libraryEntry;
		libraryEntry.setKey(key);
		libraryEntry.setStore(store);
		libraryEntry.setResource(resource);
		addLibraryEntry(libraryFilename, libraryEntry);
	}
	else if(command.compare("ls") == 0){
		list(libraryFilename);
	}
	else if(command.compare("sign") == 0){
		sign(libraryFilename, privateKeyFilename);
	}
	else if(command.compare("verify") == 0){
		verify(libraryFilename, publicKeyFilename);
	}
	else{
		printUsageInfo();
	}

	return 0;
}

void addLibraryEntry(
	const string& libraryFilename,
	const LibraryEntry& libraryEntry
){
	json library = readLibrary(libraryFilename);
	try{
		library.at("content")[libraryEntry.getKey()] = libraryEntry.getJSONValue();
	}
	catch(...){
		cout << "Unable to add library entry.\n";
	}
	writeLibrary(libraryFilename, library);
}

void addPublicKey(
	const string& libraryFilename,
	const string& publicKeyFilename
){
	string publicKey = getKey(publicKeyFilename);

	json library = readLibrary(libraryFilename);
	try{
		library["publicKey"] = publicKey;
		writeLibrary(libraryFilename, library);
	}
	catch(json::exception e){
		cout << "Unable to parse json file.\n";
		exit(1);
	}
}

void configure(const Configuration& configuration){
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

	try{
		json config = readJSON(filename);
		Configuration c(config);

		if(configuration.getCurrentLibrary().compare("") != 0){
			ifstream fin(homeDir + "/.ygg/" + configuration.getCurrentLibrary() + "/Library.json");
			if(!fin){
				cout << "Library does not exist.\n";
				exit(1);
			}
			c.setCurrentLibrary(configuration.getCurrentLibrary());
		}

		writeJSON(filename, c.getJSON());
	}
	catch(...){
		cout << "Unable to update configuration file.\n";
	}
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
//		cout << f << "\n";
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

void list(const string& libraryFilename){
	json library = readLibrary(libraryFilename);
//	cout << library.dump(4) << "\n";
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
	cout << setw(INDENT) << "" << setw(20) << "config"		<< "Configure global settings.\n";
	cout << setw(INDENT) << "" << setw(20) << "create"		<< "Create library file.\n";
	cout << setw(INDENT) << "" << setw(20) << "ls"			<< "Display content.\n";
	cout << setw(INDENT) << "" << setw(20) << "sign"		<< "Sign file with private key.\n";
	cout << setw(INDENT) << "" << setw(20) << "verify"		<< "Verify file with public key.\n";
	cout << "\n";
	cout << "Flags:\n";
	cout << setw(INDENT) << "" << setw(20) << "--current-library"	<< "\n";
	cout << setw(INDENT) << "" << setw(20) << "--key"		<< "\n";
	cout << setw(INDENT) << "" << setw(20) << "--library"		<< "\n";
	cout << setw(INDENT) << "" << setw(20) << "--private-key"	<< "\n";
	cout << setw(INDENT) << "" << setw(20) << "--public-key"	<< "\n";
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

json readLibrary(const std::string& filename){
	if(filename.compare("") != 0)
		return readJSON(filename);

	ifstream fin("Library.json");
	if(fin){
		fin.close();
		return readJSON("Library.json");
	}

	try{
		string homeDir = getenv("HOME");
		string filename = homeDir + "/.ygg/config";
		Configuration configuration(readJSON(filename));
		string currentLibrary = configuration.getCurrentLibrary();
		return readJSON(homeDir + "/.ygg/" + currentLibrary + "/Library.json");
	}
	catch(...){
		cout << "Unable to open global library file.\n";
	}
}

void sign(const string& libraryFilename, const string& privateKeyFilename){
	json library = readLibrary(libraryFilename);
	try{
		json content = library.at("content");
		RSA rsa;
		rsa.setPrivateKey(getKey(privateKeyFilename));
		string signature = rsa.getSignature(content.dump());
		library["signature"] = rsa.encode64(signature);
		writeLibrary(libraryFilename, library);
	}
	catch(...){
		cout << "Unable to sign file.\n";
		exit(1);
	}
}

void verify(const string& libraryFilename, const string& publicKeyFilename){
	json library = readLibrary(libraryFilename);
	try{
		RSA rsa;
		string content = library.at("content").dump();
		string signature = library.at("signature").dump();
		signature.erase(signature.size()-1, 1).erase(0, 1);
		signature = rsa.decode64(signature);
		rsa.setPublicKey(getKey(publicKeyFilename));
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

void writeJSON(const string& filename, const json& j){
	ofstream fout(filename);
	if(!fout){
		cout << "Unable to open file '" << filename << "'.";
		exit(1);
	}

	fout << j.dump();
	fout.close();
}

void writeLibrary(const std::string& filename, const json& j){
	if(filename.compare("") != 0)
		return writeJSON(filename, j);

	ifstream fin("Library.json");
	if(fin){
		fin.close();
		return writeJSON("Library.json", j);
	}

	try{
		string homeDir = getenv("HOME");
		string filename = homeDir + "/.ygg/config";
		Configuration configuration(readJSON(filename));
		string currentLibrary = configuration.getCurrentLibrary();
		writeJSON(homeDir + "/.ygg/" + currentLibrary + "/Library.json", j);
	}
	catch(...){
		cout << "Unable to open global library file.\n";
	}
}
