#include "IggStream.h"

#include <iostream>

using namespace std;
using namespace Ygg;

int main(int argc, char **argv){
	IggStream iggStream;
	iggStream.setStore("File<LibraryFile.json>");
	iggStream.setResource("Example file 1");
	iggStream.open();

	while(!iggStream.eof()){
		string line;
		getline(iggStream, line);
		cout << line << "\n";
	}

	return 0;
}
