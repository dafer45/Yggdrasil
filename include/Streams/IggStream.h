#ifndef COM_DAFER45_YGGDRASIL_IGGSTREAM
#define COM_DAFER45_YGGDRASIL_IGGSTREAM

#include "LibraryEntry.h"
#include "YggStream.h"

#include <string>
#include <istream>

namespace Ygg{

/** Input stream. */
class IggStream : public YggStream, public std::istream{
public:
	/** Constructor. */
	IggStream();

	/** Destructor. */
	~IggStream();

	/** Open. */
	void open();

	/** Open. */
	void open(const LibraryEntry& libraryEntry);

	/** Set hash type. */
	void setHashType(HashType hashType);

	/** Set hash. */
	void setHash(const std::string& hash);

	/** Set public key. */
	void setPublicKey(const std::string& publicKey);
private:
	/** Verify SHA1 hash. */
	bool verifyHashSHA1();
};

inline void IggStream::setHashType(HashType hashType){
	YggStream::setHashType(hashType);
}

inline void IggStream::setHash(const std::string& hash){
	YggStream::setHash(hash);
}

inline void IggStream::setPublicKey(const std::string& publicKey){
	YggStream::setPublicKey(publicKey);
}

};	//End of namespace Ygg

#endif
