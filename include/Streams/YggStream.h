#ifndef COM_DAFER45_YGGDRASIL_YGGSTREAM
#define COM_DAFER45_YGGDRASIL_YGGSTREAM

#include <algorithm>
#include <string>

namespace Ygg{

/** Base class for IggStream (input) and OggStream (output). */
class YggStream{
public:
	/** Enum class for specifying hash type. */
	enum class HashType {None, SHA1};

	/** Constructor. */
	YggStream();

	/** Destructor. */
	~YggStream();

	/** Set source. */
	void setStore(const std::string& store);

	/** Set resource. */
	void setResource(const std::string& resource);
protected:
	/** Get store. */
	const std::string getStore() const;

	/** Get resource. */
	const std::string getResource() const;

	/** Find resource. */
	void locateResource();

	/** Set hash type. */
	void setHashType(HashType hashType);

	/** Get hash type. */
	HashType getHashType() const;

	/** Set hash. */
	void setHash(const std::string& hash);

	/** Get hash. */
	const std::string& getHash() const;

	/** Set public key. */
	void setPublicKey();
private:
	/** Store. */
	std::string store;

	/** Resource. */
	std::string resource;

	/** Hash type. */
	HashType hashType;

	/** Hash of the string content. */
	std::string hash;

	/** Public key. */
	std::string publicKey;

	/** Convert string to lower case. */
	std::string toLowerCase(const std::string& str) const;
};

inline void YggStream::setStore(const std::string& store){
	this->store = store;
}

inline void YggStream::setResource(const std::string& resource){
	this->resource = resource;
}

inline void YggStream::setHashType(const HashType hashType){
	this->hashType = hashType;
}

inline YggStream::HashType YggStream::getHashType() const{
	return hashType;
}

inline void YggStream::setHash(const std::string& hash){
	this->hash = hash;
}

inline const std::string& YggStream::getHash() const{
	return hash;
}

inline std::string YggStream::toLowerCase(const std::string& str) const{
	std::string result = str;
	std::transform(
		str.begin(),
		str.end(),
		result.begin(),
		::tolower
	);

	return result;
}

};	//End of namespace Ygg

#endif
