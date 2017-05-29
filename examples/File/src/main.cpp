#include "IggStream.h"

#include <iostream>

using namespace std;
using namespace Ygg;

int main(int argc, char **argv){
	IggStream iggStream;
	iggStream.setStore("File");
	iggStream.setResource("makefile");
	iggStream.open();

	while(!iggStream.eof()){
		string line;
		getline(iggStream, line);
		cout << line << "\n";
	}

	return 0;
}
