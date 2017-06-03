#ifndef COM_DAFER45_YGGDRASIL_IGGSTREAM
#define COM_DAFER45_YGGDRASIL_IGGSTREAM

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

	/***/
	void setHashType(HashType hashType);

	/** Set hash. */
	void setHash(const std::string& hash);
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

};	//End of namespace Ygg

#endif
