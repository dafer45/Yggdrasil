#include "Configuration.h"
#include "CryptographicException.h"
#include "IggStream.h"
#include "Library.h"
#include "LibraryEntry.h"
#include "ParameterException.h"
#include "ReadException.h"
#include "RSA.h"
#include "SHA1.h"

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

string argumentToKey(const string& argument);
string argumentToStore(const string& argument);
string argumentToResource(const string& argument);
string argumentToHash(const string& argument, const string& hashType);
void configure(const Configuration& configuration);
void createLibrary(const string& filename);
string getKey(const string& filename);
void list();
void printUsageInfo();
json readJSON(const string& filename);
void writeConfiguration(const Configuration& configuration);
void writeJSON(const string& filename, const json& j);

int main(int argc, char **argv){
	try{
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

		string key;
		string store;
		string resource;
		string hash;
		string hashType = "sha1";

		while(true){
			static struct option longOptions[] = {
				//Sets flags.
				//Does not set flags.
				{"hash",		required_argument,	0,	'h'},
				{"hash-type",		required_argument,	0,	'H'},
				{"key",			required_argument,	0,	'k'},
				{"private-key",		required_argument,	0,	'P'},
				{"public-key",		required_argument,	0,	'p'},
				{"resource",		required_argument,	0,	'r'},
				{"store",		required_argument,	0,	's'},
				{0,			0,			0,	0}
			};

			int optionIndex = 0;
			int c = getopt_long(argc, argv, "H:h:k:P:p:r:s:", longOptions, &optionIndex);
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
			case 'H':
				hashType = optarg;
				break;
			case 'h':
				hash = optarg;
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
				libraryEntry.setHash(argumentToHash(argument, hashType));
			}

			if(key.size() != 0)
				libraryEntry.setKey(key);
			if(store.size() != 0)
				libraryEntry.setStore(store);
			if(resource.size() != 0)
				libraryEntry.setResource(resource);
			if(hash.size() != 0)
				libraryEntry.setHash(hash);
			if(libraryEntry.getHash().compare("") != 0)
				libraryEntry.setHashType(hashType);

			if(libraryEntry.getKey().compare("") == 0){
				throw ParameterException(
					"main()",
					YGGWhere,
					"Key not specified.",
					"Pass key as argument or using the --key flag."
				);
			}
			if(libraryEntry.getStore().compare("") == 0){
				throw ParameterException(
					"main()",
					YGGWhere,
					"Unable to determine store type.",
					"Use --store flag to specify store."
				);
			}
			Library library;
			library.load();
			library.addLibraryEntry(libraryEntry);
			library.save();
		}
		else if(command.compare("cat") == 0){
			if(argument.size() != 0){
				Library library;
				library.load();

				LibraryEntry libraryEntry = library.getLibraryEntry(argument);

				IggStream iggStream;
				iggStream.open(libraryEntry);
				while(!iggStream.eof()){
					string line;
					getline(iggStream, line);
					cout << line << "\n";
				}
			}
			else{
				throw ParameterException(
					"main()",
					YGGWhere,
					"cat requires one argument.",
					""
				);
			}
		}
		else if(command.compare("set-public-key") == 0){
			string publicKey = getKey(publicKeyFilename);
			Library library;
			library.load();
			library.setPublicKey(publicKey);
			library.save();
		}
		else if(command.compare("create") == 0){
			createLibrary(argument);
		}
		else if(command.compare("info") == 0){
			Configuration configuration;
			configuration.load();
			configuration.print();
		}
		else if(command.compare("link") == 0){
			LibraryEntry libraryEntry;
			if(argument.size() != 0){
				libraryEntry.setStore(
					argumentToStore(argument) + "<"
					+ argument + ">"
				);
			}
			else{
				throw ParameterException(
					"main()",
					YGGWhere,
					"link requires one argument.",
					""
				);
			}

			Library child;
			child.load(libraryEntry);

			Library parent;
			parent.load();
			parent.link(child);
			parent.save();
		}
		else if(command.compare("ls") == 0){
			list();
		}
		else if(command.compare("release-library") == 0){
			Configuration configuration;
			configuration.load();
			configuration.setCurrentLibrary("");
			writeConfiguration(configuration);
		}
		else if(command.compare("remove") == 0){
			LibraryEntry libraryEntry;
			if(argument.size() != 0){
				libraryEntry.setKey(argument);

				Library library;
				library.load();
				library.removeLibraryEntry(libraryEntry);
				library.save();
			}
			else{
				cout << "remove requires one argument.\n";
				exit(1);
			}
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
			Library library;
			library.load();
			library.sign(getKey(privateKeyFilename));
			library.save();
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
			if(hash.size() != 0)
				libraryEntry.setHash(hash);
			if(libraryEntry.getHash().compare("") != 0)
				libraryEntry.setHashType(hashType);

			Library library;
			library.load();
			library.updateLibraryEntry(libraryEntry);
			library.save();
		}
		else if(command.compare("verify") == 0){
			Library library;
			library.load();
			library.verify(getKey(publicKeyFilename));
		}
		else{
			printUsageInfo();
		}
	}
	catch(ParameterException e){
		e.print();
		cout << e.getMessage() << "\n";
		if(e.getHint().size() != 0)
			cout << "\t" << e.getHint() << "\n";
	}
	catch(Exception e){
		e.print();
		cout << e.what() << "\n";
	}

	return 0;
}

string argumentToKey(const string& argument){
	size_t pos = argument.find("://");
	if(pos != string::npos)
		return argument.substr(pos + 3, argument.size() - pos - 3);

	ifstream fin(argument);
	if(fin){
		fin.close();
		return argument;
	}
	else{
		throw ReadException(
			"argumentToKey()",
			YGGWhere,
			"No such file.",
			""
		);
	}
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

string argumentToHash(const string& argument, const string& hashType){
	size_t pos = argument.find("://");
	if(pos != string::npos)
		return "";

	ifstream fin(argument);
	if(fin){
		SHA1 sha1;
		string hash = sha1.calculateHash(fin);
		fin.close();

		return hash;
	}
	else{
		return "";
	}
}

void configure(const Configuration& configuration){
	Configuration c;
	c.load();
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
	fout << "{\"content\":{\"entries\":{},\"publicKeys\":{}}}";
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

void list(){
	Library l;
	l.load();
	const json& library = l.getJSON();
	try{
		json content = library.at("content").at("entries");
		cout << left;
		cout << setw(20) << "Key" << setw(20) << "Store" << setw(80) << "Resource" << setw(50) << "Hash" << setw(20) << "HashType" << "\n";
		for(json::iterator it = content.begin(); it != content.end(); ++it){
			string key = it.key();
			string store = it->at("store");
			string resource;
			try{
				resource = it->at("resource");
			}
			catch(json::exception e){
			}
			string hash;
			try{
				hash = it->at("hash");
			}
			catch(json::exception e){
			}
			string hashType;
			try{
				hashType = it->at("hashType");
			}
			catch(json::exception e){
			}
			cout << setw(20) << key << setw(20) << store << setw(80) << resource << setw(50) << hash << setw(20) << hashType << "\n";
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
	cout << setw(INDENT) << "" << setw(20) << "create"		<< "Create library file.\n";
	cout << setw(INDENT) << "" << setw(20) << "info"		<< "Create configuration information.\n";
	cout << setw(INDENT) << "" << setw(20) << "ls"			<< "Display content.\n";
	cout << setw(INDENT) << "" << setw(20) << "release-library"	<< "Release the current library.\n";
	cout << setw(INDENT) << "" << setw(20) << "set-library"		<< "Set the current library.\n";
	cout << setw(INDENT) << "" << setw(20) << "set-public-key"	<< "Set public key.\n";
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
