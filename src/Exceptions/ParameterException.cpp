#include "ParameterException.h"

namespace Ygg{

ParameterException::ParameterException(
	const std::string& function,
	const std::string& where,
	const std::string& message,
	const std::string& hint
) : Exception(function, where, message, hint){
}

ParameterException::~ParameterException(){
}

};	//End of namespace Ygg
