#include "InvalidHashException.h"

namespace Ygg{

InvalidHashException::InvalidHashException(
	const std::string& function,
	const std::string& where,
	const std::string& message,
	const std::string& hint
) : Exception(function, where, message, hint){
}

InvalidHashException::~InvalidHashException(){
}

};	//End of namespace Ygg
