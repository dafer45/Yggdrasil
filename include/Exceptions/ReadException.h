#ifndef COM_DAFER45_YGGDRASIL_READ_EXCEPTION
#define COM_DAFER45_YGGDRASIL_READ_EXCEPTION

#include "Exception.h"

#include <string>

namespace Ygg{

class ReadException : public Exception{
public:
	/** Constructor. */
	ReadException(
		const std::string& function,
		const std::string& where,
		const std::string& message,
		const std::string& hint
	);

	/** Destructor. */
	virtual ~ReadException();
private:
};

};	//End of namespace Ygg

#endif
