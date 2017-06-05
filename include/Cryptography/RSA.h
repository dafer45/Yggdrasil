#ifndef COM_DAFER45_YGGDRASIL_RSA
#define COM_DAFER45_YGGDRASIL_RSA

#include <string>

namespace Ygg{

class RSA{
public:
	/** Constructor. */
	RSA();

	/** Destructor. */
	~RSA();

	/** Set private key. */
	void setPrivateKey(const std::string& privateKey);

	/** Set public key. */
	void setPublicKey(const std::string& publicKey);

	/** Sign. */
	std::string getSignature(const std::string& message) const;

	/** Verify signature. */
	bool verifySignature(
		const std::string& signature,
		const std::string& message
	) const;
//private:
	/** Binary to base64 encoding. */
	std::string encode64(const std::string& message) const;

	/** Base64 to binary decoding. */
	std::string decode64(const std::string& message) const;
private:
	/** Private key. */
	std::string privateKey;

	/** Public key. */
	std::string publicKey;
};

inline void RSA::setPrivateKey(const std::string& privateKey){
	this->privateKey = privateKey;
}

inline void RSA::setPublicKey(const std::string& publicKey){
	this->publicKey = publicKey;
}

};	//End of namespace Ygg

#endif
