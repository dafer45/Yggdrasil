#include "IggStream.h"
#include "RSA.h"

#include <fstream>
#include <iostream>

using namespace std;
using namespace Ygg;

int main(int argc, char **argv){
	RSA rsa;
	rsa.setPrivateKey(
		"-----BEGIN PRIVATE KEY-----\n"
		"MIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQDjlOUK8bVlkH5X"
		"MasLuaogaUlgEQ2WmFxh/1suh0QJSoXOMTcIG8U+QeDe/M067fZ94MJRwjTpfXqC"
		"NDlWd0mEZq3EsXcx8Ple0u78OmLNpkD/V97jYPzu8JSos7yVRvcV8e8bzGFJxzRu"
		"qE/3dJmsxJn5V4rWBfTs7Rhv7Ah4Rrg6nWZRk5D7O3CwQNzuGnZFoAtaA6XsDeWI"
		"beQrmpfoyiPOgFkAM04goQURJwEYnUmDrdu6LjLATM2WNg1LLK0CRvOrQY5nGmBV"
		"DDrLL9noYugxDO9PwMk9ailMZmerEYzcWGVRk5kpXHFUQlVZVH0kWSCyv8ETUz6k"
		"Z6o/fJGxAgMBAAECggEBAJJlf4kwv02cS4lvi+Y5AvVihS7lCWaskBECIPenT97i"
		"L3zaL9ppctJ/slX/LU4RMZT79kR9t2fTrfEAAzpZ9dLT8kgrGM/pgaC787wRGunL"
		"XthjACAojcxuulJjfJZSVjOps/Sf3p2FJXdhkj9OpGR8+8dnxwmyP0IBqsykezZS"
		"oTPsEboINkII4GgpP2DC7r5NZFGSkCD8ncJsHNGJp4OZfcaG52yvQYmRah8ZhDYD"
		"ENJkpEAenmDN9Eg+6xDBahMU2/Iz5i3R58YtTCs88BW127EMe5NUhEVYgFXFd0/3"
		"zbmnM1pp7BfysW80afDjqvcsFx82aOFdbNKrB36nWnkCgYEA9EzjtkQ7G5s1L3Cb"
		"FQPhavkLPKljIBcY6OhvCiLT1IBB8ZConW8F0FiRwj24DR6dcI9P24PX6rZMBBMg"
		"+ydsjRFAEpVvCvvGzDv4eJjOYsX5AVxv3Vcjh+S3MSJt8f4l2pnnhnUXUNlcgnUi"
		"zPYoW035li1UQlrmVuzAAQSlAwcCgYEA7nsIzQ2jK+XK2h70tnNfPhTGcHoajjCx"
		"tllTZWwRwEFVrX+5W95gw+miWTSkerOZt4XzNTICGlRgMKT1f6OYg/y9rSXOSN70"
		"3R/DMB20HWFotxJbSkzAuHBpxlHwtGggNlZghH6GkOcWmVk9bSBnGJP1hwcXuD1P"
		"1FJU9Dzs/4cCgYAvLl5YCCtmNH58EAJTd7FkSH7ME2aqfut3p9QyH/HLO44AylBW"
		"P2ffCgv0eJkAJ6Z+Y58npPnhQZ3Ct7MJ7l4/jFJUhHmypFeLgBWHrPNHDjb99U1B"
		"cR/SkfBzU9PqBYxoAIs/Y4PCQIhmRZsX/H5ky9VlFJIOUqHjrcuTJsN7pwKBgHE6"
		"ITeaXfqdo+5j/DQUGwhSvNTyZGFWZUn6p0nkQZlSa+dwWQVxU+gwqVBJWj6Xv0gP"
		"ZSXAlJcDsJfn+lebyPIBsdzfsk2O7ht+1SGnOLkqYplpMcbZIjdvyAjNJsfBGGo0"
		"IKdHm4R485XjdPK3C7vzpAN0LqLfGWPpo0RDGRNJAoGAX860zGQjGtCIa7QkFFeF"
		"1lTmV8ruAiOBxUZiRLsXx2cpV+7BzapJ0Y72Cxu7lOPgzJ4yoiBzV+c7bSvQXSFg"
		"bs834+kVGOKR2CXi9jSwobZij/xZuicOnKRQdlx9LgxAgr97GLqj9Psa+cLSYJlg"
		"sQUCQuyFy8XXZ08dNGjHY4Y=\n"
		"-----END PRIVATE KEY-----"
	);
	string signature = rsa.getSignature("{\"makefile\":{\"store\":\"File\"}}");
	string base64 = rsa.encode64(signature);
	cout << "Signature:\n" << signature << "\n";
	cout << "Base64:\n" << base64 << "\n";

	rsa.setPublicKey(
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
	if(rsa.verifySignature(signature, "{\"makefile\":{\"store\":\"File\"}}"))
		cout << "Success!\n";
	else
		cout << "Failure :(\n";

	exit(1);

	IggStream iggStream;
	iggStream.setStore("File<LibraryFile.json>");
	iggStream.setResource("makefile");
	iggStream.setPublicKey(
/*		"-----BEGIN PUBLIC KEY-----"
		"MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA5mBS4GxfUWfqyN+KsMQp"
		"i4ytnxyv0Oc7bLT30P/N8h9bmgCt+7pOB8JekaHW6dL1PGgEEuUl6vYYdzJ4VSzw"
		"yc9CkNMRf/caCMaKwAI2AhRhnSuIzQvZKT2ObHE8wLQ2gRaf08zrZXtz4GdSO5Lp"
		"2JLVoleVkxi4NbJeCC9J6afYqgeZ/0lUiebZp7mbrpBtrLnJDiniAzIrVIwED4fO"
		"kxxVQ7nJgg8N2XIAPwCy5C8cbWydI8tmggj0zJM6NLStE8m7uJMQhdXqqPqYvwbL"
		"z3OUUCE0/gdY+UnxaFXVxg7OeWxyo2kO1H05QUZpC5JIsAZwhSkMVsVcDtOj6lqY"
		"iwIDAQAB"
		"-----END PUBLIC KEY-----"*/
		"-----BEGIN PUBLIC KEY-----\n"
		"MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA5mBS4GxfUWfqyN+KsMQp\n"
		"i4ytnxyv0Oc7bLT30P/N8h9bmgCt+7pOB8JekaHW6dL1PGgEEuUl6vYYdzJ4VSzw\n"
		"yc9CkNMRf/caCMaKwAI2AhRhnSuIzQvZKT2ObHE8wLQ2gRaf08zrZXtz4GdSO5Lp\n"
		"2JLVoleVkxi4NbJeCC9J6afYqgeZ/0lUiebZp7mbrpBtrLnJDiniAzIrVIwED4fO\n"
		"kxxVQ7nJgg8N2XIAPwCy5C8cbWydI8tmggj0zJM6NLStE8m7uJMQhdXqqPqYvwbL\n"
		"z3OUUCE0/gdY+UnxaFXVxg7OeWxyo2kO1H05QUZpC5JIsAZwhSkMVsVcDtOj6lqY\n"
		"iwIDAQAB\n"
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
