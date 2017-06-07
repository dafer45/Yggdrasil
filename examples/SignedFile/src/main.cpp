#include "IggStream.h"
#include "RSA.h"

#include <fstream>
#include <iostream>

using namespace std;
using namespace Ygg;

int main(int argc, char **argv){
	IggStream iggStream;
	iggStream.setStore("File<Library2.json>");
	iggStream.setResource("makefile");
	iggStream.setPublicKey(
		"-----BEGIN PUBLIC KEY-----\n"
		"MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA45TlCvG1ZZB+VzGrC7mq"
		"IGlJYBENlphcYf9bLodECUqFzjE3CBvFPkHg3vzNOu32feDCUcI06X16gjQ5VndJ"
		"hGatxLF3MfD5XtLu/DpizaZA/1fe42D87vCUqLO8lUb3FfHvG8xhScc0bqhP93SZ"
		"rMSZ+VeK1gX07O0Yb+wIeEa4Op1mUZOQ+ztwsEDc7hp2RaALWgOl7A3liG3kK5qX"
		"6MojzoBZADNOIKEFEScBGJ1Jg63bui4ywEzNljYNSyytAkbzq0GOZxpgVQw6yy/Z"
		"6GLoMQzvT8DJPWopTGZnqxGM3FhlUZOZKVxxVEJVWVR9JFkgsr/BE1M+pGeqP3yR"
		"sQIDAQAB\n"
		"-----END PUBLIC KEY-----"
	);
	iggStream.open();

	while(!iggStream.eof()){
		string line;
		getline(iggStream, line);
		cout << line << "\n";
	}

	return 0;
}
