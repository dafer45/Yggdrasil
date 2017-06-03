#ifndef COM_DAFER45_YGGDRASIL_INVALID_HASH_EXCEPTION
#define COM_DAFER45_YGGDRASIL_INVALID_HASH_EXCEPTION

#include "Exception.h"

#include <string>

namespace Ygg{

class InvalidHashException : public Exception{
public:
	/** Constructor. */
	InvalidHashException(
		const std::string& function,
		const std::string& where,
		const std::string& message,
		const std::string& hint
	);

	/** Destructor. */
	virtual ~InvalidHashException();
private:
	/** Exception description. */
	std::string whatString;
};

};	//End of namespace Ygg

#endif
