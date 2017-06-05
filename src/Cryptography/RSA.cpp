#include "CryptographicException.h"
#include "RSA.h"
#include "YGGMacros.h"

#include <cassert>
#include <iostream>
#include <limits>

#include <openssl/evp.h>
#include <openssl/pem.h>

using namespace std;

namespace Ygg{

RSA::RSA(){
}

RSA::~RSA(){
}

string RSA::getSignature(const string& message) const{
	EVP_MD_CTX* messageDigestContext = EVP_MD_CTX_create();
	if(!messageDigestContext){
		throw CryptographicException(
			"RSA::getSignature()",
			YGGWhere,
			"Unable to initialize message digest context.",
			""
		);
	}

	EVP_PKEY* pkey = EVP_PKEY_new();
	BIO* bio = BIO_new_mem_buf(privateKey.c_str(), privateKey.size());
	PEM_read_bio_PrivateKey(bio, &pkey, nullptr, nullptr);
	BIO_free(bio);

	if(
		1 != EVP_DigestSignInit(
			messageDigestContext,
			nullptr,
			EVP_sha1(),
			nullptr,
			pkey
		)
	){
		throw CryptographicException(
			"RSA::getSignature()",
			YGGWhere,
			"Unable to initialize digest signing.",
			""
		);
	}

	if(
		1 != EVP_DigestSignUpdate(
			messageDigestContext,
			message.c_str(),
			message.size()
		)
	){
		throw CryptographicException(
			"RSA::getSignature()",
			YGGWhere,
			"Unable to update digest signing.",
			""
		);
	}

	size_t signatureSize;
	if(
		1 != EVP_DigestSignFinal(
			messageDigestContext,
			nullptr,
			&signatureSize
		)
	){
		throw CryptographicException(
			"RSA::getSignature()",
			YGGWhere,
			"Unable to get signature size.",
			""
		);
	}

	unsigned char* signature = new unsigned char[signatureSize];
	if(!signature){
		throw CryptographicException(
			"RSA::getSignature()",
			YGGWhere,
			"Unable to allocate signature.",
			""
		);
	}

	if(
		1 != EVP_DigestSignFinal(
			messageDigestContext,
			signature,
			&signatureSize
		)
	){
		throw CryptographicException(
			"RSA::getSignature()",
			YGGWhere,
			"Unable to extract signature.",
			""
		);
	}

	EVP_PKEY_free(pkey);
	EVP_MD_CTX_destroy(messageDigestContext);

	string result((char*)signature, (char*)signature + signatureSize/sizeof(char));
	delete [] signature;

	return result;
}

bool RSA::verifySignature(const string& signature, const string& message) const{
	EVP_MD_CTX* messageDigestContext = EVP_MD_CTX_create();
	if(!messageDigestContext){
		throw CryptographicException(
			"RSA::verifySignature()",
			YGGWhere,
			"Unable to initialize message digest context.",
			""
		);
	}

	EVP_PKEY* pkey = EVP_PKEY_new();
	BIO* bio = BIO_new_mem_buf(publicKey.c_str(), publicKey.size());
	PEM_read_bio_PUBKEY(bio, &pkey, nullptr, nullptr);
	BIO_free(bio);

	if(
		1 != EVP_DigestVerifyInit(
			messageDigestContext,
			nullptr,
			EVP_sha1(),
			nullptr,
			pkey
		)
	){
		throw CryptographicException(
			"RSA::verifySignature()",
			YGGWhere,
			"Unable to initialize digest verification.",
			""
		);
	}

	if(
		1 != EVP_DigestVerifyUpdate(
			messageDigestContext,
			message.c_str(),
			message.size()
		)
	){
		throw CryptographicException(
			"RSA::getSignature()",
			YGGWhere,
			"Unable to update digest verification.",
			""
		);
	}

	bool success;
	if(
		1 == EVP_DigestVerifyFinal(
			messageDigestContext,
			(const unsigned char*)signature.c_str(),
			signature.size()
		)
	){
		success = true;
	}
	else{
		success = false;
	}

	EVP_PKEY_free(pkey);
	EVP_MD_CTX_destroy(messageDigestContext);

	return success;
}

/* Credits goes to Omnifarious
 * (https://stackoverflow.com/users/167958/omnifarious)
 * for having shared encode64 and decode64 on stackoverflow
 * (https://stackoverflow.com/a/5291537/8110110) */
string RSA::encode64(const string& message) const{
	static const char b64Table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	if(message.size() > (numeric_limits<string::size_type>::max()/4u)*3u){
		cout << "To large string.\n";
		exit(1);
	}

	string result((((message.size() + 2)/3)*4), '=');
	size_t outpos = 0;
	int bits_collected = 0;
	unsigned int accumulator = 0;
	const string::const_iterator end = message.end();

	for(string::const_iterator i = message.begin(); i != end; ++i){
		accumulator = (accumulator << 8) | (*i & 0xffu);
		bits_collected += 8;
		while(bits_collected >= 6){
			bits_collected -= 6;
			result[outpos++] = b64Table[(accumulator >> bits_collected) & 0x3fu];
		}
	}
	if(bits_collected > 0){
		assert(bits_collected < 6);
		accumulator <<= 6 - bits_collected;
		result[outpos++] = b64Table[accumulator & 0x3fu];
	}
	assert(outpos >= (result.size()-2));
	assert(outpos <= result.size());

	return result;
}

string RSA::decode64(const string& message) const{
	static const char reverse_table[128] = {
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
		64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
		15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
		64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
		41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64
	};

	string result;
	const string::const_iterator end = message.end();
	int bits_collected = 0;
	unsigned int accumulator = 0;

	for(string::const_iterator i = message.begin(); i != end; ++i){
		const int c = *i;
		if(isspace(c) || c == '=')
			continue;
		if((c > 127) || (c < 0) || (reverse_table[c] > 63)){
			cout << "Decode error.\n";
			exit(1);
		}
		accumulator = (accumulator << 6) | reverse_table[c];
		bits_collected += 6;
		if(bits_collected >= 8){
			bits_collected -= 8;
			result += (char)((accumulator >> bits_collected) & 0xffu);
		}
	}

	return result;
}

};	//End of namespace Ygg
