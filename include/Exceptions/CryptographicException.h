#ifndef COM_DAFER45_YGGDRASIL_CRYPTOGRAPHIC_EXCEPTION
#define COM_DAFER45_YGGDRASIL_CRYPTOGRAPHIC_EXCEPTION

#include "Exception.h"

#include <string>

namespace Ygg{

class CryptographicException : public Exception{
public:
	/** Constructor. */
	CryptographicException(
		const std::string& function,
		const std::string& where,
		const std::string& message,
		const std::string& hint
	);

	/** Destructor. */
	virtual ~CryptographicException();
private:
};

};	//End of namespace Ygg

#endif
