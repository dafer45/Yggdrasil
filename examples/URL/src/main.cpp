#include "IggStream.h"

#include <iostream>

using namespace std;
using namespace Yggdrasil;

int main(int argc, char **argv){
	IggStream iggStream;
	iggStream.setStore("URL");
	iggStream.setResource("https://www.georuler.com");
	iggStream.open();

	while(!iggStream.eof()){
		string line;
		getline(iggStream, line);
		cout << line << "\n";
	}

	return 0;
}
