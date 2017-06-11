#include "CryptographicException.h"
#include "SHA1.h"
#include "YGGMacros.h"

#include <openssl/sha.h>

using namespace std;

namespace Ygg{

SHA1::SHA1(){
}

SHA1::~SHA1(){
}

string SHA1::calculateHash(istream& stream) const{
	SHA_CTX context;
	if(!SHA1_Init(&context)){
		throw CryptographicException(
			"SHA1::calculateHash()",
			YGGWhere,
			"Unable to initialize SHA context.",
			""
		);
	}

	while(!stream.eof()){
		//This implementation is suboptimal. Should read larger chunks,
		//but first have to properly handle that the final chunk would
		//not necessarily be full size.
		const int CHUNK_SIZE = 1;
		string chunk(CHUNK_SIZE, ' ');
		stream.read(&chunk[0], CHUNK_SIZE);
		if(stream.eof())
			break;
		if(!SHA1_Update(&context, (unsigned char*)chunk.c_str(), CHUNK_SIZE)){
			throw CryptographicException(
				"SHA1::calculateHash()",
				YGGWhere,
				"An error occured while calculating the hash.",
				""
			);
		}
	}

	unsigned char result[SHA_DIGEST_LENGTH];
	if(!SHA1_Final(result, &context)){
		throw CryptographicException(
			"SHA1::calculateHash()",
			YGGWhere,
			"An error occured while finalizing the SHA1"
				+ string(" calculation."),
			""
		);
	}

	string hashString;
	for(unsigned int n = 0; n < SHA_DIGEST_LENGTH; n++){
		unsigned int mostSignificant = (unsigned int)(result[n]/16);
		unsigned int leastSignificant = (unsigned int)(result[n]%16);
		if(mostSignificant > 9)
			hashString += (char)(mostSignificant + 87);
		else
			hashString += (char)(mostSignificant + 48);
		if(leastSignificant > 9)
			hashString += (char)(leastSignificant + 87);
		else
			hashString += (char)(leastSignificant + 48);
	}

	return hashString;
}

};	//End of namespace Ygg
