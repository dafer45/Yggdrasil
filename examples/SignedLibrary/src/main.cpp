#include "Exception.h"
#include "IggStream.h"

#include <iostream>

using namespace std;
using namespace Ygg;

int main(int argc, char **argv){
	try{
		IggStream iggStream;
		iggStream.setStore("File<LibraryFile.json>");
		iggStream.setResource("LibraryLinkedFile");
		iggStream.setHashType(YggStream::HashType::SHA1);
		iggStream.setHash("f2b25e902fb699de9ce0043e2f4af4290a2aead4");
		iggStream.open();

		while(!iggStream.eof()){
			string line;
			getline(iggStream, line);
			cout << line << "\n";
		}
	}
	catch(Exception e){
		e.print();
	}

	return 0;
}
