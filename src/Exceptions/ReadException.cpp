#include "ReadException.h"

namespace Ygg{

ReadException::ReadException(
	const std::string& function,
	const std::string& where,
	const std::string& message,
	const std::string& hint
) : Exception(function, where, message, hint){
}

ReadException::~ReadException(){
}

};	//End of namespace Ygg
