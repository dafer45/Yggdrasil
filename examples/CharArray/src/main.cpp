#include "IggStream.h"

#include <iostream>

using namespace std;
using namespace Ygg;

int main(int argc, char **argv){
	char charArray[40] = "Hello world!\n\nThis is a string stream.\n";

	IggStream iggStream;
	iggStream.setStore("String");
	iggStream.setResource(charArray);
	iggStream.open();

	while(!iggStream.eof()){
		string line;
		getline(iggStream, line);
		cout << line << "\n";
	}

	return 0;
}
