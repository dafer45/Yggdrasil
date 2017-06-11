#ifndef COM_DAFER45_YGGDRASIL_WRITE_EXCEPTION
#define COM_DAFER45_YGGDRASIL_WRITE_EXCEPTION

#include "Exception.h"

#include <string>

namespace Ygg{

class WriteException : public Exception{
public:
	/** Constructor. */
	WriteException(
		const std::string& function,
		const std::string& where,
		const std::string& message,
		const std::string& hint
	);

	/** Destructor. */
	virtual ~WriteException();
private:
};

};	//End of namespace Ygg

#endif
