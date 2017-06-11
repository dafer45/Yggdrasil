#ifndef COM_DAFER45_YGGDRASIL_SHA1
#define COM_DAFER45_YGGDRASIL_SHA1

#include <istream>
#include <string>

namespace Ygg{

class SHA1{
public:
	/** Constructor. */
	SHA1();

	/** Destructor. */
	~SHA1();

	/** Calculate hash. */
	std::string calculateHash(std::istream& stream) const;
};

};	//End of namespace Ygg

#endif
