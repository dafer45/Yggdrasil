#ifndef COM_DAFER45_YGGDRASIL_PARAMETER_EXCEPTION
#define COM_DAFER45_YGGDRASIL_PARAMETER_EXCEPTION

#include "Exception.h"

#include <string>

namespace Ygg{

class ParameterException : public Exception{
public:
	/** Constructor. */
	ParameterException(
		const std::string& function,
		const std::string& where,
		const std::string& message,
		const std::string& hint
	);

	/** Destructor. */
	virtual ~ParameterException();
private:
};

};	//End of namespace Ygg

#endif
